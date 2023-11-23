export module Controller;

import <chrono>;
import <memory>;
import <filesystem>;
import Base;
import IController;
import SystemBus;

using namespace std::chrono;

namespace Emulator
{
	export class DefaultMemoryController final : public IMemoryController
	{
	private:
		size_t memorySize_{};
		std::unique_ptr<uint8_t[]> memory_;
	public:
		//Pass in system bus here.
		//We need to pass in the system bus here in so that the
		//memory can talk directly to the IO
		DefaultMemoryController(uint8_t addressBusSize);

		//IMemoryContoller virtual overrides
		void Load(std::filesystem::path romFile, uint16_t offset) override final;
		size_t Size() const override final;

		//IController virtual overrides
		uint8_t Read(uint16_t address) override final;
		void Write(uint16_t address, uint8_t value) override final;
		ISR ServiceInterrupts(nanoseconds currTime, uint64_t cycles) override final;
	};

	export class MockIoController : public IController
	{
		private:
			/** powerOff_

				Signals the control bus when the current instruction finishes
				executing that it is time to shutdown.

				The signal is sent during the servicing of interrupts as it
				is guaranteed that no instructions are currently executing
				at that time.
			*/
			bool powerOff_{};
		protected:
			void Write(uint16_t ioDeviceNumber, uint8_t value);			
			ISR ServiceInterrupts(nanoseconds currTime, uint64_t cycles);
	};
	
	/** TestIoController

		A minimal implementaion for an io controller. It allows
		reading and writing to one device (device0).

		@discussion		This IO controller is purely academic, it's
						main use is for unit tests, in reality the
						device data would be an interface into actual
						IO, for example, a keyboard or a mouse.
	*/
	export class TestIoController final : public MockIoController
	{
	private:
		/** lastTime_
		
			Track the cpu time so we can trigger interrupts
			at one second intervals.
		*/
		nanoseconds lastTime_;

		/** deviceData_

			The io data to be maintained by this 'io device'.

			@discussion		The mock device will be initialised to an
							arbitary value, this can be useful, for
							example, during tests.
		*/
		uint8_t deviceData_{ 0xAA };
	public:
		/**	Read

			Read the contents of the specifed io device.

			@param	ioDeviceNumber	The io device number to read from.
									Only once device is supported, ie;
									device number 0.

			@return	uint8_t			The contents of the io device at the
									time of the function call.

			@see IController::Read()
		*/
		uint8_t Read(uint16_t ioDeviceNumber) override final;

		/** Write

			Write the specified value to the specified io device.

			@param	ioDeviceNumber	The io device number to write to.
									Only one device is supported, ie;
									device number 0.
			@param	value			The data to write to the io device.

			@see IController::Write()
		*/
		void Write(uint16_t ioDeviceNumber, uint8_t value) override final;

		/** ServiceInterrupts

			Checks the io controller to see if any interrupts are pending.

			@param	currTime	The time in nanoseconds of the cpu clock.

			@return	uint8_t		The interrupt that requires servicing by the
								cpu.

			@discussion			Currently, no interrupts are triggered by this
								mock io controller. The return value will always
								be zero.

			@see IContoller::ServiceInterrupts()
		*/
		ISR ServiceInterrupts(nanoseconds currTime, uint64_t cycles) override final;
	};

	/** CpmIoController

	A basic io controller which emulates 8 bit CP/M BDOS
	console output and output string system calls.

	@discussion		This io controller is used for running the cpudiag tests
					which were originally written targeting CP/M BDOS.
					BDOS calls start from 0x0005 and since we aren't running
					CP/M we can't call 0x0005 to make those system calls, so
					the cpudiag tests program was modified to perform the
					BDOS call through a custom IO, hence eliminating the
					dependence on CP/M BDOS. See cpudiag_readme.txt for
					more information.
	*/
	export class CpmIoController final : public MockIoController
	{
	private:
		enum class Port
		{
			PrintMode,
			AddrHi,
			Process
		};
		
		/** message_

			The final output buffer that will be used to
			print to.
		*/
		std::string message_;

		/** memoryController_

			The memory that the message will be read from.
		*/
		std::shared_ptr<IController> memoryController_;

		/** printMode_

			The BDOS output mode.

			A value of 9 will output characters from memory
			into the message buffer until a '$' character is read.
			A value of 2 will output a single character into
			the output message buffer.
		*/
		uint8_t printMode_{};

		/** addrHi_

			The high 8 bits of the starting 16 bit memory address
			from which characters will be read into the output buffer.
			This value is only used when the print mode is 9.
		*/
		uint8_t addrHi_{};
	public:
		/** CpmIoController

			An io controller that emulates the CP/M BDOS output routine.
		*/
		CpmIoController(const std::shared_ptr<IController>& memoryController);

		/** Message

			Returns the message buffer. The message buffer is then cleared.

			@return		string	The message buffer.
		*/
		std::string Message();

		/** Read

			Not used by this controller.

			@return		uint8_t		Always zero.
		*/
		uint8_t Read(uint16_t ioDeviceNumber) override final;

		/** Write

			Writes the specified value to given port.

			@param	Port			Port::PrintMode -	The character output mode to use when Port::Process is written to.
														9 - Output characters into the output buffer from
															memory until a '$' character is read.
														2 - Output a single charater into the output buffer.
									Port::AddrHi	-	The high 8 bit memory address to use when 9 is written to Port::PrintMode.
									Port::Process	-	Process output message according to the value that was written to Port::PrintMode.
			@param	value			The value to be written to the specified port when Port is PrintMode or AddrHi. When the Port is
									Port::Process the value is either the low 8 bit address when the Port::PrintMode is 9 or the actual
									value to print when Port::PrintMode is 2.
		*/
		void Write(uint16_t Port, uint8_t value) override final;

		/** ServiceInterrupts

			Checks the io controller to see if any interrupts are pending.

			@param	currTime	The time in nanoseconds of the cpu clock.

			@return	uint8_t		The interrupt that requires servicing by the
								cpu.

			@discussion			Currently, no interrupts are triggered by this
								io controller. The return value will always
								be zero.

			@see IContoller::ServiceInterrupts()
		*/
		ISR ServiceInterrupts(nanoseconds currTime, uint64_t cycles) override final;
	};
}

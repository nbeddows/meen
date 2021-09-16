export module IMachine;

import <memory>;
import IController;

namespace Emulator
{
	/** IMachine

	*/
	export struct IMachine
	{
		/** Run
		
			Run the roms loaded into memory initialising execution at the given
			program counter.

			@param	pc					The program counter. When no program counter
										is specified cpu instruction execution will
										begin from memory address 0x00.

			@throw	std::runtime_error	No memory controller has been set on this
										machine.

										@see SetMemoryController

			@discussion					The program counter is the memory address at which the cpu
										will start executing the instructions contained in the
										rom files that were loaded into memory.
		*/
		virtual void Run(uint16_t pc = 0x00) = 0;

		/** SetMemoryController
		
			Set a custom memory controller with the machine.

			@param	controller	The memory controller to be used with this machine.

			@discussion			Not setting a memory controller will cause the Run function
								to throw a std::runtime_error exception.
		*/
		virtual void SetMemoryController (const std::shared_ptr<IController>& controller) = 0;

		/** SetIoController

			Set a custom io controller with the machine.

			@param	controller	The io controller to be used with this machine.

			@discussion		Not setting an io controller, while valid, is NOT recommended.
							This is because without an io controller the machine will have
							no means of exiting via an i/o device or via an i/o device
							interrupt.
							
							A valid reason for having a null io controller would be if you
							wanted to run a machine for a certain period of time.

							For example;

							...
							...

							memoryController_.Load ("myProgram.bin");
							machine_.SetMemoryController (memoryController_);
							//Don't set an io controller

							...
							...
							
							auto future = std::async(std::launch::async, [&]
							{
								machine_.Run();
							});

							auto status = future.wait_for(seconds(2));

							//One can check that status of the machine and
							//memory after 2 seconds of run time.
		*/
		virtual void SetIoController (const std::shared_ptr<IController>& controller) = 0;

		virtual ~IMachine() = default;
	};
}
/*
Copyright (c) 2021-2024 Nicolas Beddows <nicolas.beddows@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef IMACHINE_H
#define IMACHINE_H

#include <expected>
#include <functional>
#include <memory>
#include <string>
#include <system_error>

#include "meen/IController.h"

namespace meen
{
	/** Machine interface

		An abstract representation of a basic machine with a cpu, clock and
		custom memory and IO. Unless specifically stated in this API, there is no
		guarantee in regards to thread safety. All methods should be called
		from the same thread.

		Basic Principles of Operation:

		@code{.cpp}

		// Create a synchronous i8080 machine running as fast as possible 
		auto machine = MakeMachine();

		// Create a custom memory controller (See tests for examples)
		auto customMemoryController = std::make_unique<CustomMemoryController>();

		// Load memory with program via custom controller method
		customMemoryController->LoadProgram("myProgram.com");

		// Create custom IO Controller (See tests for examples)
		auto customIOController = std::make_unique<CustomIOController>();

		// Set the memory and IO controllers with the machine
		machine->SetIOController(customIOController);
		machine->SetMemoryController(customMemoryController);

		// Can be called from a different thread if the runAsync/loadAsync options are specifed
		machine_->OnLoad([]
		{
			// Return the json state to load, could be read from disk for example
			return "json state as passed to OnSave";
		});

		// Can be called from a different thread if the runAsync/saveAsync options are specifed
		machine->OnSave([](const char* json)
		{
			// Handle the machines current state, for example, writing it to disk
			std::cout << json << std::endl;
		});		
		
		// Set the ram/rom sizes (0x2000 and 0x4000) and offsets (0x0000, 0x2000) for this custom memory controller
		// These values are used for load and save requests
		machine_->SetOptions(R"({"romOffset":0,"romSize":8192,"ramOffset":8192,"ramSize":16384})");

		// Set the clock resolution - not setting this will run the
		// machine as fast as possible (default)
		machine->SetOptions(R"({"clockResolution":20000000})"); // 20 milliseconds (50Hz)

		// Run the machine sychronously, it won't return until the custom IO
		// controller ServiceInterrupts override generates an ISR::Quit interrupt
		auto runTime = machine->Run();

		// Run the machine asychronously - this can be done by setting the following json config
		// option either in the MakeMachine factory method or via IMachine::SetOptions
		machine->SetOptions(R"({"runAsync":true})");
		machine->Run();

		// ...
		// Do additional work here while the machine is running
		// ...

		// Will not return until the custom IO
		// controller ServiceInterrupts override generates an ISR::Quit interrupt
		runTime = machine->WaitForCompletion();

		@endcode
	*/
	struct IMachine
	{
		/** Run the machine

			Run the roms loaded into memory initialising execution at the given
			program counter.

			@param	pc					The program counter is the memory address at
										which the cpu will start executing the instructions
										contained in the rom files that were loaded into memory.

			@return						A std::error_code - todo: document the error codes.

			@remark						When no program counter is specified cpu instruction
										execution will begin from memory address 0x00.

			@see SetMemoryController
		*/
		virtual std::error_code Run(uint16_t pc = 0x00) = 0;

		/** Wait for the machine to finish running

			Block the current thread until the machine execution loop has completed.

			@return						A std::expected with an expected value of the duration of the
										run time of the machine as a uint64_t in nanoseconds and an
										unexpected value as a std::error_code.

			@remark						Repeated calls to this method will return the last valid meen run time. 
		*/
		virtual std::expected<uint64_t, std::error_code> WaitForCompletion() = 0;

		/** Set a custom memory controller

			The machine will use this controller when it needs to read
			and/or write data to ram.

			@param	controller		The memory controller to be used with this machine.

			@throws					std::invalid_argument exception when controller is nullptr.

			@throws					std::runtime_error if the machine is currently running.

			@remark					See Tests/TestControllers/MemoryController.cpp
		*/
		virtual std::error_code SetMemoryController (const std::shared_ptr<IController>& controller) = 0;

		/** Set a custom io controller

			The machine will use this controller when it needs to read
			and/or write to an io device.

			@param	controller	The io controller to be used with this machine.

			@throws				std::invalid_argument when the controller is nullptr.

			@throws				std::runtime_error when the machine is currently running.

			@remark				See Tests/TestControllers/TestIoController.cpp.
		*/
		virtual std::error_code SetIoController (const std::shared_ptr<IController>& controller) = 0;

		/** Set machine options

			@param		options		A json string specifying the desired options to update. Passing in an options string of nullptr will set all options
									to their defaults.

			@return					no_error: all options were set successfully.<br>
									unknown_option: all recognised options were set successfully though unrecognised options were found.

			@throws					std::runtime_error or any other exception that the underlying json parser throws (nlohmann json)

			@throws					std::runtime_error when the cpu option is specified or if the machine is currently running.

			@throws					std::invalid_argument when any option value is illegal.

			@remark					The `cpu` configuration option can only be set via the factory machine constructor.

			@see					MakeMachine for supported configuration options.
		*/
		virtual std::error_code SetOptions(const char* options) = 0;

		/** Machine save state completion handler
		
			Registers a method which will be called when the ISR::Save interrupt is triggered. The register
			method accepts a const char* which is the machine save state in json format. 
		
			The json layout of an example save state is specifed in the json snippet below:

			@code{.json}

			{
				"cpu":
				{
					"uuid":"O+hPH516S3ClRdnzSRL8rQ==",	// The base64 unique identifier of the cpu
					"registers":
					{
						"a":0,							// The a register
						"b":0,							// The b register
						"c":0,							// The c register
						"d":0,							// The d register
						"e":0,							// The e register
						"h":0,							// The h register
						"l":0,							// The l register
						"s":2							// The status register
					},
					"pc":0,								// The program counter
					"sp":0								// The stack pointer
				},
			{
			"memory":
			{
				"uuid":"zRjYZ92/TaqtWroc666wMQ==",		// The base64 unique identifier for the memory controller
				"rom":"FkgjfhUYrudiMj7y65789Io=",		// The base64 MD5 hash of the rom
				"ram":
				{
					"encoder":"base64",					// The binary to text encoding for the ram
					"compressor":"zlib",				// The compressor to use for the ram
					"size":57343,						// The size of the uncompressed ram
					"bytes":"... the ram bytes ..."		// The compressed and encoded ram bytes
				}
			}

			@endcode

			@param	onSave				The method to call with the json machine save state after it has has been
										generated via the ISR::Save interrupt. Is mutually exclusive with the OnLoad
										initiation handler.

			@throws						std::runtime_error if the machine is currently running.

			@remark						The function parameter onSave will be called from a different thread from which this
										method was called if the runAsync or saveAsync config options have been specified.

			@remark						Save requests are not queued. When a save is in progress, additional save interrupts
										will be ignored.
			@since	version 1.5.0
		*/
		virtual std::error_code OnSave(std::function<void(const char* json)>&& onSave) = 0;

		/** Machine load state initiation handler
		
			Registers a method that will be called when the ISR::Load interrupt is triggered. The register
			method returns a const char* which is the json machine state to load.

			@param	onLoad				The method to call to get the json machine state to load when the ISR::Load
										interrupt is triggered. Is mutually exclusive with the OnSave completion handler.

			@throws						std::runtime_error if machine is currently running.

			@remark						The function parameter onLoad will be called from a different thread from which this
										method was called if the runAsync or loadAsync config options have been specified.

			@remark						The machine state can fail to load for numerous reasons:
										- when the machine cpu does not match the load state cpu.
										- when the machine memory controller does not match the load state memory controller.
										- when the machine memory rom does not match the load state rom.
										- invalid json file.
			
			@remark						When the format of the returned json string is invalid or a load error occurs the state
										of the machine shall remain unchanged.

			@remark						Load requests are not queued. When a load is in progress, additional load interrupts
										will be ignored.

			@todo						Log when errors occur.

			@since	version 1.5.0
		*/
		virtual std::error_code OnLoad(std::function<const char*()>&& onLoad) = 0;

		/** Destruct the machine

			Release all resources used by this machine instance.
		*/
		virtual ~IMachine() = default;
	};
} // namespace meen

#endif // IMACHINE_H
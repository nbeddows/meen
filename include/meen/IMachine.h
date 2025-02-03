/*
Copyright (c) 2021-2025 Nicolas Beddows <nicolas.beddows@gmail.com>

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
#include <system_error>

#include "meen/Error.h" 
#include "meen/IController.h"

namespace meen
{
	/** Machine interface

		An abstract representation of a basic machine with a cpu, clock and
		custom memory and IO. Unless specifically stated in this API, there is no
		guarantee in regards to thread safety. All methods should be called
		from the same thread.

		Basic Principles of Operation (error checking has been ommitted for simplicity):

		@code{.cpp}

		// Create a synchronous i8080 machine running as fast as possible 
		auto machine = Make8080Machine();

		// Create a custom memory controller (See tests for examples)
		auto customMemoryController = IControllerPtr(new CustomMemoryController());

		// Load the controller with a program via a custom controller method
		static_cast<CustomMemoryController*>(customMemoryController.get())->LoadProgram("myProgram.com");

		// Create a custom IO Controller and attach it to the machine (See tests for examples)
		machine->AttachIOController(IControllerPtr(new CustomIOController()));

		// Attach the custom memory controller with a loaded program to the machine
		machine->AttachMemoryController(std::move(customMemoryController));

		machine->OnLoad([](char* json, int* jsonLength)
		{
			// Return the json state to load, could be read from disk for example
  			if(json == nullptr)
  			{
    			*jsonLen = JsonToLoadLength;
  			}
  			else
 			{
    			memcpy(json, jsonToLoad, *jsonLength);
  			}
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

			Start executing the instructions that reside in the currently loaded
			memory controller.

			@remark						Before a program has been loaded, current existing memory will continue ti be executed.
										This is relevant when no inital program has been loaded. In this case, current existing
										memory will be executed from address 0x0000. The memory controller's internal memory
										should be initialised to 0x00 before the first call to IMachine::Run is made. Failure
										to initialise the memory to a known state will result in undefined behaviour.

			@return						A std::error_code:

										errc::memory_controller: no memory controller has been attached.
										errc::io_controller: no io controller has been attached.
										errc::busy: meen is running.
										errc::cpu: the machine cpu is invalid.
										errc::clock_resolution: the clock is invalid or the supplied clock resolution is too high.

			@sa							OnLoad
			@sa							meen::errc
		*/
		virtual std::error_code Run() = 0;

		/** Wait for the machine to finish running

			Block the current thread until the machine execution loop has completed.

			@return						A std::expected with an expected value of the duration of the
										run time of the machine as a uint64_t in nanoseconds and an
										unexpected value as a std::error_code.

			@remark						Repeated calls to this method will return the last valid meen run time. 
		*/
		virtual std::expected<uint64_t, std::error_code> WaitForCompletion() = 0;

		/** Attach a custom memory controller

			The machine will use this controller when it needs to read
			and/or write data to ram.

			@param	controller		The memory controller to be used with this machine.

			@return					A std::error_code:

									errc::busy: meen is running.

			@remark					The controller must be stopped, otherwise errc::busy will be returned. This
									will be returned when the runAsync option has been set and Run has been called
									and is not complete. WaitForCompletion must be called to ensure meen is finished.

			@remark					Attaching a controller transfers ownership of the controller to meen.
									The only way to transfer ownership back to the caller is via DetachMemoryController.
									Consecutive different controller attachments will result in the destruction
									of the currently attached controller.

			@sa						See Tests/TestControllers/MemoryController.cpp
		*/
		virtual std::error_code AttachMemoryController (IControllerPtr&& controller) = 0;
		
		/**	Detach a custom memory controller.

		  	Transfer ownership of the controller from meen back to the caller

			@remark					Once this method has been successfully called, calls to methods which require
									a controller to be attched will fail.

			@return					A std::expected with an expected value of a unique_ptr to the detached controller
									and an unexpected value of a std::error_code (errc::busy when meen is in a running state).
		 */
		virtual std::expected<IControllerPtr, std::error_code> DetachMemoryController() = 0;

		/** Attach a custom io controller

			The machine will use this controller when it needs to read
			and/or write data to an io device.

			@param	controller		The io controller to be used with this machine.

			@return					A std::error_code:

									errc::busy: meen is running.

			@remark					The controller must be stopped, otherwise errc::busy will be returned. This
									will be returned when the runAsync option has been set and Run has been called
									and is not complete. WaitForCompletion must be called to ensure meen is finished.

			@remark					Attaching a controller transfers ownership of the controller to meen.
									The only way to transfer ownership back to the caller is via DetachIoController.
									Consecutive different controller attachments will result in the destruction
									of the currently attached controller.

			@sa						See Tests/TestControllers/MemoryController.cpp
		*/
		virtual std::error_code AttachIoController (IControllerPtr&& controller) = 0;
		
		/**	Detach a custom io controller.

		  	Transfer ownership of the controller from meen back to the caller

			@remark					Once this method has been successfully called, calls to methods which require
									a controller to be attched will fail.

			@return					A std::expected with an expected value of a unique_ptr to the detached controller
									and an unexpected value of a std::error_code (errc::busy when meen is in a running state).
		 */
		virtual std::expected<IControllerPtr, std::error_code> DetachIoController() = 0;
		
		/** Set machine options

			See README.txt section 'Configuration options' for a complete list of options and their defaults.

			@param		options		A json string specifying the desired options to update. Passing in an options string of nullptr will set all options
									to their defaults.

			@return					std::error code:<br>

									errc::no_error: all options were set successfully.<br>
									errc::json_config: one of the option values are illegal.<br>
									errc::json_parse: the json input parameter options is malformed.<br>
									errc::busy: meen is running.

			@see					MakeMachine factory methods for supported configuration options.
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
			}

			@endcode

			@param	onSave				The method to call with the json machine save state after it has has been
										generated via the ISR::Save interrupt. Is mutually exclusive with the OnLoad
										initiation handler.

			@return						std::error_code:<br>

										errc::no_error: the state completion handler was set successfully.<br>
										errc::busy: meen is currently running.<br>
										errc::not_implemented: save support has been disabled.

			@remark						The function parameter onSave will be called from a different thread from which this
										method was called if the runAsync or saveAsync config options have been specified.

			@remark						Save requests are not queued. When a save is in progress, additional save interrupts
										will be ignored.
			@since	version 1.5.0
		*/
		virtual std::error_code OnSave(std::function<errc(const char* json)>&& onSave) = 0;

		/** Machine load state initiation handler
		
			Registers a method that will be called when the ISR::Load interrupt is triggered. The register
			method returns a std:error_code and accepts two parameters: json - the char array to write the
			machine state json, jsonLen - the length of the json state array. When the json array is nullptr
			the length of the json state array must be written to jsonLen.

			@param	onLoad				The method to call to get the json machine state to load when the ISR::Load
										interrupt is triggered. Is mutually exclusive with the OnSave completion handler.

			@return						std::error_code:<br>

										errc::no_error: the state completion handler was set successfully.<br>
										errc::busy: meen is currently running.<br>
										errc::not_implemented: load support has been disabled.

			@remark						The function parameter onLoad will be called from a different thread from which this
										method was called if the runAsync or loadAsync config options have been specified.

			@remark						The function parameter onLoad should return errc::invalid argument if it's jsonLen
										parameter is nullptr or if it's jsonLen parameter is too small.

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
		virtual std::error_code OnLoad(std::function<errc(char* json, int* jsonLen)>&& onLoad) = 0;

		/** Destruct the machine

			Release all resources used by this machine instance.
		*/
		virtual ~IMachine() = default;
	};
} // namespace meen

#endif // IMACHINE_H
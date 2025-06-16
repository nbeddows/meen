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
	*/
	struct IMachine
	{
		/** Run the machine

			Start executing the instructions that reside in the currently loaded
			memory controller.

			@remark					Before a program has been loaded, current existing memory will continue to be executed.
								This is relevant when no inital program has been loaded. In this case, current existing
								memory will be executed from address `0x0000`. The memory controller's internal memory
								should be initialised to `0x00` before the first call to IMachine::Run is made. Failure
								to initialise the memory to a known state will result in undefined behaviour.

			@return					A `std::expected` with an expected value of the duration of the
								run time of the machine as a `uint64_t` in nanoseconds and an
								unexpected value of one of the following MEEN std error codes:

			| MEEN error code         | Explanation                                                       |
			|:------------------------|:------------------------------------------------------------------|
			| memory_controller       | No memory controller has been attached                            |
			| io_controller           | No io controller has been attached                                |
			| busy                    | MEEN is running                                                   |
			| cpu                     | The machine cpu is invalid                                        |
			| clock_resolution        | The clock is invalid or the supplied clock resolution is too high |

			@since					version 0.2.0
		*/
		virtual std::expected<uint64_t, std::error_code> Run() = 0;

		/** Attach a custom memory controller

			The machine will use this controller when it needs to read
			and/or write data to ram.

			@param	controller			The memory controller to be used with this machine.

			@remark					Attaching a controller transfers ownership of the controller to MEEN.
								The only way to transfer ownership back to the caller is via
								IMachine::DetachMemoryController. Consecutive different controller
								attachments will result in the destruction of the currently attached controller.

			@return					One of the following MEEN std error codes:

			| MEEN error code         | Explanation                       |
			|:------------------------|:----------------------------------|
			| invalid_argument        | The controller parameter is empty |
			| busy                    | MEEN is currently running         |

			@sa					DetachMemoryController

			@since					version 2.0.0
		*/
		virtual std::error_code AttachMemoryController (IControllerPtr&& controller) = 0;

		/**	Detach a custom memory controller.

		  	Transfer ownership of the controller from MEEN back to the caller.

			@remark					Once this method has been successfully called, calls to methods which require
								a controller to be attched will fail.

			@return					A `std::expected` with an expected value of a `std::unique_ptr` to the detached controller
								and an unexpected value of one of the following MEEN std error codes:

								| MEEN error code         | Explanation                                            |
								|:------------------------|:-------------------------------------------------------|
								| io_controller           | MEEN currently has no memory controller attached to it |
								| busy                    | MEEN is currently running                              |

                        @sa					AttachMemoryController

			@since					version 2.0.0
		*/
		virtual std::expected<IControllerPtr, std::error_code> DetachMemoryController() = 0;

		/** Attach a custom io controller

			The machine will use this controller when it needs to read and/or write data to an io device.

			@param	controller			The io controller to be used with this machine.

			@remark					Attaching a controller transfers ownership of the controller to MEEN.
								The only way to transfer ownership back to the caller is via
								IMachine::DetachIoController. Consecutive different controller attachments
								will result in the destruction of the currently attached controller.

			@return					One of the following MEEN std error codes:

			| MEEN error code         | Explanation                       |
			|:------------------------|:----------------------------------|
			| invalid_argument        | The controller parameter is empty |
			| busy                    | MEEN is currently running         |

			@sa					DetachIoController

			@since					version 2.0.0
		*/
		virtual std::error_code AttachIoController (IControllerPtr&& controller) = 0;

		/**	Detach a custom io controller.

		  	Transfer ownership of the controller from MEEN back to the caller.

			@remark					Once this method has been successfully called, calls to methods which require
								a controller to be attched will fail.

			@return					A `std::expected` with an expected value of a `std::unique_ptr` to the detached controller
								and an unexpected value of one of the following MEEN std error codes:

			| MEEN error code         | Explanation                                        |
			|:------------------------|:---------------------------------------------------|
			| io_controller           | MEEN currently has no io controller attached to it |
			| busy                    | MEEN is currently running                          |

			@sa					AttachIoController

			@since					version 2.0.0
		*/
		virtual std::expected<IControllerPtr, std::error_code> DetachIoController() = 0;

		/** Set machine options

			See the `Configuration Options` section for a complete list of options and their defaults.

			@param		options			A json string specifying the desired options to update. Passing in an options string of nullptr will set all options
								to their defaults.

			@remark					The options parameter should be prefixed by one of the following supported protocols:

			| URI                     | Explanation                                                                                                |
	 		|:------------------------|:-----------------------------------------------------------------------------------------------------------|
			| file://                 | load the configuration options specifed by the json in the file on local disk given by the remaining bytes |
			| json:// (default)       | load the configuration options specified by the json in the remaining bytes                                |

			@remark					When no protocol is specified, `json://` will be used.

			@return					One of the following MEEN std error codes:

			| MEEN error code         | Explanation                                               |
			|:------------------------|:----------------------------------------------------------|
			| no_error                | All options were set successfully                         |
			| json_config             | One the options values are illegal                        |
			| json_parse              | The json pointed to by the options parameter is malformed |
			| busy                    | MEEN is currently running                                 |
		*/
		virtual std::error_code SetOptions(const char* options) = 0;

		/** One time initialisation handler

			Registers a method that will be called exactly once.

			The `onInit` signature:

			| Return Type      | Explanation                                    |
			|:-----------------|------------------------------------------------|
			| meen::errc       | The meen error code as defined in meen/Error.h |

			| Parameter        | Explanation                                                                                  |
			|:-----------------|----------------------------------------------------------------------------------------------|
			| ioController     | A pointer to the io controller that was attached via the IMachine::AttachIoController method |

			@param		onInit		A method that will be called exactly once, right before the beginning of the machine execution
									loop. The handler is guaranteed to be called no earlier than when the IMachine::Run method is
									called successfully for the first time.

			@remark					If your initialisation handler returns a meen::errc value other than `no_error` it is not possible
								for your initialisation handler to be invoked again with this machine instance.

			@remark					When the `runAsync` configuration option is set to true, the initialisation handler will be called
								from the thread that launched the machine execution loop, otherwise, it will be called from the
								main thread.

			@return					One of the following MEEN std error codes:

			| MEEN error code         | Explanation                                      |
	 		|:------------------------|:-------------------------------------------------|
			| no_error                | The on init	 handler was registered successfully |
			| busy                    | MEEN is currently running                        |

			@since					version 2.0.0
		*/
		virtual std::error_code OnInit(std::function<errc(IController* ioController)>&& onInit) = 0;

		/** Machine save state completion handler

			Registers two methods that will be called when the ISR::Save interrupt is triggered:

			The `onSaveBegin` signature:

			| Return Type      | Explanation                                    |
			|:-----------------|:-----------------------------------------------|
			| meen::errc       | The meen error code as defined in meen/Error.h |

			| Parameter        | Explanation                                                                                                               |
			|:-----------------|:--------------------------------------------------------------------------------------------------------------------------|
			| uri              | A `char*` buffer that the method will write the location where the state is to be saved preceeded by a supported protocol |
			| uriLen           | An `int*` containing the size of the uri buffer. The method will write to actual uri length to `uriLen`                   |
			| ioController     | A pointer to the io controller that was attached via the IMachine::AttachIoController method                              |

			NOTE: The only supported protocol is `file://`. When no protocol is specified in the uri, `file://` will be used.

			The `onSave` signature:

			| Return Type      | Explanation                                    |
			|:-----------------|:-----------------------------------------------|
			| meen::errc       | The meen error code as defined in meen/Error.h |

			| Parameter        | Explanation                                                                                  |
			|:-----------------|:---------------------------------------------------------------------------------------------|
			| location         | The uri obtained from the onSaveBegin method                                                 |
			| json             | A `const char*` which points to the machine save state in json format                        |
			| ioController     | A pointer to the io controller that was attached via the IMachine::AttachIoController method |

			The format of the save state is not overly relevant from a user perspective. The json save state is
			defined below:<br>

			@code{.json}

			{
				"cpu":
				{
					"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==",			// The cpu UUID encoded in base64
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
					"uuid":"base64://zRjYZ92/TaqtWroc666wMQ==",			// The memory controller UUID encoded in base64
					"rom":{
						"bytes":"base64://md5://FkgjfhUYrudiMj7y65789Io=",	// The rom md5 hash encoded in base64
					},
					"ram":
					{
						"size":57343,						// The size of the uncompressed ram
						"bytes":"base64://zlib://... the ram bytes ..."		// The zlib compressed and base64 encoded ram bytes
					}
				}
			}

			@endcode

			@param	onSaveBegin		A method used to obtain the uri of the resource that will be used to write the json
									save state to. It is mutually exclusive with the IMachine::OnLoad initiation handler.

			@param	onSave			A optional method that will be called in the following circumstances:
									- when the onSaveBegin method yields a location with an unknown protocol (location and json parameters will be valid).
									- when a supported protocol is found, but it fails to write (location and json parameters will be valid).
									- when the save state is written successfully (location and json parameters will be nullptr).
			
									When no method is registered and an error is encountered, the method registered via IMachine::OnError will be
									called with a meen::errc error code. It is mutually exclusive with the IMachine::OnLoad initiation handler.

			@return					One of the following MEEN std error codes:

			| MEEN error code         | Explanation                                     |
			|:------------------------|:------------------------------------------------|
			| no_error                | The on save handler was registered successfully |
			| busy                    | MEEN is currently running                       |
			| not_implemented         | Save support has been disabled                  |

			@remark						The function parameters `onSaveBegin` and `onSave` will be called from a different thread from which this
									method was called if the `runAsync` and/or `saveAsync` configuration options have been specified.

			@remark						Save requests are not queued. When a save is in progress, additional save interrupts
									will be ignored.

			@since						version 1.5.0
		*/
		virtual std::error_code OnSave(std::function<errc(char* uri, int* uriLen, IController* ioController)>&& onSaveBegin, std::function<errc(const char* location, const char* json, IController* ioController)>&& onSave) = 0;

		/** Machine load state initiation handler

			Registers two methods that will be called when the ISR::Load interrupt is triggered.

			The `onLoad` signature:

			| Return Type      | Explanation                                    |
			|:-----------------|:------------------------------------------------|
			| meen::errc       | The meen error code as defined in meen/Error.h |

			| Parameter        | Explanation                                                                                  |
			|:-----------------|:---------------------------------------------------------------------------------------------|
			| json             | A `char*` buffer to write the machine state json to                                          |
			| jsonLen          | An `int*` to write the length of the json state to                                           |
			| ioController     | A pointer to the io controller that was attached via the IMachine::AttachIoController method |

			The `onLoadComplete` sugnture:

			| Return Type      | Explanation                                    |
			|:-----------------|:-----------------------------------------------|
			| meen::errc       | The meen error code as defined in meen/Error.h |

			| Parameter        | Explanation                                                                                  |
			|:-----------------|:---------------------------------------------------------------------------------------------|
			| ioController     | A pointer to the io controller that was attached via the IMachine::AttachIoController method |

			The first method will be called once with a `char` array of the size specified by the `maxLoadStateLen`
			configuration option (or the default value of 512 if not specified). When the computed json asset
			size (file:// or json:// for example) is larger than the `jsonLen` parameter an errc::invalid_argument
			shall be returned and the user must set the `maxLoadStateLen` configuration option with a larger
			value before invoking this method again. Upon successfully copying out the load state buffer to the
			supplied json array, the `jsonLen` parameter must be set to the actual size of the load state buffer.

			Upon successfully loading the new machine state specified by the json set in the first method the
			second method will be called to indicate that the json load is complete.

			The json layout of an example load state is specifed in the json snippet below:<br>

			@code{.json}

			{
				"cpu":{
					// Start executing the loaded program from address 256 (0x100)
					"pc":256
				},
				"memory": {
					"rom":{
						"block":[
							// Load from local disk all bytes from the file rom0.bin starting at the memory address
							// 0x0000.
							{"bytes":"file://rom0.bin"},
							// Load from local disk the first 128 bytes from the file rom1.bin starting at the memory
							// address 256 (0x100).
							{"bytes":"file://rom1.bin","offset":256,"size":128}
						]
					}
				}
			}

			@endcode

			NOTE: the cpu registers can also be set (see IMachine::OnSave), registers unassigned will remain unchanged.
			NOTE: upon machine (re)start, the program counter (pc) and stack pointer (sp) will always be assigned the
			last valid value set (or 0 if these parameters have never been set).

			The memory:rom:block array can be omitted if loading from a single file:<br>

			@code{.json}

			{
				"memory": {
					"rom":{
						// Load from local disk all bytes from the file rom0.bin starting at the memory address 0x0000.
						{"bytes":"file://rom0.bin"}
					}
				}
			}

			@endcode

			See the unit tests for further examples.

			NOTE: a uri scheme like method is used to determine the content type of the bytes property.

			Supported protocols:

			| URI                     | Explanation                                                                                     |
	 		|:------------------------|:------------------------------------------------------------------------------------------------|
			| file:// 		  | load a resource located at the path specifed by the remainder of the bytes property into memory |
			| base64://               | base64 decode the remaining bytes and load them into memory.                                    |
			| base64://zlib://	  | Decompress the remaining bytes, base64 decode them and load them into memory                    |

			The size and offset parameters of memory:rom:[block] are optional and will default to 0 for the offset and the length of the remaining bytes in the bytes parameter for size.
			NOTE: the size parameter is mandatory for "zlib://" and MUST be the length of the uncompressed bytes.

			@param	onLoad						The method to call to get the json machine state to load when the ISR::Load
										interrupt is triggered. Is mutually exclusive with the IMachine::OnSave completion handler.
			@param	onLoadComplete					An optional method to call when the machine state returned by the onLoad handler has been
										SUCESSFULLY loaded.

			@return							One of the following MEEN std error codes:

			| MEEN error code         | Explanation                                     |
			|:------------------------|:------------------------------------------------|
			| no_error                | The on load handler was registered successfully |
			| busy                    | MEEN is currently running                       |
			| not_implemented         | Load support has been disabled                  |

			@remark							The function parameter onLoad will be called from a different thread from which this
										method was called if the `runAsync` or `loadAsync` configuration options have been specified.

			@remark							The function parameter `onLoadComplete` will be called from a different thread from which this
										method was called if the `runAsync` configuration option has been specified.

			@remark							The function parameter `onLoad` should return errc::invalid_argument if it's `jsonLen`
										parameter is too small to hold the generated buffer.

			@remark							The machine state can fail to load for numerous reasons:
											- when the machine cpu does not match the load state cpu.
											- when the machine memory controller does not match the load state memory controller.
											- when the machine memory rom does not match the load state rom.
											- invalid json file.

			@remark							Therefore, it is recommended that IMachine::OnError be called to register an on error handler
										in order to catch any of the above stated possible failures.

			@remark							When the format of the returned json string is invalid or a load error occurs the state
										of the machine is undefined and should be re-loaded.

			@remark							Load requests are not queued. When a load is in progress, additional load interrupts
										will be ignored.

			@since							version 1.5.0
		*/
		virtual std::error_code OnLoad(std::function<errc(char* json, int* jsonLen, IController* ioController)>&& onLoad, std::function<errc(IController* ioController)>&& onLoadComplete) = 0;

		/** Machine on idle handler

			Registers a handler that will be called periodically.

			The `onIdle` signature:

			| Return Type      | Value | Explanation                              |
			|:-----------------|:------|:-----------------------------------------|
			| bool             | True  | Quit the running machine instance        |
            | ^                | False | Call the registered on idle method again |

			| Parameter        | Explanation                                                                                  |
			|:-----------------|:---------------------------------------------------------------------------------------------|
			| ioController     | A pointer to the io controller that was attached via the IMachine::AttachIoController method |

			@param		onIdle			The on idle handler to register.

			@remark						The registered handler will always be called from the same thread from which IMachine::Run was invoked.
			
			@remark						When the configuration option `runAsync` is true, any data that is shared between this handler and the
										IMachine::OnLoad and IMachine::OnSave handlers needs to be protected as they will be invoked from a separate
										thread to this handler.
										
			@remark						When the configuration option `runAsync` is false, this method should be lightweight
										and non-blocking as superfluous delays here can have an impact on overall performace.

			@return						One of the following MEEN std error codes:

			| MEEN error code         | Explanation                                     |
			|:------------------------|:------------------------------------------------|
			| no_error                | The on idle handler was registered successfully |
			| busy                    | MEEN is currently running                       |

			@since							version 2.0.0
		*/
		virtual std::error_code OnIdle(std::function<bool(IController* ioController)>&& onIdle) = 0;

		/** Machine on error handler

			Registers a handler that allows for the custom processing of errors generated by MEEN.

			The `onError` signature:

			| Return Type      | Explanation                                    |
			|:-----------------|------------------------------------------------|
			| void             | The method does not return anything            |

			| Parameter        | Explanation                                                                                   |
			|:-----------------|:----------------------------------------------------------------------------------------------|
			| ec               | The MEEN std::error_code whose value method returns one of the errors defined in meen/Error.h |
			| fileName         | A `const char*` to the source file that generated the call to this method                     |
			| functionName     | The name of the function that generated the call to this method                               |
			| line             | The line number from the source file that generated the call to this method                   |
			| column           | The line number column from the source file that generated the call to this method            |
			| ioController     | A pointer to the io controller that was attached via the IMachine::AttachIoController method  |

			@param		onError			The on error handler to register.

			@return						One of the following MEEN std::error_codes:

			| MEEN error code         | Explanation                                      |
			|:------------------------|:-------------------------------------------------|
			| no_error                | The on error handler was registered successfully |
			| busy                    | MEEN is currently running                        |

			@remark							Care needs to be taken when handling the errors generated by this handler as the
										handler may or may not be called from a different thread to which you are on if
										the `runasync`, `loadAsync` or `saveAsync` options have been set.

			@since							version 2.0.0
		*/
		virtual std::error_code OnError(std::function<void(std::error_code ec, const char* fileName, const char* functionName, uint32_t line, uint32_t column, IController* ioController)>&& onError) = 0;

		/** Destruct the machine

			Release all resources used by this machine instance.
		*/
		virtual ~IMachine() = default;
	};
} // namespace meen

#endif // IMACHINE_H
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

#ifndef MACHINE_FACTORY_H
#define MACHINE_FACTORY_H

#include <memory>
#include "meen/IMachine.h"

#ifdef _WINDOWS
#if meem_STATIC
#define DLL_EXP_IMP
#elif defined meen_EXPORTS
#define DLL_EXP_IMP __declspec(dllexport)
#else
#define DLL_EXP_IMP __declspec(dllimport)
#endif
#else
#ifdef meen_EXPORTS
#define DLL_EXP_IMP [[gnu::visibility("default")]]
#else
#define DLL_EXP_IMP
#endif
#endif

namespace meen
{
	/**
		The meen library version

		@return A string containing the current version, in a format
				described by [semantic versioning](https://semver.org/)

				@code

				<major>"."<minor>"."<patch>
				<major>"."<minor>"."<patch>"-"<pre-release>
				<major>"."<minor>"."<patch>"+"<build>
				<major>"."<minor>"."<patch>"-"<pre-release>"+"<build>
				
				@endcode

		@remark Available since 1.4.0.

		@remark	Version 1.3.0 is implied when this method is not available.
	*/
	DLL_EXP_IMP const char* Version();

	/** Create a Machine

		Build a machine based on the supplied configuration.

		@param		config	a json configuration string or the location of the json configuration file. When the config
							string starts with a recognised protocol, the json will be loaded via that protocol, otherwise
							it will be treated as raw json, for example R"({"option":"value"})".

							Supported Protocols:

							| Protocol | Remarks                                   |
							|:---------|:------------------------------------------|
							| file://  | Load a json file from local disk storage  |

							Configuration options:

							| Option          | Type   | Value	            | Remarks                                                                            |
							|:----------------|:-------|:-------------------|:-----------------------------------------------------------------------------------|
							| clockResolution | int64  | -1 (default)       | Run the machine as fast as possible with the highest possible resolution           |
							|                 |        | 0                  | Run the machine at realtime (or as close to) with the highest possible resolution  |
							|                 |        | 0 - 1000000        | Will always spin the cpu to maintain the clock speed and is not recommended        |
							|                 |        | n                  | A request in nanoseconds as to how frequently the machine clock will tick          |
							| compressor      | string | "zlib" (default)   | Use zlib compression library to compress the ram when saving its state             |
							|                 |        | "none"             | No compression will be used when saving the state of the ram                       |
							| encoder         | string | "base64" (default) | The binary to text encoder to use when saving the machine state ram to json        |
							| cpu             | string | "i8080" (default)  | A machine based on the Intel8080 cpu (can only be set via meen::MakeMachine)    |
							| isrFreq         | double | 0 (default)        | Service interrupts at the completion of each instruction                           |
							|                 |        | 1                  | Service interrupts after each clock tick                                           |
							|                 |        | n                  | Service interrupts frequency, example: 0.5 - twice per clock tick                  |
							| loadAsync       | bool   | true               | Run the load initiation handler on a separate thread                               |
							|                 |        | false (default)    | Run the load initiation handler from the thread specified by the `runAsync` option |
							| ramOffset       | uint16 | n (default: 0)     | The offset in bytes from the start of the memory to the start of the ram           |
							| ramSize         | uint16 | n (default: 0)     | The size of the ram in bytes                                                       |
							| romOffset		  | uint16 | n (default: 0)     | The offset in bytes from the start of the memory to the start of the rom           |
							| romSize         | uint16 | n (default: 0)     | The size of the rom in bytes                                                       |
							| runAsync        | bool   | true               | `IMachine::Run` will launch its execution loop on a separate thread                |
							|                 |        | false (default)    | `IMachine::Run` will run its execution loop on the current thread                  |
							| saveAsync       | bool   | true               | Run the save completion handler on a separate thread                               |
							|                 |        | false (default)    | Run the save completion handler from the thread specifed by the `runAsync` option  |


		@throws		std::runtime_error or any exception that the underlying json parser can throw.

		@throws		std::invalid_argument if any of the configuration options are invalid (negative isrFreq, unsupported cpu or clock resolution).
		
		@return		A unique machine pointer that can be loaded with memory and io controllers.
	*/
	DLL_EXP_IMP std::unique_ptr<IMachine> Make8080Machine();
} // namespace meen

#endif // MACHINE_FACTORY_H
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

	/** Create a machine with an i8080 cpu

		Build an i8080 machine based on the default configuration. See README.txt section 'Configuration options'
		for a complete list of options and their defaults.
		
		@return		A unique machine pointer that can be loaded with memory and io controllers.

		@remark		When this factory method fails a valid object will be still returned, however, API calls on the returned object will fail.
	*/
	DLL_EXP_IMP std::unique_ptr<IMachine> Make8080Machine();
} // namespace meen

#endif // MACHINE_FACTORY_H
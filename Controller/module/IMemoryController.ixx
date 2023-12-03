/*
Copyright (c) 2021-2023 Nicolas Beddows <nicolas.beddows@gmail.com>

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

export module IMemoryController;

import <chrono>;
import <cstdint>;
import <filesystem>;
import Base;
import IController;

namespace Emulator
{ 
    /** Memory Controller interface.
	
		An interface to a generic memory device.
	*/
	export struct IMemoryController : public IController
	{
		/** Load ROM.
		
			Load a ROM file into memory at the specified address.

			@param	romFile				The binary file containing cpu instructions to be
										loaded into memory.
			@param	address				The memory address at which the romFile is to be
										loaded at.

			@throw	runtime_error		The rom file failed to open, for example, the path does
										not exist.
			@throw	length_error		The rom file size is too big to fit at the specified
										memory offet.
			@throw	invalid_argument	The rom file exists, but it failed to load.
			
			@todo	add other load variant interfaces, like from vector, array, etc
		*/
		virtual void Load(std::filesystem::path romFile, uint16_t address) = 0;
		
		/**	Memory size.
		
			Obtains the size of the allocated memory used by this memory device.

			@return	size_t		The size of memory allocated by this device.
		*/
		virtual size_t Size() const = 0;

		/** Destructor.

			Release all resources used by this memory controller instance.
		*/
		virtual ~IMemoryController() = default;
	};
} // namespace Emulator
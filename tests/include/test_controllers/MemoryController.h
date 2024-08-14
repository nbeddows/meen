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

#ifndef MEMORYCONTROLLER_H
#define MEMORYCONTROLLER_H

#include <array>
#include <vector>

#include "meen/IController.h"

namespace MachEmu
{
	/**
		Basic example memory controller

		This minimal example reads and writes to a vector of bytes
		on disk one byte at a time. The maximum addressable size
		is 16 bits.
	*/
	class MemoryController final : public IController
	{
	private:
		/**
			The size of the memory in bytes

			This example is a 16 bit memory controller.
		*/
		size_t memorySize_{ 1 << 16 };
		
		/**
			Machine memory

			All the memory that the machine will have access to.
		*/
		std::vector<uint8_t> memory_;
	public:
		/**
			Memory controller constructor
		*/
		MemoryController();

		/**
			Load a program

			Loads a program into memory at a specified offset from the
			starting memory address 0x0000.

			@param	romFilePath				The (absolute or relative) address on local disk
											where the program resides.

			@param	offset					The memory location to load the program into.

			@throw	std::runtime_error		The rom file failed to open.
			@throw	std::length_error		The rom file is too large for the given offset.
			@throw	std::invalid_argument	Failed to read the rom file into memory.
		*/
		int Load(const char* romFilePath, uint16_t offset);
		
		/** Memory clear
		
			Wipes all memory bytes to 0.
		*/
		void Clear();

		/** Memory size
		
			@return					The total size of the memory in bytes.
		*/
		size_t Size() const;

		/**	Uuid

			Unique universal identifier for this controller.

			@return					The uuid as a 16 byte array.
		*/
		std::array<uint8_t, 16> Uuid() const final;

		/** Read a byte of memory

			The maximum size of each read is 8 bits from a 16 address.

			@param		address		The 16 bit address to read from.

			@return					The 8 bits residing at the 16 bit memory address.
		*/
		uint8_t Read(uint16_t address) final;
		
		/** Write a byte of data to memory
		
			The maximum size of each write is 8 bits to a 16 address.
		
			@param		address		The 16 bit address to write to.
			
			@param		value		The 8 bit value to write.
		*/
		void Write(uint16_t address, uint8_t value) final;

		/** Memory IO interrupt handler
		 
			Checks the memory controller to see if any interrupts are pending.
		
			@param	currTime	The time in nanoseconds of the machine clock.

			@param	cycles		The total number of cycles that have elapsed.
		
			@return				ISR::NoInterrupt.

			@remark				This controller never generates any interrupts.
		*/
		ISR ServiceInterrupts(uint64_t currTime, uint64_t cycles) final;
	};
} // namespace MachEmu

#endif // MEMORYCONTROLLER_H
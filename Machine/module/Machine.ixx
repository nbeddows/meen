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

module;

#include <future>

#include "Controller/IController.h"
#include "Machine/IMachine.h"
#include "Opt/Opt.h"

export module Machine;

import <cstdint>;
import <memory>;
import ICpu;
import ICpuClock;
import SystemBus;

namespace MachEmu
{
	/** Machine
		
		@see IMachine.h
	*/
	export struct Machine final : public IMachine
	{
	private:
		std::unique_ptr<ICpuClock> clock_;
		std::unique_ptr<ICpu> cpu_;
		std::shared_ptr<IController> memoryController_;
		std::shared_ptr<IController> ioController_;
		SystemBus<uint16_t, uint8_t, 8> systemBus_;
		Opt opt_;
		int64_t ticksPerIsr_{};
		std::future<int64_t> fut_;
		bool running_{};

		void ProcessControllers(const SystemBus<uint16_t, uint8_t, 8>&& systemBus);
	public:
		Machine(const char* json);
		~Machine() = default;

		/** Run
		
			@see IMachine::Run
		*/
		uint64_t Run(uint16_t pc) final;

		/** WaitForCompletion

			@see IMachine::WaitForCompletion
		*/
		uint64_t WaitForCompletion() final;

		/** SetMemoryController

			@see IMachine::SetMemoryController
		*/
		void SetMemoryController(const std::shared_ptr<IController>& controller) final;

		/** SetIoController

			@see IMachine::SetIoController
		*/
		void SetIoController(const std::shared_ptr<IController>& controller) final;

		/** SetOpts

			@see IMachine::SetOpts
		*/
		ErrorCode SetOptions(const char* options);

		/** Set the clock resolution.
		
			@see IMachine::SetClockResolution
		*/
		ErrorCode SetClockResolution(int64_t clockResolution) final;

		/** GetCpuState
		
			@see IMachine::GetCpuState
		*/
		std::unique_ptr<uint8_t[]> GetState(int* size) const final;
	};
} // namespace MachEmu
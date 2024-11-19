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

#ifndef MACHINE_H
#define MACHINE_H

#ifdef EMABLE_MEEN_RP2040

#else
#include <future>
#endif

#include "meen/IController.h"
#include "meen/cpu/ICpu.h"
#include "meen/clock/ICpuClock.h"
#include "meen/IMachine.h"
#include "meen/opt/Opt.h"

namespace meen
{
	/** Machine

		@see IMachine.h
	*/
	struct Machine final : public IMachine
	{
	private:
		std::unique_ptr<ICpuClock> clock_;
		std::unique_ptr<ICpu> cpu_;
		std::shared_ptr<IController> ioController_;
		std::shared_ptr<IController> memoryController_;
		Opt opt_;
		//cppcheck-suppress unusedStructMember
		int64_t ticksPerIsr_{};
		uint64_t runTime_{};
#ifdef ENABLE_MEEN_RP2040

#else
		std::future<void> fut_;
#endif
		//cppcheck-suppress unusedStructMember
		bool running_{};
#ifdef ENABLE_MEEN_SAVE
		std::function<const char*()> onLoad_{};
		std::function<void(const char* json)> onSave_{};
#endif // ENABLE_MEEN_SAVE
		friend void RunMachine(Machine* machine);
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
		std::error_code SetMemoryController(const std::shared_ptr<IController>& controller) final;

		/** SetIoController

			@see IMachine::SetIoController
		*/
		std::error_code SetIoController(const std::shared_ptr<IController>& controller) final;

		/** SetOptions

			@see IMachine::SetOpts
		*/
		std::error_code SetOptions(const char* options) final;

		/** OnLoad

			@see IMachine::OnLoad
		*/
		std::error_code OnLoad(std::function<const char*()>&& onLoad) final;

		/** OnSave

			@see IMachine::OnSave
		*/
		std::error_code OnSave(std::function<void(const char* json)>&& onSave) final;

		/** Get the machine state

			@see IMachine::GetState
		*/
		std::string Save() const final;

		/** Set the clock resolution.

			@see IMachine::SetClockResolution
		*/
		ErrorCode SetClockResolution(int64_t clockResolution) final;

		/** GetCpuState

			@see IMachine::GetCpuState
		*/
		std::unique_ptr<uint8_t[]> GetState(int* size) const final;
	};
} // namespace meen

#endif // MACHINE_H
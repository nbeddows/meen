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
	enum class Cpu
	{
		i8080
	};
	
	/** Machine

		@see IMachine.h
	*/
	struct Machine final : public IMachine
	{
	private:
		std::unique_ptr<ICpuClock> clock_;
		std::unique_ptr<ICpu> cpu_;
		IControllerPtr ioController_;
		IControllerPtr memoryController_;
		Opt opt_;
		//cppcheck-suppress unusedStructMember
		int64_t ticksPerIsr_{};
		uint64_t runTime_{};
#ifdef ENABLE_MEEN_RP2040

#else
		std::future<void> fut_;
#endif
		std::map<uint16_t, uint16_t> romMetadata_;
		std::map<uint16_t, uint16_t> ramMetadata_;
		//cppcheck-suppress unusedStructMember
		bool running_{};
#ifdef ENABLE_MEEN_SAVE
		std::function<std::error_code(char* json, int* jsonLen)> onLoad_{};
		std::function<std::error_code(const char* json)> onSave_{};
#endif // ENABLE_MEEN_SAVE
		friend void RunMachine(Machine* machine);
	public:
		Machine(Cpu cpu);
		~Machine() = default;

		/** Run

			@see IMachine::Run
		*/
		std::error_code Run() final;

		/** WaitForCompletion

			@see IMachine::WaitForCompletion
		*/
		std::expected<uint64_t, std::error_code> WaitForCompletion() final;

		/** AttachMemoryController

			@see IMachine::AttachMemoryController
		*/
		std::error_code AttachMemoryController (IControllerPtr&& controller) final;
		
		/** DetachMemoryController

			@see IMachine::DetachMemoryController
		*/
		std::expected<IControllerPtr, std::error_code> DetachMemoryController() final;

		/** AttachIoController

			@see IMachine::AttachIoController
		*/
		std::error_code AttachIoController (IControllerPtr&& controller) final;

		/** DetachIoController

			@see IMachine::DetachIoController
		*/
		std::expected<IControllerPtr, std::error_code> DetachIoController () final;

		/** SetOptions

			@see IMachine::SetOpts
		*/
		std::error_code SetOptions(const char* options) final;

		/** OnLoad

			@see IMachine::OnLoad
		*/
		std::error_code OnLoad(std::function<errc(char* json, int* jsonLen)>&& onLoad) final;

		/** OnSave

			@see IMachine::OnSave
		*/
		std::error_code OnSave(std::function<errc(const char* json)>&& onSave) final;
	};
} // namespace meen

#endif // MACHINE_H
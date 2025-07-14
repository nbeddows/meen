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

#include <atomic>
#ifdef PICO_BOARD

#else
#include <future>
#endif
#include <map>
#include <source_location>

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
		std::atomic_bool quit_{};
#ifdef PICO_BOARD

#else
		std::future<void> fut_;
#endif // PICO_BOARD
		std::map<uint16_t, uint16_t> romMetadata_;
		std::map<uint16_t, uint16_t> ramMetadata_;
		//cppcheck-suppress unusedStructMember
		bool running_{};
		std::function<void(std::error_code ec, const char* fileName, const char* functionName, uint32_t line, uint32_t column, IController* ioController)> onError_;
		std::function<bool(IController* ioController)> onIdle_;
		std::function<errc(IController* ioController)> onInit_;
		std::function<errc(char* json, int* jsonLen, IController* ioController)> onLoad_;
		std::function<errc(IController* ioController)> onLoadComplete_;
#ifdef ENABLE_MEEN_SAVE
		std::function<errc(char* uri, int* uriLen, IController* ioController)> onSaveBegin_;
		std::function<errc(const char* location, const char* json, IController* ioController)> onSave_;
#endif // ENABLE_MEEN_SAVE
		/** One time initialisation handler call flag
		
			This is used for std::call_once to invoke the intialisation handler exactly once.
		*/
		std::once_flag initOnceFlag_;
		std::error_code HandleError(std::error_code err, std::source_location&& sl);
		std::error_code HandleError(errc ec, std::source_location&& sl);
		friend void RunMachine(Machine* machine);
	public:
		Machine(Cpu cpu);
		~Machine() = default;

		/** Run

			@see IMachine::Run
		*/
		std::expected<uint64_t, std::error_code> Run() final;

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
		std::error_code OnLoad(std::function<errc(char* json, int* jsonLen, IController* ioController)>&& onLoad, std::function<errc(IController* ioController)>&& onLoadComplete) final;

		/** OnSave

			@see IMachine::OnSave
		*/
		std::error_code OnSave(std::function<errc(char* uri, int* uriLen, IController* ioController)>&& onSaveBegin, std::function<errc(const char* location, const char* json, IController* ioController)>&& onSave) final;

		/** OnIdle

			@see IMachine::OnIdle
		*/
		std::error_code OnIdle(std::function<bool(IController* ioController)>&& onIdle) final;

		/** OnInit

			@see IMachine::OnInit
		*/
		std::error_code OnInit(std::function<errc(IController* ioController)>&& onInit) final;

		/** OnError

			@see IMachine::OnError
		*/
		std::error_code OnError(std::function<void(std::error_code ec, const char* fileName, const char* functionName, uint32_t line, uint32_t column, IController* ioController)>&& onError) final;
	};
} // namespace meen

#endif // MACHINE_H
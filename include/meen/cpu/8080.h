/*
Copyright (c) 2021-2026 Nicolas Beddows <nicolas.beddows@gmail.com>

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

#ifndef _8080_H
#define _8080_H

#include <array>
#include <bitset>
#include <cstdint>
#include <memory>
#include <functional>
#include <string_view>

#include "meen/cpu/ICpu.h"
#include "meen/IController.h"

#define ENABLE_OPCODE_TABLE

namespace meen
{
	class Intel8080 final : public ICpu
	{
	private:
		using Register = std::bitset<8>;
		static constexpr uint8_t maxRegisters_ = 8;
		//cppcheck-suppress unusedStructMember
		static constexpr const char registerName_[maxRegisters_] = {'B', 'C', 'D', 'E', 'H', 'L', 'M', 'A'};
		//cppcheck-suppress unusedStructMember
		static constexpr bool dbg = false;
		//cppcheck-suppress unusedStructMember
		static constexpr std::array<uint8_t, 16> uuid_{ 0x3B, 0xE8, 0x4F, 0x1F, 0x9D, 0x7A, 0x4B, 0x70, 0xA5, 0x45, 0xD9, 0xF3, 0x49, 0x12, 0xFC, 0xAD };

		enum /*class*/Condition
		{
			CarryFlag = 0x00,
			ParityFlag = 0x02,
			AuxCarryFlag = 0x04,
			ZeroFlag = 0x06,
			SignFlag = 0x07
		};

		/**
			The 8080 provides the programmer with an 8-bit accumulator and six additional 8-bit "scratchpad" registers.
			These seven working registers are numbered and referenced via the integers 0, 1,2,3,4,5, and 7; by convention,
			these registers may also be accessed via the letters B, C, D,
			E, H, L, and A (for the accumulator), respectively.
		*/
		//cppcheck-suppress unusedStructMember
		Register a_;
		//cppcheck-suppress unusedStructMember
		Register b_;
		//cppcheck-suppress unusedStructMember
		Register c_;
		//cppcheck-suppress unusedStructMember
		Register d_;
		//cppcheck-suppress unusedStructMember
		Register e_;
		//cppcheck-suppress unusedStructMember
		Register h_;
		//cppcheck-suppress unusedStructMember
		Register l_;

		/**
			The program counter is a 16 bit register which is accessible to the programmer and whose contents indicate the
			address of the next instruction to be executed.
		*/
		//cppcheck-suppress unusedStructMember
		uint16_t pc_{};

		/**
			A stack is an area of memory set aside by the programmer in which data or addresses are stored and retrieved
			by stack operations. The programmer specifies which addresses the stack operations will
			operate upon via a special accessible 16-bit register called the stack pointer.
		*/
		//cppcheck-suppress unusedStructMember
		uint16_t sp_{};

		/**
			Five condition (or status) bits are provided by the
			8080 to reflect the results of data operations

			S Z 0 AC 0 P 1 C
		*/
		Register status_ = 0b00000010;

		//interrupt flip-flip - 1 enabled, 0 disabled
		//cppcheck-suppress unusedStructMember
		bool iff_{};
		// cpu halt - 1 enabled, 0 disabled
		//cppcheck-suppress unusedStructMember
		bool hlt_{};

		//The opcode for the instruction to be executed.
		//cppcheck-suppress unusedStructMember
		uint8_t opcode_{};
#ifdef ENABLE_OPCODE_TABLE 
		std::unique_ptr<std::function <int8_t()>[]> opcodeTable_;
#endif
		IController* memoryController_{};
		IController* ioController_{};

		static uint8_t Value(const Register& r) { return static_cast<uint8_t>(r.to_ulong()); }
		static uint16_t Uint16(const Register& hi, const Register& low) { return (Value(hi) << 8) | Value(low); }
		static bool Parity(const Register& r) { return (r.count() & 1) == 0; }
		static bool Sign(const Register& r) { return r.test(7); }
		static bool Zero(const Register& r) { return r.none(); }

		//Should be implicitly inline
		inline int8_t Inr(Register& r);
		inline int8_t Inr();
		inline int8_t Dcr(Register& r);
		inline int8_t Dcr(uint16_t addr);
		inline int8_t Mvi(Register& reg);
		inline int8_t Mvi();
		inline int8_t Daa();
		inline int8_t Rlc();
		inline int8_t Rrc();
		inline int8_t Ral();
		inline int8_t Rar();
		inline int8_t Lxi(Register& regHi, Register& regLow);
		inline int8_t Lxi();
		inline int8_t Shld();
		inline int8_t Stax(const Register& hi, const Register& low);
		inline int8_t Inx(Register& hi, Register& low);
		inline int8_t Inx();
		inline int8_t Dad(const Register& hi, const Register& low);
		inline int8_t Dad();
		inline int8_t Lhld();
		inline int8_t Ldax(const Register& hi, const Register& low);
		inline int8_t Dcx(Register& hi, Register& low);
		inline int8_t Dcx();
		inline int8_t Cma();
		inline int8_t Sta();
		inline int8_t Stc();
		inline int8_t Lda();
		inline int8_t Cmc();
		inline int8_t Mov(Register& lhs, const Register& rhs);
		inline int8_t Mov(Register& lhs);
		inline int8_t Mov(uint8_t value);
		inline int8_t Nop();
		inline int8_t Hlt();
		inline Register Add(const Register& lhs, const Register& rhs, uint8_t carry, bool setCarryFlag, [[maybe_unused]] std::string_view instructionName);
		inline int8_t Add(const Register& r, std::string_view instructionName);
		inline int8_t Add(uint16_t addr, std::string_view instructionName);
		inline int8_t Adc(const Register& r, std::string_view instructionName);
		inline int8_t Adc(uint16_t addr, std::string_view instructionName);
		inline Register Sub(const Register& r, uint8_t withCarry, std::string_view instructionName);
		inline int8_t Sub(const Register& r, std::string_view instructionName);
		inline int8_t Sub(uint16_t addr, std::string_view instructionName);
		inline int8_t Sbb(const Register& r, std::string_view instructionName);
		inline int8_t Sbb(uint16_t addr, std::string_view instructionName);
		inline void Ana(const Register& r);
		inline int8_t Ana(const Register& r, std::string_view instructionName);
		inline int8_t Ana(uint16_t addr, std::string_view instructionName);
		inline void Xra(const Register& r);
		inline int8_t Xra(const Register& r, std::string_view instructionName);
		inline int8_t Xra(uint16_t addr, std::string_view instructionName);
		inline void Ora(const Register& r);
		inline int8_t Ora(const Register& r, std::string_view instructionName);
		inline int8_t Ora(uint16_t addr, std::string_view instructionName);
		inline int8_t Cmp(const Register& r, std::string_view instructionName);
		inline int8_t Cmp(uint16_t addr, std::string_view instructionName);
		inline int8_t NotImplemented();
		inline int8_t RetOnFlag(bool status, std::string_view instructionName);
		inline int8_t Pop(Register& hi, Register& low);
		inline int8_t JmpOnFlag(bool status, std::string_view instructionName);
		inline int8_t CallOnFlag(bool status, std::string_view instructionName);
		inline int8_t Push(const Register& hi, const Register& low);
		inline int8_t Adi(const Register& r);
		inline int8_t Rst();
		inline int8_t Rst(uint8_t restart);
		inline int8_t Out();
		inline int8_t In();
		inline int8_t Xthl();
		inline int8_t Pchl();
		inline int8_t Xchg();
		inline int8_t Di();
		inline int8_t Sphl();
		inline int8_t Ei();

	public:
		/* I8080 overrides */
		int8_t Execute() final;
		int8_t Interrupt(ISR isr);
		std::error_code Load(const std::string&& json, bool checkUuid) final;
#ifdef ENABLE_MEEN_SAVE
		std::expected<std::string, std::error_code> Save() const final;
#endif // ENABLE_MEEN_SAVE
		void Reset() final;
		void SetMemoryController(IController* memoryController) final;
		void SetIoController(IController* ioController) final;
		/* End I8080 overrides */

		Intel8080();
		~Intel8080() = default;
	};
} // namespace meen

#endif // _8080_H
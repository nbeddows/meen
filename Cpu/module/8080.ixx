module;
#include <coroutine>

export module _8080;

import <array>;
import <bitset>;
import <functional>;
import <string_view>;
import Base;
import CoRoutineHelper;
import I8080;
import SystemBus;

namespace Emulator
{
	constexpr bool dbg = false;

	export class Intel8080 final : public I8080 //replace with public ICpu
	{
	private:
		using Register = std::bitset<8>;
		static constexpr uint8_t maxRegisters_ = 8;
		static constexpr char registerName_[maxRegisters_] = { 'B', 'C', 'D', 'E', 'H', 'L', 'M', 'A' };

		enum /*class*/Condition
		{
			CarryFlag = 0x00,
			ParityFlag = 0x02,
			AuxCarryFlag = 0x04,
			ZeroFlag = 0x06,
			SignFlag = 0x07
		};

		/**
			The 8080 provides the programmer with an 8-bit accumulator and six additional 8-bit "scratchpad" registers.
			These seven working registers are numbered and referenced via the integers 0, 1,2,3,4,5, and 7; by convention,
			these registers may also be accessed via the letters B, C, D,
			E, H, L, and A (for the accumulator), respectively.
		*/
		Register a_;
		Register b_;
		Register c_;
		Register d_;
		Register e_;
		Register h_;
		Register l_;

		//The currently executing co routine.
		CoObj coObj_;

		//Helper funtions to prime the address and data buses
		//for read write operations.
		void ReadFromAddress(Signal readLocation, uint16_t addr);
		void WriteToAddress(Signal writeLocation, uint16_t addr, uint8_t value);

		/**
			The program counter is a 16 bit register which is accessible to the programmer and whose contents indicate the
			address of the next instruction to be executed.
		*/
		uint16_t pc_{};

		/**
			A stack is an area of memory set aside by the programmer in which data or addresses are stored and retrieved
			by stack operations. The programmer specifies which addresses the stack operations will
			operate upon via a special accessible 16-bit register called the stack pointer.
		*/
		uint16_t sp_{};

		/**
			Five condition (or status) bits are provided by the
			8080 to reflect the results of data operations

			S Z 0 AC 0 P 1 C
		*/
		Register status_ = 0b00000010;

		//interrupt flip-flip - 1 enabled, 0 disabled
		bool iff_{};

		//The number of time periods that the current
		//instruction requires to execute.
		int8_t timePeriods_{};

		//Start a new co routine if we are done, otherwise resume the incomplete co routine.
		bool coDone_{ true };

		//The opcode for the instruction to be executed.
		uint8_t opcode_{};

		//Initialise to an invalid value.
		ISR isr_{ISR::NoInterrupt};

		std::unique_ptr<std::function <CoObj()>[]> opcodeTable_;

		std::shared_ptr<AddressBus<uint16_t>> addressBus_;
		std::shared_ptr<DataBus<uint8_t>> dataBus_;
		std::shared_ptr<ControlBus<8>> controlBus_;

		static uint8_t Value(const Register& r) { return static_cast<uint8_t>(r.to_ulong()); }
		static uint16_t Uint16(const Register& hi, const Register& low) { return (Value(hi) << 8) | Value(low); }
		static bool AuxCarry(const Register& r, uint8_t value) { return ((Value(r) & 0x0F) + (value & 0x0F)) > 0x0F; }
		static bool Carry(const Register& r, uint8_t value) { return (Value(r) + value) > 0xFF; }
		static bool Parity(const Register& r) { return (r.count() & 1) == 0; }
		static bool Sign(const Register& r) { return r.test(7); }
		static bool Zero(const Register& r) { return r.none(); }

		//Should be implicitly inline
		inline CoObj Inr(Register& r);
		inline CoObj Inr();
		inline CoObj Dcr(Register& r);
		inline CoObj Dcr(uint16_t addr);
		inline CoObj Mvi(Register& reg);
		inline CoObj Mvi();
		inline CoObj Daa();
		inline CoObj Rlc();
		inline CoObj Rrc();
		inline CoObj Ral();
		inline CoObj Rar();
		inline CoObj Lxi(Register& regHi, Register& regLow);
		inline CoObj Lxi();
		inline CoObj Shld();
		inline CoObj Stax(const Register& hi, const Register& low);
		inline CoObj Inx(Register& hi, Register& low);
		inline CoObj Inx();
		inline CoObj Dad(const Register& hi, const Register& low);
		inline CoObj Dad();
		inline CoObj Lhld();
		inline CoObj Ldax(const Register& hi, const Register& low);
		inline CoObj Dcx(Register& hi, Register& low);
		inline CoObj Dcx();
		inline CoObj Cma();
		inline CoObj Sta();
		inline CoObj Stc();
		inline CoObj Lda();
		inline CoObj Cmc();
		inline CoObj Mov(Register& lhs, const Register& rhs);
		inline CoObj Mov(Register& lhs);
		inline CoObj Mov(uint8_t value);
		inline CoObj Nop();
		inline CoObj Hlt();
		inline Register Add(const Register& lhs, const Register& rhs, bool setCarryFlag, uint8_t carry, [[maybe_unused]] std::string_view instructionName);
		inline CoObj Add(const Register& r, std::string_view instructionName);
		inline CoObj Add(uint16_t addr, std::string_view instructionName);
		inline CoObj Adc(const Register& r, std::string_view instructionName);
		inline CoObj Adc(uint16_t addr, std::string_view instructionName);
		inline Register Sub(const Register& r, uint8_t withCarry, std::string_view instructionName);
		inline CoObj Sub(const Register& r, std::string_view instructionName);
		inline CoObj Sub(uint16_t addr, std::string_view instructionName);
		inline CoObj Sbb(const Register& r, std::string_view instructionName);
		inline CoObj Sbb(uint16_t addr, std::string_view instructionName);
		inline void Ana(const Register& r);
		inline CoObj Ana(const Register& r, std::string_view instructionName);
		inline CoObj Ana(uint16_t addr, std::string_view instructionName);
		inline void Xra(const Register& r);
		inline CoObj Xra(const Register& r, std::string_view instructionName);
		inline CoObj Xra(uint16_t addr, std::string_view instructionName);
		inline void Ora(const Register& r);
		inline CoObj Ora(const Register& r, std::string_view instructionName);
		inline CoObj Ora(uint16_t addr, std::string_view instructionName);
		inline CoObj Cmp(const Register& r, std::string_view instructionName);
		inline CoObj Cmp(uint16_t addr, std::string_view instructionName);
		inline CoObj NotImplemented();
		inline CoObj RetOnFlag(bool status, std::string_view instructionName);
		inline CoObj Pop(Register& hi, Register& low);
		inline CoObj JmpOnFlag(bool status, std::string_view instructionName);
		inline CoObj CallOnFlag(bool status, std::string_view instructionName);
		inline CoObj Push(const Register& hi, const Register low);
		inline CoObj Adi(const Register& r);
		inline CoObj Rst();
		inline CoObj Rst(uint8_t restart);
		inline CoObj Out();
		inline CoObj In();
		inline CoObj Xthl();
		inline CoObj Pchl();
		inline CoObj Xchg();
		inline CoObj Di();
		inline CoObj Sphl();
		inline CoObj Ei();

		CoObj Fetch();

		void Dump(bool fileWrite);
		FILE* fout_{};
		bool start_dump = false;
		uint64_t totalTp_{};

	public:
		/* I8080 overrides */
		//bool Execute() override final;
		uint8_t Execute() override final;
		void Reset(uint16_t programCounter) override final;
		/* End I8080 overrides */

		Intel8080() = default;
		Intel8080 (const SystemBus<uint16_t, uint8_t, 8>& systemBus);
		~Intel8080() { if (fout_ != nullptr) fclose (fout_); }

		uint8_t A() const { return Value(a_); }
		uint8_t B() const { return Value(b_); }
		uint8_t C() const { return Value(c_); }
		uint8_t D() const { return Value(d_); }
		uint8_t E() const { return Value(e_); }
		uint8_t H() const { return Value(h_); }
		uint8_t L() const { return Value(l_); }
		uint16_t SP() const { return sp_; }

		void PC(uint16_t pc) { pc_ = pc; }

		bool Zero() const { return status_[Condition::ZeroFlag]; }
		bool Sign() const { return status_[Condition::SignFlag]; }
		bool Parity() const { return status_[Condition::ParityFlag]; }
		bool AuxCarry() const { return status_[Condition::AuxCarryFlag]; }
		bool Carry() const { return status_[Condition::CarryFlag]; }
	};
}
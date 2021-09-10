module;
#include <coroutine>

export module _8080;

import <array>;
import <bitset>;
//import <coroutine>;
import <functional>;
import <string_view>;
import Base;
import I8080;
import SystemBus;

namespace Emulator
{
	constexpr bool dbg = false;

/* start co routine stuff - move to utils ... co-routine helper header */
	//MUST define a struct called 'promise_type'
	struct promise_type
	{
		//promise_type() = default;

		promise_type get_return_object() { return {}; }
		std::suspend_never initial_suspend() { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		void return_void() noexcept {}
		void unhandled_exception() { throw; } //throw the right exception, look me up ... std::exception::broken_promise
	};

	struct Awaiter
	{
		std::coroutine_handle<promise_type>* hp_;
		//await_ready is an optimization, If it returns true, then co_await does not suspend the function.
		constexpr bool await_ready() const noexcept { return false; }
		void await_suspend(std::coroutine_handle<promise_type> h) { *hp_ = h; }
		//the method await_resume here returns void, but if instead it returned a value,
		//this value would be the value of the co_await expression.
		constexpr void await_resume() const noexcept {}
	};
/* end co routine stuff */

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

/* start co routine stuff */
		std::coroutine_handle<promise_type> coh_;
		//std::coroutine_handle<promise_type> coh0_;
/* end co routine stuff */

		//Helper funtions to prime the address and data buses
		//for read write operations.
		void ReadFromAddress(Signal readLocation, uint16_t addr);
		void WriteToAddress(Signal writeLocation, uint16_t addr, uint8_t value);
		
		//this will be heap allocated
		//std::array<uint8_t, 65536/*maxMemory*/> memory_{};

		/**
			The program counter is a 16 bit register which is accessible to the programmer and whose contents indicate the
			address of the next instruction to be executed.
		*/
		uint16_t pc_{};
		//uint16_t end_{};

		/**
			A stack is an area of memory set aside by the programmer in which data or addresses are stored and retrieved
			by stack operations. The programmer specifies which addresses the stack operations will
			operate upon via a special accessible 16-bit register called the stack pointer.
		*/
		uint16_t sp_{};

		/**
			Five condition (or status) bits are provided by the
			8080 to reflect the results of data operations
		*/
		Register status_ = 0b00000010;

		//interrupt flip-flip - 1 enabled, 0 disabled
		bool iff_{};

		//The number of time periods that the current
		//instruction requires to execute.
		int8_t timePeriods_{};

		//The 'CPU' is 'always' running, however since we are emulating this
		//we need a way to 'exit'.
		//bool running_{ true };

		//The co routine is suspended at it's final suspend point.
		bool coDone_{ true };

		//The opcode for the instruction to be executed.
		uint8_t opcode_{};

		//Initialise to an invalid value.
		ISR isr_{ISR::NoInterrupt};
		//uint8_t isr_{ 0xFF };

		std::unique_ptr<std::function <promise_type()>[]> opcodeTable_;

		//See I8080IO.ixx for documentation.
		//Need to remove this once we finish the IO Controller
		//std::shared_ptr<I8080IO> io_;

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
		inline promise_type Inr(Register& r);
		inline promise_type Inr();
		inline promise_type Dcr(Register& r);
		inline promise_type Dcr(uint16_t addr);
		inline promise_type Mvi(Register& reg);
		inline promise_type Mvi();
		inline promise_type Daa();
		inline promise_type Rlc();
		inline promise_type Rrc();
		inline promise_type Ral();
		inline promise_type Rar();
		inline promise_type Lxi(Register& regHi, Register& regLow);
		inline promise_type Lxi();
		inline promise_type Shld();
		inline promise_type Stax(const Register& hi, const Register& low);
		inline promise_type Inx(Register& hi, Register& low);
		inline promise_type Inx();
		inline promise_type Dad(const Register& hi, const Register& low);
		inline promise_type Dad();
		inline promise_type Lhld();
		inline promise_type Ldax(const Register& hi, const Register& low);
		inline promise_type Dcx(Register& hi, Register& low);
		inline promise_type Dcx();
		inline promise_type Cma();
		inline promise_type Sta();
		inline promise_type Stc();
		inline promise_type Lda();
		inline promise_type Cmc();
		inline promise_type Mov(Register& lhs, const Register& rhs);
		inline promise_type Mov(Register& lhs);
		inline promise_type Mov(uint8_t value);
		inline promise_type Nop();
		inline promise_type Hlt();
		inline Register Add(const Register& lhs, const Register& rhs, bool setCarryFlag, uint8_t carry, [[maybe_unused]] std::string_view instructionName);
		inline promise_type Add(const Register& r, std::string_view instructionName);
		inline promise_type Add(uint16_t addr, std::string_view instructionName);
		inline promise_type Adc(const Register& r, std::string_view instructionName);
		inline promise_type Adc(uint16_t addr, std::string_view instructionName);
		inline Register Sub(const Register& r, uint8_t withCarry, std::string_view instructionName);
		inline promise_type Sub(const Register& r, std::string_view instructionName);
		inline promise_type Sub(uint16_t addr, std::string_view instructionName);
		inline promise_type Sbb(const Register& r, std::string_view instructionName);
		inline promise_type Sbb(uint16_t addr, std::string_view instructionName);
		inline void Ana(const Register& r);
		inline promise_type Ana(const Register& r, std::string_view instructionName);
		inline promise_type Ana(uint16_t addr, std::string_view instructionName);
		inline void Xra(const Register& r);
		inline promise_type Xra(const Register& r, std::string_view instructionName);
		inline promise_type Xra(uint16_t addr, std::string_view instructionName);
		inline void Ora(const Register& r);
		inline promise_type Ora(const Register& r, std::string_view instructionName);
		inline promise_type Ora(uint16_t addr, std::string_view instructionName);
		//inline void Cmp(const Register& r);
		inline promise_type Cmp(const Register& r, std::string_view instructionName);
		inline promise_type Cmp(uint16_t addr, std::string_view instructionName);
		inline promise_type NotImplemented();
		inline promise_type Ret();
		inline promise_type Ret(std::string_view instructionName);
		inline promise_type RetOnFlag(bool status, std::string_view instructionName);
		inline promise_type Rnz();
		inline promise_type Rz();
		inline promise_type Pop(Register& hi, Register& low);
		//inline void Jmp(uint16_t addr);
		//inline promise_type Jmp(std::string_view instructionName);
		inline promise_type JmpOnFlag(bool status, std::string_view instructionName);
		inline promise_type Jnz();
		inline promise_type Call(uint16_t addr);
		//inline promise_type Call();
		inline promise_type CallOnFlag(bool status, std::string_view instructionName);
		inline promise_type Cnz();
		inline promise_type Push(uint8_t hi, uint8_t low);
		inline promise_type Push(const Register& hi, const Register low);
		inline promise_type Adi(const Register& r);
		inline promise_type Rst();
		inline promise_type Rst(uint8_t restart);
		inline promise_type Jz();
		inline promise_type Cz();
		inline promise_type Rnc();
		inline promise_type Jnc();
		inline promise_type Out();
		inline promise_type Cnc();
		inline promise_type Rc();
		inline promise_type Jc();
		inline promise_type In();
		inline promise_type Cc();
		inline promise_type Rpo();
		inline promise_type Jpo();
		inline promise_type Xthl();
		inline promise_type Cpo();
		inline promise_type Rpe();
		inline promise_type Pchl();
		inline promise_type Jpe();
		inline promise_type Xchg();
		inline promise_type Cpe();
		inline promise_type Rp();
		inline promise_type Jp();
		inline promise_type Di();
		inline promise_type Cp();
		inline promise_type Rm();
		inline promise_type Sphl();
		inline promise_type Jm();
		inline promise_type Ei();
		inline promise_type Cm();

		promise_type FetchDecodeExecute();

	public:
		/**
			The programmer visualizes memory as a sequence of
			bytes, each of which may store 8 bits (represented by two
			hexadecimal digits). Up to 65,536 bytes of memory may be
			present, and an individual memory byte is addressed by its
			sequential number from 0 to 65,535D=FFFFH, the largest
			number which can be represented by 16 bits.
		*/
		//static constexpr uint32_t maxMemory = static_cast<uint32_t>(65536);

		/* I8080.ixx interface overrides*/

		/** LoadProgram
		*
		*	An Intel8080 program to emulate.
		*
		*	@param	program		The 8080 machine code to load into memory.
		*	@param	offset		The offset from the beginning of memory to load the program into.
		*/
		//void LoadProgram(std::string_view program, uint32_t offset);
		//void LoadProgram(const std::vector<std::byte>& program, uint32_t offset);

		//void RunProgram();
		
		//I8080 overrides
		bool Execute() override final;
		void Reset() override final;

		//void IO(std::shared_ptr<I8080IO> io);
		
		/* end overrides */

		Intel8080() = default;
		Intel8080 (const SystemBus<uint16_t, uint8_t, 8>& systemBus);

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
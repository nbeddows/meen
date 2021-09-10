#include <future>
#include "gtest/gtest.h"

import <chrono>;
import <filesystem>;
//We mainly want access to the unexposed cpu functions, hence we don't import I8080 here
import _8080;
import Base;
import ControllerFactory;
import IController;
//so we can cast to CpmIoController to get the message for the cpudiag test
import Controller;
import ICpuClock;
import SystemBus;

using namespace std::chrono;
using namespace Emulator;

#if 0
class MockMachine final
{
	private:
		std::unique_ptr<Intel8080> cpu_;
		std::unique_ptr<ICpuClock> clock_;
		std::shared_ptr<IController> ioController_;
		std::shared_ptr<IController> testIoController_;
		std::shared_ptr<IController> cpmIoController_;
		std::shared_ptr<IMemoryController> memoryController_;
		std::shared_ptr<AddressBus<uint16_t>> addressBus_;
		std::shared_ptr<DataBus<uint8_t>> dataBus_;
		std::shared_ptr<ControlBus<8>> controlBus_;

	public:
		MockMachine();
};

//template <class addrW, class aataW, int ctrlW, int clockSpeed>
MockMachine::MockMachine()
{
	SystemBus<uint16_t, uint8_t, 8> systemBus;

	clock_ = MakeCpuClock(systemBus.controlBus, nanoseconds(2000));
	cpu_ = std::make_unique<Intel8080>(systemBus);
	memoryController_ = MakeDefaultMemoryController(systemBus.addressBus->Size()); //16 bits addressable memory
	testIoController_ = MakeTestIoController(systemBus.controlBus);
	cpmIoController_ = MakeCpmIoController(memoryController_, systemBus.controlBus);

	addressBus_ = systemBus.addressBus;
	dataBus_ = systemBus.dataBus;
	controlBus_ = systemBus.controlBus;
}
#endif

class Intel8080Test : public testing::Test
{
protected:
	static std::unique_ptr<ICpuClock> clock_;
	static std::unique_ptr<Intel8080> cpu_;
	static SystemBus<uint16_t, uint8_t, 8> systemBus_;
	static std::shared_ptr<IController> ioController_;
	static std::shared_ptr<IController> testIoController_;
	static std::shared_ptr<IController> cpmIoController_;
	static std::shared_ptr<IMemoryController> memoryController_;

	static const inline std::filesystem::path directory_ = "../../programs";
	
	//static std::shared_ptr<MockMachine> machine_;

	void CheckStatus (bool zero, bool sign, bool parity, bool auxCarry, bool carry) const;
	void LoadAndRun(std::filesystem::path, uint32_t offset = 0x100) const;
	
public:
	static void SetUpTestCase();
	static void TearDownTestCase();
	void SetUp();
	void TearDown();
};

std::unique_ptr<ICpuClock> Intel8080Test::clock_;
std::unique_ptr<Intel8080> Intel8080Test::cpu_;
SystemBus<uint16_t, uint8_t, 8> Intel8080Test::systemBus_;
std::shared_ptr<IController> Intel8080Test::ioController_;
std::shared_ptr<IController> Intel8080Test::testIoController_;
std::shared_ptr<IController> Intel8080Test::cpmIoController_;
std::shared_ptr<IMemoryController> Intel8080Test::memoryController_;

//std::shared_ptr<MockMachine> Intel8080Test::machine_ = std::make_shared<MockMachine>();

void Intel8080Test::CheckStatus(bool zero, bool sign, bool parity, bool auxCarry, bool carry) const
{
	EXPECT_EQ(zero, cpu_->Zero());
	EXPECT_EQ(sign, cpu_->Sign());
	EXPECT_EQ(parity, cpu_->Parity());
	EXPECT_EQ(auxCarry, cpu_->AuxCarry());
	EXPECT_EQ(carry, cpu_->Carry());
}

void Intel8080Test::LoadAndRun(std::filesystem::path name, uint32_t offset) const
{
	auto path = directory_;
	
	//EXPECT_NO_THROW
	//(

	memoryController_->Load(path /= name, offset);
	cpu_->PC(offset);

	auto addressBus = systemBus_.addressBus;
	auto dataBus = systemBus_.dataBus;
	auto controlBus = systemBus_.controlBus;

	while (controlBus->Receive(Signal::PowerOff) == false)
	{
		auto currTime = clock_->Tick();

		//check if we can tick the cpu
		if (controlBus->Receive(Signal::Clock) == true)
		{
			//(continue to) execute the next instruction
			auto executionComplete = cpu_->Execute();

			if (memoryController_ != nullptr)
			{
				//check the control bus to see if there are any operations pending
				if (controlBus->Receive(Signal::MemoryRead))
				{
					dataBus->Send(memoryController_->Read(addressBus->Receive()));
				}

				if (controlBus->Receive(Signal::MemoryWrite))
				{
					memoryController_->Write(addressBus->Receive(), dataBus->Receive());
				}
			}

			if (ioController_ != nullptr)
			{
				if (controlBus->Receive(Signal::IoRead))
				{
					dataBus->Send(ioController_->Read(addressBus->Receive()));
				}

				if (controlBus->Receive(Signal::IoWrite))
				{
					ioController_->Write(addressBus->Receive(), dataBus->Receive());
				}

				//Check the IO to see if any interrupts are pending, don't check if we are in the middle of executing an instruction
				//otherwise we will overload the databus.
				if (executionComplete == true)
				{
					auto isr = ioController_->ServiceInterrupts(currTime);

					if (isr != ISR::NoInterrupt)
					{
						controlBus->Send(Signal::Interrupt);
						dataBus->Send(static_cast<uint8_t>(isr));
					}
				}
			}
		}
	}
	//);
}

void Intel8080Test::SetUpTestCase()
{
	//systemBus_ = SystemBus<uint16_t, uint8_t, 8>();
	//We want access to the implementation so we don't use the factory here
	clock_ = MakeCpuClock (systemBus_.controlBus, nanoseconds(2000));
	cpu_ = std::make_unique<Intel8080>(systemBus_);
	memoryController_ = MakeDefaultMemoryController (16); //16 bits addressable memory
	testIoController_ = MakeTestIoController(systemBus_.controlBus);
	cpmIoController_ = MakeCpmIoController(static_pointer_cast<IController>(memoryController_), systemBus_.controlBus);
	ioController_ = testIoController_;
}

void Intel8080Test::TearDownTestCase()
{

}

void Intel8080Test::SetUp()
{
	//This needs to be a flag on the control bus.
	cpu_->Reset();
}

void Intel8080Test::TearDown()
{
	//cpu_->UnloadProgram();
}

TEST_F(Intel8080Test, LXI_B)
{
	LoadAndRun("lxib.bin");
	EXPECT_EQ(0xFF, cpu_->B());
	EXPECT_EQ(0x12, cpu_->C());
}

TEST_F(Intel8080Test, STAX_B)
{
	LoadAndRun("staxb.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, INX_B)
{
	LoadAndRun("inxb.bin");
	EXPECT_EQ(0xFF, cpu_->B());
	EXPECT_EQ(0x13, cpu_->C());
	//No flags should be changed by this instruction
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, INR_B)
{
	LoadAndRun("inrb.bin");
	EXPECT_EQ(0x10, cpu_->B());
	//zero, sign, parity, auxCarry, carry
	CheckStatus (false, false, false, true, false);
}

TEST_F(Intel8080Test, DCR_B)
{
	LoadAndRun("dcrb.bin");
	EXPECT_EQ(0x00, cpu_->B());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, MVI_B)
{
	LoadAndRun("mvib.bin");
	EXPECT_EQ(0x12, cpu_->B());
}

TEST_F(Intel8080Test, RLC)
{
	LoadAndRun("rlc.bin");
	EXPECT_EQ(0xE5, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, true);
}

TEST_F(Intel8080Test, DAD_B)
{
	LoadAndRun("dadb.bin");
	//Answer in HL
	EXPECT_EQ(0xD5, cpu_->H());
	EXPECT_EQ(0x1A, cpu_->L());
	//BC remains unchanged
	EXPECT_EQ(0x33, cpu_->B());
	EXPECT_EQ(0x9F, cpu_->C());
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, LDAX_B)
{
	LoadAndRun("ldaxb.bin");
	EXPECT_EQ(0x0A, cpu_->A());
}

TEST_F(Intel8080Test, DCX_B)
{
	LoadAndRun("dcxb.bin");
	EXPECT_EQ(0xFF, cpu_->B());
	EXPECT_EQ(0xFF, cpu_->C());
	//No flags should be changed by this instruction
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, INR_C)
{
	LoadAndRun("inrc.bin");
	EXPECT_EQ(0x10, cpu_->C());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, true, false);
}

TEST_F(Intel8080Test, DCR_C)
{
	LoadAndRun("dcrc.bin");
	EXPECT_EQ(0x00, cpu_->C());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, MVI_C)
{
	LoadAndRun("mvic.bin");
	EXPECT_EQ(0x12, cpu_->C());
}

TEST_F(Intel8080Test, RRC)
{
	LoadAndRun("rrc.bin");
	EXPECT_EQ(0x79, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, LXI_D)
{
	LoadAndRun("lxid.bin");
	EXPECT_EQ(0xFF, cpu_->D());
	EXPECT_EQ(0x12, cpu_->E());
}

TEST_F(Intel8080Test, STAX_D)
{
	LoadAndRun("staxd.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, INX_D)
{
	LoadAndRun("inxd.bin");
	EXPECT_EQ(0xFF, cpu_->D());
	EXPECT_EQ(0x13, cpu_->E());
	//No flags should be changed by this instruction
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, INR_D)
{
	LoadAndRun("inrd.bin");
	EXPECT_EQ(0x10, cpu_->D());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, true, false);
}

TEST_F(Intel8080Test, DCR_D)
{
	LoadAndRun("dcrd.bin");
	EXPECT_EQ(0x00, cpu_->D());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, MVI_D)
{
	LoadAndRun("mvid.bin");
	EXPECT_EQ(0x12, cpu_->D());
}

TEST_F(Intel8080Test, RAL)
{
	LoadAndRun("ral.bin");
	EXPECT_EQ(0x6A, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, true);
}

TEST_F(Intel8080Test, DAD_D)
{
	LoadAndRun("dadd.bin");
	//Answer in HL
	EXPECT_EQ(0x00, cpu_->H());
	EXPECT_EQ(0x00, cpu_->L());
	//DE remains unchanged
	EXPECT_EQ(0x00, cpu_->D());
	EXPECT_EQ(0x01, cpu_->E());
	CheckStatus(false, false, false, false, true);
}

TEST_F(Intel8080Test, LDAX_D)
{
	LoadAndRun("ldaxd.bin");
	EXPECT_EQ(0x1A, cpu_->A());
}

TEST_F(Intel8080Test, DCX_D)
{
	LoadAndRun("dcxd.bin");
	EXPECT_EQ(0xFF, cpu_->D());
	EXPECT_EQ(0xFF, cpu_->E());
	//No flags should be changed by this instruction
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, INR_E)
{
	LoadAndRun("inre.bin");
	EXPECT_EQ(0x10, cpu_->E());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, true, false);
}

TEST_F(Intel8080Test, DCR_E)
{
	LoadAndRun("dcre.bin");
	EXPECT_EQ(0x00, cpu_->E());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, MVI_E)
{
	LoadAndRun("mvie.bin");
	EXPECT_EQ(0x12, cpu_->E());
}

TEST_F(Intel8080Test, RAR)
{
	LoadAndRun("rar.bin");
	EXPECT_EQ(0xB5, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, LXI_H)
{
	LoadAndRun("lxih.bin");
	EXPECT_EQ(0xFF, cpu_->H());
	EXPECT_EQ(0x12, cpu_->L());
}

TEST_F(Intel8080Test, SHLD)
{
	LoadAndRun("shld.bin");
	EXPECT_EQ(0xAB, cpu_->A());
	EXPECT_EQ(0xCD, cpu_->B());
}

TEST_F(Intel8080Test, INX_H)
{
	LoadAndRun("inxh.bin");
	EXPECT_EQ(0xFF, cpu_->H());
	EXPECT_EQ(0x13, cpu_->L());
	//No flags should be changed by this instruction
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, INR_H)
{
	LoadAndRun("inrh.bin");
	EXPECT_EQ(0x10, cpu_->H());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, true, false);
}

TEST_F(Intel8080Test, DCR_H)
{
	LoadAndRun("dcrh.bin");
	EXPECT_EQ(0x00, cpu_->H());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, MVI_H)
{
	LoadAndRun("mvih.bin");
	EXPECT_EQ(0x12, cpu_->H());
}


//Need accumulator mvi, instruction needs more work
TEST_F(Intel8080Test, DAA)
{
	LoadAndRun("daa.bin");
	EXPECT_EQ(0x01, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, true, true);
}


TEST_F(Intel8080Test, DAD_H)
{
	LoadAndRun("dadh.bin");
	//Answer in HL
	EXPECT_EQ(0x42, cpu_->H());
	EXPECT_EQ(0xF6, cpu_->L());
	CheckStatus(false, false, false, false, true);
}

TEST_F(Intel8080Test, LHLD)
{
	LoadAndRun("lhld.bin");
	EXPECT_EQ(0x7B, cpu_->L());
	EXPECT_EQ(0x2A, cpu_->H());
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, DCX_H)
{

	LoadAndRun("dcxh.bin");
	EXPECT_EQ(0xFF, cpu_->H());
	EXPECT_EQ(0xFF, cpu_->L());
	//No flags should be changed by this instruction
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, INR_L)
{
	LoadAndRun("inrl.bin");
	EXPECT_EQ(0x10, cpu_->L());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, true, false);
}

TEST_F(Intel8080Test, DCR_L)
{
	LoadAndRun("dcrl.bin");
	EXPECT_EQ(0x00, cpu_->L());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, MVI_L)
{
	LoadAndRun("mvil.bin");
	EXPECT_EQ(0x12, cpu_->L());
}

TEST_F(Intel8080Test, CMA)
{
	LoadAndRun("cma.bin");
	EXPECT_EQ(0x00, cpu_->A());
}

TEST_F(Intel8080Test, LXI_SP)
{
	LoadAndRun("lxisp.bin");
	EXPECT_EQ(0xCDAB, cpu_->SP());
}

TEST_F(Intel8080Test, STA)
{
	LoadAndRun("sta.bin");
	EXPECT_EQ(0xFF, cpu_->A());
}

TEST_F(Intel8080Test, INX_SP)
{
	LoadAndRun("inxsp.bin");
	EXPECT_EQ(0xCDAC, cpu_->SP());
}

TEST_F(Intel8080Test, INR_M)
{
	LoadAndRun("inrm.bin");
	EXPECT_EQ(0x22, cpu_->L());
	//EXPECT_EQ(0x35, cpu_->L());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, DCR_M)
{
	LoadAndRun("dcrm.bin");
	EXPECT_EQ(0x20, cpu_->L());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, true, false);
}

TEST_F(Intel8080Test, MVI_M)
{
	LoadAndRun("mvim.bin");
	EXPECT_EQ(0xDD, cpu_->L());
}

TEST_F(Intel8080Test, STC)
{
	LoadAndRun("stc.bin");
	CheckStatus(false, false, false, false, true);
}

TEST_F(Intel8080Test, DAD_SP)
{
	LoadAndRun("dadsp.bin");
	EXPECT_EQ(0x57, cpu_->H());
	EXPECT_EQ(0x9A, cpu_->L());
	CheckStatus(false, false, false, false, true);
}

TEST_F(Intel8080Test, LDA)
{
	LoadAndRun("lda.bin");
	EXPECT_EQ(0x3A, cpu_->A());
}

TEST_F(Intel8080Test, DCX_SP)
{
	LoadAndRun("dcxsp.bin");
	EXPECT_EQ(0xCDAA, cpu_->SP());
}

TEST_F(Intel8080Test, INR_A)
{
	LoadAndRun("inra.bin");
	EXPECT_EQ(0x3B, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, true);
}

TEST_F(Intel8080Test, DCR_A)
{
	//This program starts out by setting the carry bit
	//which tests the fact that inr does not modify the carry bit
	LoadAndRun("dcra.bin");
	EXPECT_EQ(0x39, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, true, true);
}

TEST_F(Intel8080Test, MVI_A)
{
	LoadAndRun("mvia.bin");
	EXPECT_EQ(0x12, cpu_->A());
}

TEST_F(Intel8080Test, CMC)
{
	LoadAndRun("cmc.bin");
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, true);
}

TEST_F(Intel8080Test, MOV_BB)
{
	LoadAndRun("movbb.bin");
	EXPECT_EQ(0x21, cpu_->B());
}

TEST_F(Intel8080Test, MOV_BC)
{
	LoadAndRun("movbc.bin");
	EXPECT_EQ(0x21, cpu_->B());
}

TEST_F(Intel8080Test, MOV_BD)
{
	LoadAndRun("movbd.bin");
	EXPECT_EQ(0x21, cpu_->B());
}

TEST_F(Intel8080Test, MOV_BE)
{
	LoadAndRun("movbe.bin");
	EXPECT_EQ(0x21, cpu_->B());
}

TEST_F(Intel8080Test, MOV_BH)
{
	LoadAndRun("movbh.bin");
	EXPECT_EQ(0x21, cpu_->B());
}

TEST_F(Intel8080Test, MOV_BL)
{
	LoadAndRun("movbl.bin");
	EXPECT_EQ(0x21, cpu_->B());
}

TEST_F(Intel8080Test, MOV_BM)
{
	LoadAndRun("movbm.bin");
	EXPECT_EQ(0x21, cpu_->B());
}

TEST_F(Intel8080Test, MOV_BA)
{
	LoadAndRun("movba.bin");
	EXPECT_EQ(0x21, cpu_->B());
}

TEST_F(Intel8080Test, MOV_CB)
{
	LoadAndRun("movcb.bin");
	EXPECT_EQ(0x21, cpu_->C());
}

TEST_F(Intel8080Test, MOV_CC)
{
	LoadAndRun("movcc.bin");
	EXPECT_EQ(0x21, cpu_->C());
}

TEST_F(Intel8080Test, MOV_CD)
{
	LoadAndRun("movcd.bin");
	EXPECT_EQ(0x21, cpu_->C());
}

TEST_F(Intel8080Test, MOV_CE)
{
	LoadAndRun("movce.bin");
	EXPECT_EQ(0x21, cpu_->C());
}

TEST_F(Intel8080Test, MOV_CH)
{
	LoadAndRun("movch.bin");
	EXPECT_EQ(0x21, cpu_->C());
}

TEST_F(Intel8080Test, MOV_CL)
{
	LoadAndRun("movcl.bin");
	EXPECT_EQ(0x21, cpu_->C());
}

TEST_F(Intel8080Test, MOV_CM)
{
	LoadAndRun("movcm.bin");
	EXPECT_EQ(0x21, cpu_->C());
}

TEST_F(Intel8080Test, MOV_CA)
{
	LoadAndRun("movca.bin");
	EXPECT_EQ(0x21, cpu_->C());
}

TEST_F(Intel8080Test, MOV_DB)
{
	LoadAndRun("movdb.bin");
	EXPECT_EQ(0x21, cpu_->D());
}

TEST_F(Intel8080Test, MOV_DC)
{
	LoadAndRun("movdc.bin");
	EXPECT_EQ(0x21, cpu_->D());
}

TEST_F(Intel8080Test, MOV_DD)
{
	LoadAndRun("movdd.bin");
	EXPECT_EQ(0x21, cpu_->D());
}

TEST_F(Intel8080Test, MOV_DE)
{
	LoadAndRun("movde.bin");
	EXPECT_EQ(0x21, cpu_->D());
}

TEST_F(Intel8080Test, MOV_DH)
{
	LoadAndRun("movdh.bin");
	EXPECT_EQ(0x21, cpu_->D());
}

TEST_F(Intel8080Test, MOV_DL)
{
	LoadAndRun("movdl.bin");
	EXPECT_EQ(0x21, cpu_->D());
}

TEST_F(Intel8080Test, MOV_DM)
{
	LoadAndRun("movdm.bin");
	EXPECT_EQ(0x21, cpu_->D());
}

TEST_F(Intel8080Test, MOV_DA)
{
	LoadAndRun("movda.bin");
	EXPECT_EQ(0x21, cpu_->D());
}

TEST_F(Intel8080Test, MOV_EB)
{
	LoadAndRun("moveb.bin");
	EXPECT_EQ(0x21, cpu_->E());
}

TEST_F(Intel8080Test, MOV_EC)
{
	LoadAndRun("movec.bin");
	EXPECT_EQ(0x21, cpu_->E());
}

TEST_F(Intel8080Test, MOV_ED)
{
	LoadAndRun("moved.bin");
	EXPECT_EQ(0x21, cpu_->E());
}

TEST_F(Intel8080Test, MOV_EE)
{
	LoadAndRun("movee.bin");
	EXPECT_EQ(0x21, cpu_->E());
}

TEST_F(Intel8080Test, MOV_EH)
{
	LoadAndRun("moveh.bin");
	EXPECT_EQ(0x21, cpu_->E());
}

TEST_F(Intel8080Test, MOV_EL)
{
	LoadAndRun("movel.bin");
	EXPECT_EQ(0x21, cpu_->E());
}

TEST_F(Intel8080Test, MOV_EM)
{
	LoadAndRun("movem.bin");
	EXPECT_EQ(0x21, cpu_->E());
}

TEST_F(Intel8080Test, MOV_EA)
{
	LoadAndRun("movea.bin");
	EXPECT_EQ(0x21, cpu_->E());
}

TEST_F(Intel8080Test, MOV_HB)
{
	LoadAndRun("movhb.bin");
	EXPECT_EQ(0x21, cpu_->H());
}

TEST_F(Intel8080Test, MOV_HC)
{
	LoadAndRun("movhc.bin");
	EXPECT_EQ(0x21, cpu_->H());
}

TEST_F(Intel8080Test, MOV_HD)
{
	LoadAndRun("movhd.bin");
	EXPECT_EQ(0x21, cpu_->H());
}

TEST_F(Intel8080Test, MOV_HE)
{
	LoadAndRun("movhe.bin");
	EXPECT_EQ(0x21, cpu_->H());
}

TEST_F(Intel8080Test, MOV_HH)
{
	LoadAndRun("movhh.bin");
	EXPECT_EQ(0x21, cpu_->H());
}

TEST_F(Intel8080Test, MOV_HL)
{
	LoadAndRun("movhl.bin");
	EXPECT_EQ(0x21, cpu_->H());
}

TEST_F(Intel8080Test, MOV_HM)
{
	LoadAndRun("movhm.bin");
	EXPECT_EQ(0x21, cpu_->H());
}

TEST_F(Intel8080Test, MOV_HA)
{
	LoadAndRun("movha.bin");
	EXPECT_EQ(0x21, cpu_->H());
}

TEST_F(Intel8080Test, MOV_LB)
{
	LoadAndRun("movlb.bin");
	EXPECT_EQ(0x21, cpu_->L());
}

TEST_F(Intel8080Test, MOV_LC)
{
	LoadAndRun("movlc.bin");
	EXPECT_EQ(0x21, cpu_->L());
}

TEST_F(Intel8080Test, MOV_LD)
{
	LoadAndRun("movld.bin");
	EXPECT_EQ(0x21, cpu_->L());
}

TEST_F(Intel8080Test, MOV_LE)
{
	LoadAndRun("movle.bin");
	EXPECT_EQ(0x21, cpu_->L());
}

TEST_F(Intel8080Test, MOV_LH)
{
	LoadAndRun("movlh.bin");
	EXPECT_EQ(0x21, cpu_->L());
}

TEST_F(Intel8080Test, MOV_LL)
{
	LoadAndRun("movll.bin");
	EXPECT_EQ(0x21, cpu_->L());
}

TEST_F(Intel8080Test, MOV_LM)
{
	LoadAndRun("movlm.bin");
	EXPECT_EQ(0x21, cpu_->L());
}

TEST_F(Intel8080Test, MOV_LA)
{
	LoadAndRun("movla.bin");
	EXPECT_EQ(0x21, cpu_->L());
}

//Using MovA for the memory tests since
//we store the result in the accumulator
TEST_F(Intel8080Test, MOV_MB)
{
	LoadAndRun("movmb.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, MOV_MC)
{
	LoadAndRun("movmc.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, MOV_MD)
{
	LoadAndRun("movmd.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, MOV_ME)
{
	LoadAndRun("movme.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

/*
//These two tests need to be rewritten since
//the registers to test are being overwritten
//by the program

TEST_F(Intel8080Test, MOV_MH)
{
	LoadAndRun("./programs/movmh.bin");
}

TEST_F(Intel8080Test, MOV_ML)
{
	LoadAndRun("./programs/movml.bin");
}
*/

//TEST_F(Intel8080Test, HLT)
//{
	/*
		cpu_->LoadProgram(./programs/hlt.bin, 0);

		thread
		{
			//cpu should halt
			LoadAndRun("./programs/hlt.bin");
		
			//program should return
			//set future
		}

		//wait 2 seconds
		cpu_->Interrupt(1);

		future_status
	*/
//}

TEST_F(Intel8080Test, MOV_MA)
{
	LoadAndRun("movma.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, MOV_AB)
{
	LoadAndRun("movab.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, MOV_AC)
{
	LoadAndRun("movac.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, MOV_AD)
{
	LoadAndRun("movad.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, MOV_AE)
{
	LoadAndRun("movae.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, MOV_AH)
{
	LoadAndRun("movah.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, MOV_AL)
{
	LoadAndRun("moval.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, MOV_AM)
{
	LoadAndRun("movam.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, MOV_AA)
{
	LoadAndRun("movaa.bin");
	EXPECT_EQ(0x21, cpu_->A());
}

TEST_F(Intel8080Test, MovIncDec)
{
	LoadAndRun("movIncDec.bin");
	EXPECT_EQ(0x77, cpu_->A());
}

TEST_F(Intel8080Test, ADD_B)
{
	LoadAndRun("addb.bin");
	EXPECT_EQ(0x9A, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, true, false);
}

TEST_F(Intel8080Test, ADD_C)
{
	LoadAndRun("addc.bin");
	EXPECT_EQ(0x9A, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, true, false);
}

TEST_F(Intel8080Test, ADD_D)
{
	LoadAndRun("addd.bin");
	EXPECT_EQ(0x9A, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, true, false);
}

TEST_F(Intel8080Test, ADD_E)
{
	LoadAndRun("adde.bin");
	EXPECT_EQ(0x9A, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, true, false);
}

TEST_F(Intel8080Test, ADD_H)
{
	LoadAndRun("addh.bin");
	EXPECT_EQ(0x9A, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, true, false);
}

TEST_F(Intel8080Test, ADD_L)
{
	LoadAndRun("addl.bin");
	EXPECT_EQ(0x9A, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, true, false);
}

TEST_F(Intel8080Test, ADD_M)
{
	LoadAndRun("addm.bin");
	EXPECT_EQ(0xD8, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, true, false);
}

TEST_F(Intel8080Test, ADD_A)
{
	LoadAndRun("adda.bin");
	EXPECT_EQ(0xD8, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, true, false);
}

TEST_F(Intel8080Test, ADC_B)
{
	LoadAndRun("adcb.bin");
	EXPECT_EQ(0x80, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, false, true, false);
}

TEST_F(Intel8080Test, ADC_C)
{
	LoadAndRun("adcc.bin");
	EXPECT_EQ(0x80, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, false, true, false);
}

TEST_F(Intel8080Test, ADC_D)
{
	LoadAndRun("adcd.bin");
	EXPECT_EQ(0x80, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, false, true, false);
}

TEST_F(Intel8080Test, ADC_E)
{
	LoadAndRun("adce.bin");
	EXPECT_EQ(0x80, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, false, true, false);
}

TEST_F(Intel8080Test, ADC_H)
{
	LoadAndRun("adch.bin");
	EXPECT_EQ(0x80, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, false, true, false);
}

TEST_F(Intel8080Test, ADC_L)
{
	LoadAndRun("adcl.bin");
	EXPECT_EQ(0x80, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, false, true, false);
}

TEST_F(Intel8080Test, ADC_M)
{
	LoadAndRun("adcm.bin");
	EXPECT_EQ(0x85, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, false, false, false);
}

TEST_F(Intel8080Test, ADC_A)
{
	LoadAndRun("adca.bin");
	EXPECT_EQ(0x85, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, false, false, false);
}

TEST_F(Intel8080Test, SUB_B)
{
	LoadAndRun("subb.bin");
	EXPECT_EQ(0x00, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, SUB_C)
{
	LoadAndRun("subc.bin");
	EXPECT_EQ(0x00, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, SUB_D)
{
	LoadAndRun("subd.bin");
	EXPECT_EQ(0x00, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, SUB_E)
{
	LoadAndRun("sube.bin");
	EXPECT_EQ(0x00, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, SUB_H)
{
	LoadAndRun("subh.bin");
	EXPECT_EQ(0x00, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, SUB_L)
{
	LoadAndRun("subl.bin");
	EXPECT_EQ(0x00, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, SUB_M)
{
	LoadAndRun("subm.bin");
	EXPECT_EQ(0x00, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, SUB_A)
{
	LoadAndRun("suba.bin");
	EXPECT_EQ(0x00, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, SBB_B)
{
	LoadAndRun("sbbb.bin");
	EXPECT_EQ(0x01, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, true, false);
}

TEST_F(Intel8080Test, SBB_C)
{
	LoadAndRun("sbbc.bin");
	EXPECT_EQ(0x01, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, true, false);
}

TEST_F(Intel8080Test, SBB_D)
{
	LoadAndRun("sbbd.bin");
	EXPECT_EQ(0x01, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, true, false);
}

TEST_F(Intel8080Test, SBB_E)
{
	LoadAndRun("sbbe.bin");
	EXPECT_EQ(0x01, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, true, false);
}

TEST_F(Intel8080Test, SBB_H)
{
	LoadAndRun("sbbh.bin");
	EXPECT_EQ(0x01, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, true, false);
}

TEST_F(Intel8080Test, SBB_L)
{
	LoadAndRun("sbbl.bin");
	EXPECT_EQ(0x01, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, true, false);
}

TEST_F(Intel8080Test, SBB_M)
{
	LoadAndRun("sbbm.bin");
	EXPECT_EQ(0xFF, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, true);
}

TEST_F(Intel8080Test, SBB_A)
{
	LoadAndRun("sbba.bin");
	EXPECT_EQ(0xFF, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, true);
}

TEST_F(Intel8080Test, ANA_B)
{
	LoadAndRun("anab.bin");
	EXPECT_EQ(0x0C, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ANA_C)
{
	LoadAndRun("anac.bin");
	EXPECT_EQ(0x0C, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ANA_D)
{
	LoadAndRun("anad.bin");
	EXPECT_EQ(0x0C, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ANA_E)
{
	LoadAndRun("anae.bin");
	EXPECT_EQ(0x0C, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ANA_H)
{
	LoadAndRun("anah.bin");
	EXPECT_EQ(0x0C, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ANA_L)
{
	LoadAndRun("anal.bin");
	EXPECT_EQ(0x0C, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ANA_M)
{
	LoadAndRun("anam.bin");
	EXPECT_EQ(0x20, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, ANA_A)
{
	LoadAndRun("anaa.bin");
	EXPECT_EQ(0xFC, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, false);
}

TEST_F(Intel8080Test, XRA_B)
{
	LoadAndRun("xrab.bin");
	EXPECT_EQ(0xF0, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, false);
}

TEST_F(Intel8080Test, XRA_C)
{
	LoadAndRun("xrac.bin");
	EXPECT_EQ(0xF0, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, false);
}

TEST_F(Intel8080Test, XRA_D)
{
	LoadAndRun("xrad.bin");
	EXPECT_EQ(0xF0, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, false);
}

TEST_F(Intel8080Test, XRA_E)
{
	LoadAndRun("xrae.bin");
	EXPECT_EQ(0xF0, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, false);
}

TEST_F(Intel8080Test, XRA_H)
{
	LoadAndRun("xrah.bin");
	EXPECT_EQ(0xF0, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, false);
}

TEST_F(Intel8080Test, XRA_L)
{
	LoadAndRun("xral.bin");
	EXPECT_EQ(0xF0, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, false);
}

TEST_F(Intel8080Test, XRA_M)
{
	LoadAndRun("xram.bin");
	EXPECT_EQ(0xDE, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, false);
}

TEST_F(Intel8080Test, XRA_A)
{
	LoadAndRun("xraa.bin");
	EXPECT_EQ(0x00, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, false, false);
}

TEST_F(Intel8080Test, ORA_B)
{
	LoadAndRun("orab.bin");
	EXPECT_EQ(0x3F, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ORA_C)
{
	LoadAndRun("orac.bin");
	EXPECT_EQ(0x3F, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ORA_D)
{
	LoadAndRun("orad.bin");
	EXPECT_EQ(0x3F, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ORA_E)
{
	LoadAndRun("orae.bin");
	EXPECT_EQ(0x3F, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ORA_H)
{
	LoadAndRun("orah.bin");
	EXPECT_EQ(0x3F, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ORA_L)
{
	LoadAndRun("oral.bin");
	EXPECT_EQ(0x3F, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ORA_M)
{
	LoadAndRun("oram.bin");
	EXPECT_EQ(0xFF, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, false);
}

TEST_F(Intel8080Test, ORA_A)
{
	LoadAndRun("oraa.bin");
	EXPECT_EQ(0xFC, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, false);
}

TEST_F(Intel8080Test, CMP_B)
{
	LoadAndRun("cmpb.bin");
	EXPECT_EQ(0x0A, cpu_->A());
	EXPECT_EQ(0x05, cpu_->B());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, true, false);
}

TEST_F(Intel8080Test, CMP_C)
{
	LoadAndRun("cmpc.bin");
	EXPECT_EQ(0x0A, cpu_->A());
	EXPECT_EQ(0x05, cpu_->C());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, true, false);
}

TEST_F(Intel8080Test, CMP_D)
{
	LoadAndRun("cmpd.bin");
	EXPECT_EQ(0x0A, cpu_->A());
	EXPECT_EQ(0x05, cpu_->D());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, true, false);
}

TEST_F(Intel8080Test, CMP_E)
{
	LoadAndRun("cmpe.bin");
	EXPECT_EQ(0x0A, cpu_->A());
	EXPECT_EQ(0x05, cpu_->E());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, true, false);
}

TEST_F(Intel8080Test, CMP_H)
{
	LoadAndRun("cmph.bin");
	EXPECT_EQ(0x0A, cpu_->A());
	EXPECT_EQ(0x05, cpu_->H());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, true, false);
}

TEST_F(Intel8080Test, CMP_L)
{
	LoadAndRun("cmpl.bin");
	EXPECT_EQ(0x0A, cpu_->A());
	EXPECT_EQ(0x05, cpu_->L());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, true, false);
}

TEST_F(Intel8080Test, CMP_M)
{
	LoadAndRun("cmpm.bin");
	//The contents of the accumulator and the memory must not change.
	EXPECT_EQ(0x0A, cpu_->A());
	EXPECT_EQ(0x21, memoryController_->Read(0x0102));

	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, false, true, true);
}

/*
	Need to fix git settings, seems to be adding a carraige return to this
	test which causes it to fail
*/
#ifdef ENABLE_CMP_A
TEST_F(Intel8080Test, CMP_A)
{
	LoadAndRun("cmpa.bin");
	EXPECT_EQ(0x0A, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}
#endif

TEST_F(Intel8080Test, RNZ)
{
	LoadAndRun("rnz.bin");
	EXPECT_EQ(0x00, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, PUSH_POP_B)
{
	LoadAndRun("pushpopb.bin");
	EXPECT_EQ(0xAB, cpu_->B());
	EXPECT_EQ(0xCD, cpu_->C());
}

TEST_F(Intel8080Test, JNZ)
{
	LoadAndRun("jnz.bin");
	EXPECT_EQ(0xFF, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, false);
}

TEST_F(Intel8080Test, JMP)
{
	LoadAndRun("jmp.bin");
	EXPECT_EQ(0x3F, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, CNZ)
{
	LoadAndRun("cnz.bin");
	EXPECT_EQ(0x30, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ADI_1)
{
	LoadAndRun("adi1.bin");
	EXPECT_EQ(0x56, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, ADI_2)
{
	LoadAndRun("adi2.bin");
	EXPECT_EQ(0x14, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, true, true);
}
/*
TEST_F(Intel8080Test, RST_0)
{
	LoadAndRun("rst0.bin");
	EXPECT_EQ(0x02, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}
*/
TEST_F(Intel8080Test, RST_1)
{
	LoadAndRun("rst1.bin", 0x00);
	EXPECT_EQ(0x02, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, RST_2)
{
	LoadAndRun("rst2.bin", 0x00);
	EXPECT_EQ(0x02, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, RST_3)
{
	LoadAndRun("rst3.bin", 0x00);
	EXPECT_EQ(0x02, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, RST_4)
{
	LoadAndRun("rst4.bin", 0x00);
	EXPECT_EQ(0x02, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, RST_5)
{
	LoadAndRun("rst5.bin", 0x00);
	EXPECT_EQ(0x02, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, RST_6)
{
	LoadAndRun("rst6.bin", 0x00);
	EXPECT_EQ(0x02, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, RST_7)
{
	LoadAndRun("rst7.bin", 0x00);
	EXPECT_EQ(0x02, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, RZ)
{
	LoadAndRun("rz.bin");
	EXPECT_EQ(0x20, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, RET)
{
	LoadAndRun("ret.bin");
	EXPECT_EQ(0x40, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, JZ)
{
	LoadAndRun("jz.bin");
	EXPECT_EQ(0x00, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, true, false);
}

TEST_F(Intel8080Test, CZ)
{
	LoadAndRun("cz.bin");
	EXPECT_EQ(0x10, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, CALL)
{
	LoadAndRun("call.bin");
	EXPECT_EQ(0x20, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, ACI_1)
{
	LoadAndRun("aci1.bin");
	EXPECT_EQ(0x14, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, true, true);
}

TEST_F(Intel8080Test, ACI_2)
{
	LoadAndRun("aci2.bin");
	EXPECT_EQ(0x57, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, false, false, false);
}

TEST_F(Intel8080Test, RNC)
{
	LoadAndRun("rnc.bin");
	EXPECT_EQ(0x10, cpu_->A());
}

TEST_F(Intel8080Test, PUSH_POP_D)
{
	LoadAndRun("pushpopd.bin");
	EXPECT_EQ(0xAB, cpu_->D());
	EXPECT_EQ(0xCD, cpu_->E());
}

TEST_F(Intel8080Test, JNC)
{
	LoadAndRun("jnc.bin");
	EXPECT_EQ(0xFF, cpu_->A());
}

TEST_F(Intel8080Test, OUT)
{
	LoadAndRun("out.bin");
	//Read the contents of device 0
	EXPECT_EQ(0xFF, ioController_->Read(0x00));

	//Reset the value of the device to it's default (0xAA)
	ioController_->Write (0x00, 0xAA);
}

TEST_F(Intel8080Test, CNC)
{
	LoadAndRun("cnc.bin");
	EXPECT_EQ(0xFF, cpu_->A());
}

TEST_F(Intel8080Test, SUI)
{
	LoadAndRun("sui.bin");
	EXPECT_EQ(0xFF, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, true);
}

TEST_F(Intel8080Test, RC)
{
	LoadAndRun("rc.bin");
	EXPECT_EQ(0xFF, cpu_->A());
}

TEST_F(Intel8080Test, JC)
{
	LoadAndRun("jc.bin");
	EXPECT_EQ(0x10, cpu_->A());
}

TEST_F(Intel8080Test, IN)
{
	LoadAndRun("in.bin");
	EXPECT_EQ(0xAA, cpu_->A());
}

TEST_F(Intel8080Test, CC)
{
	LoadAndRun("cc.bin");
	EXPECT_EQ(0x10, cpu_->A());
}

TEST_F(Intel8080Test, SBI)
{
	LoadAndRun("sbi.bin");
	EXPECT_EQ(0xFE, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, false, false, true);
}

TEST_F(Intel8080Test, RPO)
{
	LoadAndRun("rpo.bin");
	EXPECT_EQ(0x10, cpu_->A());
}

TEST_F(Intel8080Test, PUSH_POP_H)
{
	LoadAndRun("pushpoph.bin");
	EXPECT_EQ(0xAB, cpu_->H());
	EXPECT_EQ(0xCD, cpu_->L());
}

TEST_F(Intel8080Test, JPO)
{
	LoadAndRun("jpo.bin");
	EXPECT_EQ(0x11, cpu_->A());
}

TEST_F(Intel8080Test, XTHL)
{
	LoadAndRun("xthl.bin");
	EXPECT_EQ(0xAB, cpu_->H());
	EXPECT_EQ(0xCD, cpu_->L());
}

TEST_F(Intel8080Test, CPO)
{
	LoadAndRun("cpo.bin");
	EXPECT_EQ(0x11, cpu_->A());
}

TEST_F(Intel8080Test, ANI)
{
	LoadAndRun("ani.bin");
	EXPECT_EQ(0x0A, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, RPE)
{
	LoadAndRun("rpe.bin");
	EXPECT_EQ(0x11, cpu_->A());
}

TEST_F(Intel8080Test, PCHL)
{
	LoadAndRun("pchl.bin");
	EXPECT_EQ(0x11, cpu_->A());
}

TEST_F(Intel8080Test, JPE)
{
	LoadAndRun("jpe.bin");
	EXPECT_EQ(0x10, cpu_->A());
}

TEST_F(Intel8080Test, XCHG)
{
	LoadAndRun("xchg.bin");
	EXPECT_EQ(0x01, cpu_->D());
	EXPECT_EQ(0x23, cpu_->E());
	EXPECT_EQ(0xAB, cpu_->H());
	EXPECT_EQ(0xCD, cpu_->L());
}

TEST_F(Intel8080Test, CPE)
{
	LoadAndRun("cpe.bin");
	EXPECT_EQ(0x10, cpu_->A());
}

TEST_F(Intel8080Test, XRI)
{
	LoadAndRun("xri.bin");
	EXPECT_EQ(0xBA, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, false, false, false);
}

TEST_F(Intel8080Test, RP)
{
	LoadAndRun("rp.bin");
	EXPECT_EQ(0x11, cpu_->A());
}

TEST_F(Intel8080Test, PUSH_POP_PSW)
{
	LoadAndRun("pushpoppsw.bin");
	EXPECT_EQ(0x00, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(true, false, true, false, true);
}

TEST_F(Intel8080Test, JP)
{
	LoadAndRun("jp.bin");
	EXPECT_EQ(0x10, cpu_->A());
}

/*
TEST_F(Intel8080Test, DI)
{
	LoadAndRun("di.bin");
}
*/

TEST_F(Intel8080Test, CP)
{
	LoadAndRun("cp.bin");
	EXPECT_EQ(0x10, cpu_->A());
}

TEST_F(Intel8080Test, ORI)
{
	LoadAndRun("ori.bin");
	EXPECT_EQ(0xBF, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, false, false, false);
}

TEST_F(Intel8080Test, RM)
{
	LoadAndRun("rm.bin");
	EXPECT_EQ(0x10, cpu_->A());
}

TEST_F(Intel8080Test, SPHL)
{
	LoadAndRun("sphl.bin");
	EXPECT_EQ(0x0123, cpu_->SP());
}

TEST_F(Intel8080Test, JM)
{
	LoadAndRun("jm.bin");
	EXPECT_EQ(0x11, cpu_->A());
}

/*
TEST_F(Intel8080Test, EI)
{
	LoadAndRun("ei.bin");
}
*/

TEST_F(Intel8080Test, CM)
{
	LoadAndRun("cm.bin");
	EXPECT_EQ(0x11, cpu_->A());
}

TEST_F(Intel8080Test, CPI)
{
	LoadAndRun("cpi.bin");
	EXPECT_EQ(0x4A, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, false, true, false, false);
}

TEST_F(Intel8080Test, CPI0)
{
	LoadAndRun("cpi0.bin");
	EXPECT_EQ(0xF5, cpu_->A());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(false, true, true, false, false);
}

TEST_F(Intel8080Test, CPUDIAG)
{
	//machine_->SetIoController (cpmIoController_);
	ioController_ = cpmIoController_;
	LoadAndRun("cpudiag_patched.bin", 0xEB);
	EXPECT_TRUE(static_pointer_cast<CpmIoController>(cpmIoController_)->Message() == " CPU IS OPERATIONAL");
	ioController_ = testIoController_;
}

TEST_F(Intel8080Test, ISR_1)
{
	auto path = directory_;
	//Load the interrupt service routine at the correct address
	//The ioController fires rst 1 every second.
	memoryController_->Load(path /= "isr.bin", 0x08);

	//There is a chance for an infinate spin if the test fails.
	//Since the ioController fires an interrupt every second we
	//need to run this off an async task with a two second time out.
	auto future = std::async(std::launch::async, [&]
	{
		LoadAndRun("spinUntilIsr.bin");
	});

	EXPECT_EQ(std::future_status::ready, future.wait_for(seconds(2)));
	EXPECT_EQ(0x00, cpu_->A());
	EXPECT_EQ(0x01, cpu_->B());
}
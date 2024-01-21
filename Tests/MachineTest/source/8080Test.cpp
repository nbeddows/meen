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

TEST_F(MachineTest, LXI_B)
{
	auto state = LoadAndRun("lxib.bin");
	EXPECT_EQ(0xFF, state[State::B]);
	EXPECT_EQ(0x12, state[State::C]);
}

TEST_F(MachineTest, STAX_B)
{
	auto state = LoadAndRun("staxb.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, INX_B)
{
	auto state = LoadAndRun("inxb.bin");
	EXPECT_EQ(0xFF, state[State::B]);
	EXPECT_EQ(0x13, state[State::C]);
	//No flags should be changed by this instruction
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, INR_B)
{
	auto state = LoadAndRun("inrb.bin");
	EXPECT_EQ(0x10, state[State::B]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus (state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, DCR_B)
{
	auto state = LoadAndRun("dcrb.bin");
	EXPECT_EQ(0x00, state[State::B]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, MVI_B)
{
	auto state = LoadAndRun("mvib.bin");
	EXPECT_EQ(0x12, state[State::B]);
}

TEST_F(MachineTest, RLC)
{
	auto state = LoadAndRun("rlc.bin");
	EXPECT_EQ(0xE5, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, true);
}

TEST_F(MachineTest, DAD_B)
{
	auto state = LoadAndRun("dadb.bin");
	//Answer in HL
	EXPECT_EQ(0xD5, state[State::H]);
	EXPECT_EQ(0x1A, state[State::L]);
	//BC remains unchanged
	EXPECT_EQ(0x33, state[State::B]);
	EXPECT_EQ(0x9F, state[State::C]);
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, LDAX_B)
{
	auto state = LoadAndRun("ldaxb.bin");
	EXPECT_EQ(0x0A, state[State::A]);
}

TEST_F(MachineTest, DCX_B)
{
	auto state = LoadAndRun("dcxb.bin");
	EXPECT_EQ(0xFF, state[State::B]);
	EXPECT_EQ(0xFF, state[State::C]);
	//No flags should be changed by this instruction
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, INR_C)
{
	auto state = LoadAndRun("inrc.bin");
	EXPECT_EQ(0x10, state[State::C]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, DCR_C)
{
	auto state = LoadAndRun("dcrc.bin");
	EXPECT_EQ(0x00, state[State::C]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, MVI_C)
{
	auto state = LoadAndRun("mvic.bin");
	EXPECT_EQ(0x12, state[State::C]);
}

TEST_F(MachineTest, RRC)
{
	auto state = LoadAndRun("rrc.bin");
	EXPECT_EQ(0x79, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, LXI_D)
{
	auto state = LoadAndRun("lxid.bin");
	EXPECT_EQ(0xFF, state[State::D]);
	EXPECT_EQ(0x12, state[State::E]);
}

TEST_F(MachineTest, STAX_D)
{
	auto state = LoadAndRun("staxd.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, INX_D)
{
	auto state = LoadAndRun("inxd.bin");
	EXPECT_EQ(0xFF, state[State::D]);
	EXPECT_EQ(0x13, state[State::E]);
	//No flags should be changed by this instruction
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, INR_D)
{
	auto state = LoadAndRun("inrd.bin");
	EXPECT_EQ(0x10, state[State::D]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, DCR_D)
{
	auto state = LoadAndRun("dcrd.bin");
	EXPECT_EQ(0x00, state[State::D]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, MVI_D)
{
	auto state = LoadAndRun("mvid.bin");
	EXPECT_EQ(0x12, state[State::D]);
}

TEST_F(MachineTest, RAL)
{
	auto state = LoadAndRun("ral.bin");
	EXPECT_EQ(0x6A, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, true);
}

TEST_F(MachineTest, DAD_D)
{
	auto state = LoadAndRun("dadd.bin");
	//Answer in HL
	EXPECT_EQ(0x00, state[State::H]);
	EXPECT_EQ(0x00, state[State::L]);
	//DE remains unchanged
	EXPECT_EQ(0x00, state[State::D]);
	EXPECT_EQ(0x01, state[State::E]);
	CheckStatus(state[State::S], false, false, false, false, true);
}

TEST_F(MachineTest, LDAX_D)
{
	auto state = LoadAndRun("ldaxd.bin");
	EXPECT_EQ(0x1A, state[State::A]);
}

TEST_F(MachineTest, DCX_D)
{
	auto state = LoadAndRun("dcxd.bin");
	EXPECT_EQ(0xFF, state[State::D]);
	EXPECT_EQ(0xFF, state[State::E]);
	//No flags should be changed by this instruction
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, INR_E)
{
	auto state = LoadAndRun("inre.bin");
	EXPECT_EQ(0x10, state[State::E]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, DCR_E)
{
	auto state = LoadAndRun("dcre.bin");
	EXPECT_EQ(0x00, state[State::E]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, MVI_E)
{
	auto state = LoadAndRun("mvie.bin");
	EXPECT_EQ(0x12, state[State::E]);
}

TEST_F(MachineTest, RAR)
{
	auto state = LoadAndRun("rar.bin");
	EXPECT_EQ(0xB5, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, LXI_H)
{
	auto state = LoadAndRun("lxih.bin");
	EXPECT_EQ(0xFF, state[State::H]);
	EXPECT_EQ(0x12, state[State::L]);
}

TEST_F(MachineTest, SHLD)
{
	auto state = LoadAndRun("shld.bin");
	EXPECT_EQ(0xAB, state[State::A]);
	EXPECT_EQ(0xCD, state[State::B]);
}

TEST_F(MachineTest, INX_H)
{
	auto state = LoadAndRun("inxh.bin");
	EXPECT_EQ(0xFF, state[State::H]);
	EXPECT_EQ(0x13, state[State::L]);
	//No flags should be changed by this instruction
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, INR_H)
{
	auto state = LoadAndRun("inrh.bin");
	EXPECT_EQ(0x10, state[State::H]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, DCR_H)
{
	auto state = LoadAndRun("dcrh.bin");
	EXPECT_EQ(0x00, state[State::H]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, MVI_H)
{
	auto state = LoadAndRun("mvih.bin");
	EXPECT_EQ(0x12, state[State::H]);
}


//Need accumulator mvi, instruction needs more work
TEST_F(MachineTest, DAA)
{
	auto state = LoadAndRun("daa.bin");
	EXPECT_EQ(0x01, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, true);
}


TEST_F(MachineTest, DAD_H)
{
	auto state = LoadAndRun("dadh.bin");
	//Answer in HL
	EXPECT_EQ(0x42, state[State::H]);
	EXPECT_EQ(0xF6, state[State::L]);
	CheckStatus(state[State::S], false, false, false, false, true);
}

TEST_F(MachineTest, LHLD)
{
	auto state = LoadAndRun("lhld.bin");
	EXPECT_EQ(0x7B, state[State::L]);
	EXPECT_EQ(0x2A, state[State::H]);
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, DCX_H)
{

	auto state = LoadAndRun("dcxh.bin");
	EXPECT_EQ(0xFF, state[State::H]);
	EXPECT_EQ(0xFF, state[State::L]);
	//No flags should be changed by this instruction
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, INR_L)
{
	auto state = LoadAndRun("inrl.bin");
	EXPECT_EQ(0x10, state[State::L]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, DCR_L)
{
	auto state = LoadAndRun("dcrl.bin");
	EXPECT_EQ(0x00, state[State::L]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, MVI_L)
{
	auto state = LoadAndRun("mvil.bin");
	EXPECT_EQ(0x12, state[State::L]);
}

TEST_F(MachineTest, CMA)
{
	auto state = LoadAndRun("cma.bin");
	EXPECT_EQ(0x00, state[State::A]);
}

TEST_F(MachineTest, LXI_SP)
{
	auto state = LoadAndRun("lxisp.bin");
	EXPECT_EQ(0xCDAB, (state[State::SP] << 8) | state[State::SP + 1]);
}

TEST_F(MachineTest, STA)
{
	auto state = LoadAndRun("sta.bin");
	EXPECT_EQ(0xFF, state[State::A]);
}

TEST_F(MachineTest, INX_SP)
{
	auto state = LoadAndRun("inxsp.bin");
	EXPECT_EQ(0xCDAC, (state[State::SP] << 8) | state[State::SP + 1]);
}

TEST_F(MachineTest, INR_M)
{
	auto state = LoadAndRun("inrm.bin");
	EXPECT_EQ(0x22, state[State::L]);
	//EXPECT_EQ(0x35, state[State::L]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, false, false);
}

TEST_F(MachineTest, DCR_M)
{
	auto state = LoadAndRun("dcrm.bin");
	EXPECT_EQ(0x20, state[State::L]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, MVI_M)
{
	auto state = LoadAndRun("mvim.bin");
	EXPECT_EQ(0xDD, state[State::L]);
}

TEST_F(MachineTest, STC)
{
	auto state = LoadAndRun("stc.bin");
	CheckStatus(state[State::S], false, false, false, false, true);
}

TEST_F(MachineTest, DAD_SP)
{
	auto state = LoadAndRun("dadsp.bin");
	EXPECT_EQ(0x57, state[State::H]);
	EXPECT_EQ(0x9A, state[State::L]);
	CheckStatus(state[State::S], false, false, false, false, true);
}

TEST_F(MachineTest, LDA)
{
	auto state = LoadAndRun("lda.bin");
	EXPECT_EQ(0x3A, state[State::A]);
}

TEST_F(MachineTest, DCX_SP)
{
	auto state = LoadAndRun("dcxsp.bin");
	EXPECT_EQ(0xCDAA, (state[State::SP] << 8) | state[State::SP + 1]);
}

TEST_F(MachineTest, INR_A)
{
	auto state = LoadAndRun("inra.bin");
	EXPECT_EQ(0x3B, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, true);
}

TEST_F(MachineTest, DCR_A)
{
	//This program starts out by setting the carry bit
	//which tests the fact that inr does not modify the carry bit
	auto state = LoadAndRun("dcra.bin");
	EXPECT_EQ(0x39, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, true);
}

TEST_F(MachineTest, MVI_A)
{
	auto state = LoadAndRun("mvia.bin");
	EXPECT_EQ(0x12, state[State::A]);
}

TEST_F(MachineTest, CMC)
{
	auto state = LoadAndRun("cmc.bin");
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, true);
}

TEST_F(MachineTest, MOV_BB)
{
	auto state = LoadAndRun("movbb.bin");
	EXPECT_EQ(0x21, state[State::B]);
}

TEST_F(MachineTest, MOV_BC)
{
	auto state = LoadAndRun("movbc.bin");
	EXPECT_EQ(0x21, state[State::B]);
}

TEST_F(MachineTest, MOV_BD)
{
	auto state = LoadAndRun("movbd.bin");
	EXPECT_EQ(0x21, state[State::B]);
}

TEST_F(MachineTest, MOV_BE)
{
	auto state = LoadAndRun("movbe.bin");
	EXPECT_EQ(0x21, state[State::B]);
}

TEST_F(MachineTest, MOV_BH)
{
	auto state = LoadAndRun("movbh.bin");
	EXPECT_EQ(0x21, state[State::B]);
}

TEST_F(MachineTest, MOV_BL)
{
	auto state = LoadAndRun("movbl.bin");
	EXPECT_EQ(0x21, state[State::B]);
}

TEST_F(MachineTest, MOV_BM)
{
	auto state = LoadAndRun("movbm.bin");
	EXPECT_EQ(0x21, state[State::B]);
}

TEST_F(MachineTest, MOV_BA)
{
	auto state = LoadAndRun("movba.bin");
	EXPECT_EQ(0x21, state[State::B]);
}

TEST_F(MachineTest, MOV_CB)
{
	auto state = LoadAndRun("movcb.bin");
	EXPECT_EQ(0x21, state[State::C]);
}

TEST_F(MachineTest, MOV_CC)
{
	auto state = LoadAndRun("movcc.bin");
	EXPECT_EQ(0x21, state[State::C]);
}

TEST_F(MachineTest, MOV_CD)
{
	auto state = LoadAndRun("movcd.bin");
	EXPECT_EQ(0x21, state[State::C]);
}

TEST_F(MachineTest, MOV_CE)
{
	auto state = LoadAndRun("movce.bin");
	EXPECT_EQ(0x21, state[State::C]);
}

TEST_F(MachineTest, MOV_CH)
{
	auto state = LoadAndRun("movch.bin");
	EXPECT_EQ(0x21, state[State::C]);
}

TEST_F(MachineTest, MOV_CL)
{
	auto state = LoadAndRun("movcl.bin");
	EXPECT_EQ(0x21, state[State::C]);
}

TEST_F(MachineTest, MOV_CM)
{
	auto state = LoadAndRun("movcm.bin");
	EXPECT_EQ(0x21, state[State::C]);
}

TEST_F(MachineTest, MOV_CA)
{
	auto state = LoadAndRun("movca.bin");
	EXPECT_EQ(0x21, state[State::C]);
}

TEST_F(MachineTest, MOV_DB)
{
	auto state = LoadAndRun("movdb.bin");
	EXPECT_EQ(0x21, state[State::D]);
}

TEST_F(MachineTest, MOV_DC)
{
	auto state = LoadAndRun("movdc.bin");
	EXPECT_EQ(0x21, state[State::D]);
}

TEST_F(MachineTest, MOV_DD)
{
	auto state = LoadAndRun("movdd.bin");
	EXPECT_EQ(0x21, state[State::D]);
}

TEST_F(MachineTest, MOV_DE)
{
	auto state = LoadAndRun("movde.bin");
	EXPECT_EQ(0x21, state[State::D]);
}

TEST_F(MachineTest, MOV_DH)
{
	auto state = LoadAndRun("movdh.bin");
	EXPECT_EQ(0x21, state[State::D]);
}

TEST_F(MachineTest, MOV_DL)
{
	auto state = LoadAndRun("movdl.bin");
	EXPECT_EQ(0x21, state[State::D]);
}

TEST_F(MachineTest, MOV_DM)
{
	auto state = LoadAndRun("movdm.bin");
	EXPECT_EQ(0x21, state[State::D]);
}

TEST_F(MachineTest, MOV_DA)
{
	auto state = LoadAndRun("movda.bin");
	EXPECT_EQ(0x21, state[State::D]);
}

TEST_F(MachineTest, MOV_EB)
{
	auto state = LoadAndRun("moveb.bin");
	EXPECT_EQ(0x21, state[State::E]);
}

TEST_F(MachineTest, MOV_EC)
{
	auto state = LoadAndRun("movec.bin");
	EXPECT_EQ(0x21, state[State::E]);
}

TEST_F(MachineTest, MOV_ED)
{
	auto state = LoadAndRun("moved.bin");
	EXPECT_EQ(0x21, state[State::E]);
}

TEST_F(MachineTest, MOV_EE)
{
	auto state = LoadAndRun("movee.bin");
	EXPECT_EQ(0x21, state[State::E]);
}

TEST_F(MachineTest, MOV_EH)
{
	auto state = LoadAndRun("moveh.bin");
	EXPECT_EQ(0x21, state[State::E]);
}

TEST_F(MachineTest, MOV_EL)
{
	auto state = LoadAndRun("movel.bin");
	EXPECT_EQ(0x21, state[State::E]);
}

TEST_F(MachineTest, MOV_EM)
{
	auto state = LoadAndRun("movem.bin");
	EXPECT_EQ(0x21, state[State::E]);
}

TEST_F(MachineTest, MOV_EA)
{
	auto state = LoadAndRun("movea.bin");
	EXPECT_EQ(0x21, state[State::E]);
}

TEST_F(MachineTest, MOV_HB)
{
	auto state = LoadAndRun("movhb.bin");
	EXPECT_EQ(0x21, state[State::H]);
}

TEST_F(MachineTest, MOV_HC)
{
	auto state = LoadAndRun("movhc.bin");
	EXPECT_EQ(0x21, state[State::H]);
}

TEST_F(MachineTest, MOV_HD)
{
	auto state = LoadAndRun("movhd.bin");
	EXPECT_EQ(0x21, state[State::H]);
}

TEST_F(MachineTest, MOV_HE)
{
	auto state = LoadAndRun("movhe.bin");
	EXPECT_EQ(0x21, state[State::H]);
}

TEST_F(MachineTest, MOV_HH)
{
	auto state = LoadAndRun("movhh.bin");
	EXPECT_EQ(0x21, state[State::H]);
}

TEST_F(MachineTest, MOV_HL)
{
	auto state = LoadAndRun("movhl.bin");
	EXPECT_EQ(0x21, state[State::H]);
}

TEST_F(MachineTest, MOV_HM)
{
	auto state = LoadAndRun("movhm.bin");
	EXPECT_EQ(0x21, state[State::H]);
}

TEST_F(MachineTest, MOV_HA)
{
	auto state = LoadAndRun("movha.bin");
	EXPECT_EQ(0x21, state[State::H]);
}

TEST_F(MachineTest, MOV_LB)
{
	auto state = LoadAndRun("movlb.bin");
	EXPECT_EQ(0x21, state[State::L]);
}

TEST_F(MachineTest, MOV_LC)
{
	auto state = LoadAndRun("movlc.bin");
	EXPECT_EQ(0x21, state[State::L]);
}

TEST_F(MachineTest, MOV_LD)
{
	auto state = LoadAndRun("movld.bin");
	EXPECT_EQ(0x21, state[State::L]);
}

TEST_F(MachineTest, MOV_LE)
{
	auto state = LoadAndRun("movle.bin");
	EXPECT_EQ(0x21, state[State::L]);
}

TEST_F(MachineTest, MOV_LH)
{
	auto state = LoadAndRun("movlh.bin");
	EXPECT_EQ(0x21, state[State::L]);
}

TEST_F(MachineTest, MOV_LL)
{
	auto state = LoadAndRun("movll.bin");
	EXPECT_EQ(0x21, state[State::L]);
}

TEST_F(MachineTest, MOV_LM)
{
	auto state = LoadAndRun("movlm.bin");
	EXPECT_EQ(0x21, state[State::L]);
}

TEST_F(MachineTest, MOV_LA)
{
	auto state = LoadAndRun("movla.bin");
	EXPECT_EQ(0x21, state[State::L]);
}

//Using MovA for the memory tests since
//we store the result in the accumulator
TEST_F(MachineTest, MOV_MB)
{
	auto state = LoadAndRun("movmb.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, MOV_MC)
{
	auto state = LoadAndRun("movmc.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, MOV_MD)
{
	auto state = LoadAndRun("movmd.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, MOV_ME)
{
	auto state = LoadAndRun("movme.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

/*
//These two tests need to be rewritten since
//the registers to test are being overwritten
//by the program

TEST_F(MachineTest, MOV_MH)
{
	LoadAndRun("./programs/movmh.bin");
}

TEST_F(MachineTest, MOV_ML)
{
	LoadAndRun("./programs/movml.bin");
}
*/

//TEST_F(MachineTest, HLT)
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

TEST_F(MachineTest, MOV_MA)
{
	auto state = LoadAndRun("movma.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, MOV_AB)
{
	auto state = LoadAndRun("movab.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, MOV_AC)
{
	auto state = LoadAndRun("movac.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, MOV_AD)
{
	auto state = LoadAndRun("movad.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, MOV_AE)
{
	auto state = LoadAndRun("movae.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, MOV_AH)
{
	auto state = LoadAndRun("movah.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, MOV_AL)
{
	auto state = LoadAndRun("moval.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, MOV_AM)
{
	auto state = LoadAndRun("movam.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, MOV_AA)
{
	auto state = LoadAndRun("movaa.bin");
	EXPECT_EQ(0x21, state[State::A]);
}

TEST_F(MachineTest, MovIncDec)
{
	auto state = LoadAndRun("movIncDec.bin");
	EXPECT_EQ(0x77, state[State::A]);
}

TEST_F(MachineTest, ADD_B)
{
	auto state = LoadAndRun("addb.bin");
	EXPECT_EQ(0x9A, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, true, false);
}

TEST_F(MachineTest, ADD_C)
{
	auto state = LoadAndRun("addc.bin");
	EXPECT_EQ(0x9A, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, true, false);
}

TEST_F(MachineTest, ADD_D)
{
	auto state = LoadAndRun("addd.bin");
	EXPECT_EQ(0x9A, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, true, false);
}

TEST_F(MachineTest, ADD_E)
{
	auto state = LoadAndRun("adde.bin");
	EXPECT_EQ(0x9A, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, true, false);
}

TEST_F(MachineTest, ADD_H)
{
	auto state = LoadAndRun("addh.bin");
	EXPECT_EQ(0x9A, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, true, false);
}

TEST_F(MachineTest, ADD_L)
{
	auto state = LoadAndRun("addl.bin");
	EXPECT_EQ(0x9A, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, true, false);
}

TEST_F(MachineTest, ADD_M)
{
	auto state = LoadAndRun("addm.bin");
	EXPECT_EQ(0xD8, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, true, false);
}

TEST_F(MachineTest, ADD_A)
{
	auto state = LoadAndRun("adda.bin");
	EXPECT_EQ(0xD8, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, true, false);
}

TEST_F(MachineTest, ADC_B)
{
	auto state = LoadAndRun("adcb.bin");
	EXPECT_EQ(0x80, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, false, true, false);
}

TEST_F(MachineTest, ADC_C)
{
	auto state = LoadAndRun("adcc.bin");
	EXPECT_EQ(0x80, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, false, true, false);
}

TEST_F(MachineTest, ADC_D)
{
	auto state = LoadAndRun("adcd.bin");
	EXPECT_EQ(0x80, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, false, true, false);
}

TEST_F(MachineTest, ADC_E)
{
	auto state = LoadAndRun("adce.bin");
	EXPECT_EQ(0x80, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, false, true, false);
}

TEST_F(MachineTest, ADC_H)
{
	auto state = LoadAndRun("adch.bin");
	EXPECT_EQ(0x80, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, false, true, false);
}

TEST_F(MachineTest, ADC_L)
{
	auto state = LoadAndRun("adcl.bin");
	EXPECT_EQ(0x80, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, false, true, false);
}

TEST_F(MachineTest, ADC_M)
{
	auto state = LoadAndRun("adcm.bin");
	EXPECT_EQ(0x85, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, false, false, false);
}

TEST_F(MachineTest, ADC_A)
{
	auto state = LoadAndRun("adca.bin");
	EXPECT_EQ(0x85, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, false, false, false);
}

TEST_F(MachineTest, SUB_B)
{
	auto state = LoadAndRun("subb.bin");
	EXPECT_EQ(0x00, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, SUB_C)
{
	auto state = LoadAndRun("subc.bin");
	EXPECT_EQ(0x00, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, SUB_D)
{
	auto state = LoadAndRun("subd.bin");
	EXPECT_EQ(0x00, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, SUB_E)
{
	auto state = LoadAndRun("sube.bin");
	EXPECT_EQ(0x00, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, SUB_H)
{
	auto state = LoadAndRun("subh.bin");
	EXPECT_EQ(0x00, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, SUB_L)
{
	auto state = LoadAndRun("subl.bin");
	EXPECT_EQ(0x00, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, SUB_M)
{
	auto state = LoadAndRun("subm.bin");
	EXPECT_EQ(0x00, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, SUB_A)
{
	auto state = LoadAndRun("suba.bin");
	EXPECT_EQ(0x00, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, SBB_B)
{
	auto state = LoadAndRun("sbbb.bin");
	EXPECT_EQ(0x01, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, SBB_C)
{
	auto state = LoadAndRun("sbbc.bin");
	EXPECT_EQ(0x01, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, SBB_D)
{
	auto state = LoadAndRun("sbbd.bin");
	EXPECT_EQ(0x01, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, SBB_E)
{
	auto state = LoadAndRun("sbbe.bin");
	EXPECT_EQ(0x01, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, SBB_H)
{
	auto state = LoadAndRun("sbbh.bin");
	EXPECT_EQ(0x01, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, SBB_L)
{
	auto state = LoadAndRun("sbbl.bin");
	EXPECT_EQ(0x01, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, SBB_M)
{
	auto state = LoadAndRun("sbbm.bin");
	EXPECT_EQ(0xFF, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, false, true);
}

TEST_F(MachineTest, SBB_A)
{
	auto state = LoadAndRun("sbba.bin");
	EXPECT_EQ(0xFF, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, false, true);
}

TEST_F(MachineTest, ANA_B)
{
	auto state = LoadAndRun("anab.bin");
	EXPECT_EQ(0x0C, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, ANA_C)
{
	auto state = LoadAndRun("anac.bin");
	EXPECT_EQ(0x0C, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, ANA_D)
{
	auto state = LoadAndRun("anad.bin");
	EXPECT_EQ(0x0C, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, ANA_E)
{
	auto state = LoadAndRun("anae.bin");
	EXPECT_EQ(0x0C, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, ANA_H)
{
	auto state = LoadAndRun("anah.bin");
	EXPECT_EQ(0x0C, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, ANA_L)
{
	auto state = LoadAndRun("anal.bin");
	EXPECT_EQ(0x0C, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, ANA_M)
{
	auto state = LoadAndRun("anam.bin");
	EXPECT_EQ(0x20, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, true, false);
}

TEST_F(MachineTest, ANA_A)
{
	auto state = LoadAndRun("anaa.bin");
	EXPECT_EQ(0xFC, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, true, false);
}

TEST_F(MachineTest, XRA_B)
{
	auto state = LoadAndRun("xrab.bin");
	EXPECT_EQ(0xF0, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, false, false);
}

TEST_F(MachineTest, XRA_C)
{
	auto state = LoadAndRun("xrac.bin");
	EXPECT_EQ(0xF0, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, false, false);
}

TEST_F(MachineTest, XRA_D)
{
	auto state = LoadAndRun("xrad.bin");
	EXPECT_EQ(0xF0, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, false, false);
}

TEST_F(MachineTest, XRA_E)
{
	auto state = LoadAndRun("xrae.bin");
	EXPECT_EQ(0xF0, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, false, false);
}

TEST_F(MachineTest, XRA_H)
{
	auto state = LoadAndRun("xrah.bin");
	EXPECT_EQ(0xF0, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, false, false);
}

TEST_F(MachineTest, XRA_L)
{
	auto state = LoadAndRun("xral.bin");
	EXPECT_EQ(0xF0, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, false, false);
}

TEST_F(MachineTest, XRA_M)
{
	auto state = LoadAndRun("xram.bin");
	EXPECT_EQ(0xDE, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, false, false);
}

TEST_F(MachineTest, XRA_A)
{
	auto state = LoadAndRun("xraa.bin");
	EXPECT_EQ(0x00, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, false, false);
}

TEST_F(MachineTest, ORA_B)
{
	auto state = LoadAndRun("orab.bin");
	EXPECT_EQ(0x3F, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, false, false);
}

TEST_F(MachineTest, ORA_C)
{
	auto state = LoadAndRun("orac.bin");
	EXPECT_EQ(0x3F, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, false, false);
}

TEST_F(MachineTest, ORA_D)
{
	auto state = LoadAndRun("orad.bin");
	EXPECT_EQ(0x3F, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, false, false);
}

TEST_F(MachineTest, ORA_E)
{
	auto state = LoadAndRun("orae.bin");
	EXPECT_EQ(0x3F, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, false, false);
}

TEST_F(MachineTest, ORA_H)
{
	auto state = LoadAndRun("orah.bin");
	EXPECT_EQ(0x3F, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, false, false);
}

TEST_F(MachineTest, ORA_L)
{
	auto state = LoadAndRun("oral.bin");
	EXPECT_EQ(0x3F, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, false, false);
}

TEST_F(MachineTest, ORA_M)
{
	auto state = LoadAndRun("oram.bin");
	EXPECT_EQ(0xFF, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, false, false);
}

TEST_F(MachineTest, ORA_A)
{
	auto state = LoadAndRun("oraa.bin");
	EXPECT_EQ(0xFC, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, false, false);
}

TEST_F(MachineTest, CMP_B)
{
	auto state = LoadAndRun("cmpb.bin");
	EXPECT_EQ(0x0A, state[State::A]);
	EXPECT_EQ(0x05, state[State::B]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, CMP_C)
{
	auto state = LoadAndRun("cmpc.bin");
	EXPECT_EQ(0x0A, state[State::A]);
	EXPECT_EQ(0x05, state[State::C]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, CMP_D)
{
	auto state = LoadAndRun("cmpd.bin");
	EXPECT_EQ(0x0A, state[State::A]);
	EXPECT_EQ(0x05, state[State::D]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, CMP_E)
{
	auto state = LoadAndRun("cmpe.bin");
	EXPECT_EQ(0x0A, state[State::A]);
	EXPECT_EQ(0x05, state[State::E]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, CMP_H)
{
	auto state = LoadAndRun("cmph.bin");
	EXPECT_EQ(0x0A, state[State::A]);
	EXPECT_EQ(0x05, state[State::H]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, CMP_L)
{
	auto state = LoadAndRun("cmpl.bin");
	EXPECT_EQ(0x0A, state[State::A]);
	EXPECT_EQ(0x05, state[State::L]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, CMP_M)
{
	auto state = LoadAndRun("cmpm.bin");
	//The contents of the accumulator and the memory must not change.
	EXPECT_EQ(0x0A, state[State::A]);
	EXPECT_EQ(0x21, memoryController_->Read(0x0102));

	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, false, true, true);
}

/*
	Need to fix git settings, seems to be adding a carraige return to this
	test which causes it to fail
*/
#ifdef ENABLE_CMP_A
TEST_F(MachineTest, CMP_A)
{
	auto state = LoadAndRun("cmpa.bin");
	EXPECT_EQ(0x0A, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}
#endif

TEST_F(MachineTest, RNZ)
{
	auto state = LoadAndRun("rnz.bin");
	EXPECT_EQ(0x00, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, PUSH_POP_B)
{
	auto state = LoadAndRun("pushpopb.bin");
	EXPECT_EQ(0xAB, state[State::B]);
	EXPECT_EQ(0xCD, state[State::C]);
}

TEST_F(MachineTest, JNZ)
{
	auto state = LoadAndRun("jnz.bin");
	EXPECT_EQ(0xFF, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, false, false);
}

TEST_F(MachineTest, JMP)
{
	auto state = LoadAndRun("jmp.bin");
	EXPECT_EQ(0x3F, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, false, false);
}

TEST_F(MachineTest, CNZ)
{
	auto state = LoadAndRun("cnz.bin");
	EXPECT_EQ(0x30, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, false, false);
}

TEST_F(MachineTest, ADI_1)
{
	auto state = LoadAndRun("adi1.bin");
	EXPECT_EQ(0x56, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, false, false);
}

TEST_F(MachineTest, ADI_2)
{
	auto state = LoadAndRun("adi2.bin");
	EXPECT_EQ(0x14, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, true);
}

TEST_F(MachineTest, RST_0)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x00);
	auto state = LoadAndRun("rst0.bin");
	EXPECT_EQ(0x02, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, RST_1)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x08);
	auto state = LoadAndRun("rst1.bin");
	EXPECT_EQ(0x02, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, RST_2)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x10);
	auto state = LoadAndRun("rst2.bin");
	EXPECT_EQ(0x02, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, RST_3)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x18);
	auto state = LoadAndRun("rst3.bin");
	EXPECT_EQ(0x02, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, RST_4)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x20);
	auto state = LoadAndRun("rst4.bin");
	EXPECT_EQ(0x02, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, RST_5)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x28);
	auto state = LoadAndRun("rst5.bin");
	EXPECT_EQ(0x02, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, RST_6)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x30);
	auto state = LoadAndRun("rst6.bin");
	EXPECT_EQ(0x02, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, RST_7)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x38);
	auto state = LoadAndRun("rst7.bin");
	EXPECT_EQ(0x02, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, RZ)
{
	auto state = LoadAndRun("rz.bin");
	EXPECT_EQ(0x20, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, RET)
{
	auto state = LoadAndRun("ret.bin");
	EXPECT_EQ(0x40, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, JZ)
{
	auto state = LoadAndRun("jz.bin");
	EXPECT_EQ(0x00, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, CZ)
{
	auto state = LoadAndRun("cz.bin");
	EXPECT_EQ(0x10, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, CALL)
{
	auto state = LoadAndRun("call.bin");
	EXPECT_EQ(0x20, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, ACI_1)
{
	auto state = LoadAndRun("aci1.bin");
	EXPECT_EQ(0x14, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, true);
}

TEST_F(MachineTest, ACI_2)
{
	auto state = LoadAndRun("aci2.bin");
	EXPECT_EQ(0x57, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, false, false, false);
}

TEST_F(MachineTest, RNC)
{
	auto state = LoadAndRun("rnc.bin");
	EXPECT_EQ(0x10, state[State::A]);
}

TEST_F(MachineTest, PUSH_POP_D)
{
	auto state = LoadAndRun("pushpopd.bin");
	EXPECT_EQ(0xAB, state[State::D]);
	EXPECT_EQ(0xCD, state[State::E]);
}

TEST_F(MachineTest, JNC)
{
	auto state = LoadAndRun("jnc.bin");
	EXPECT_EQ(0xFF, state[State::A]);
}

TEST_F(MachineTest, OUT)
{
	auto state = LoadAndRun("out.bin");
	//Read the contents of device 0
	EXPECT_EQ(0xFF, testIoController_->Read(0x00));
	//Reset the value of the device to it's default (0xAA)
	testIoController_->Write (0x00, 0xAA);
}

TEST_F(MachineTest, CNC)
{
	auto state = LoadAndRun("cnc.bin");
	EXPECT_EQ(0xFF, state[State::A]);
}

TEST_F(MachineTest, SUI)
{
	auto state = LoadAndRun("sui.bin");
	EXPECT_EQ(0xFF, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, false, true);
}

TEST_F(MachineTest, RC)
{
	auto state = LoadAndRun("rc.bin");
	EXPECT_EQ(0xFF, state[State::A]);
}

TEST_F(MachineTest, JC)
{
	auto state = LoadAndRun("jc.bin");
	EXPECT_EQ(0x10, state[State::A]);
}

TEST_F(MachineTest, IN)
{
	auto state = LoadAndRun("in.bin");
	EXPECT_EQ(0xAA, state[State::A]);
}

TEST_F(MachineTest, CC)
{
	auto state = LoadAndRun("cc.bin");
	EXPECT_EQ(0x10, state[State::A]);
}

TEST_F(MachineTest, SBI)
{
	auto state = LoadAndRun("sbi.bin");
	EXPECT_EQ(0xFE, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, false, false, true);
}

TEST_F(MachineTest, RPO)
{
	auto state = LoadAndRun("rpo.bin");
	EXPECT_EQ(0x10, state[State::A]);
}

TEST_F(MachineTest, PUSH_POP_H)
{
	auto state = LoadAndRun("pushpoph.bin");
	EXPECT_EQ(0xAB, state[State::H]);
	EXPECT_EQ(0xCD, state[State::L]);
}

TEST_F(MachineTest, JPO)
{
	auto state = LoadAndRun("jpo.bin");
	EXPECT_EQ(0x11, state[State::A]);
}

TEST_F(MachineTest, XTHL)
{
	auto state = LoadAndRun("xthl.bin");
	EXPECT_EQ(0xAB, state[State::H]);
	EXPECT_EQ(0xCD, state[State::L]);
}

TEST_F(MachineTest, CPO)
{
	auto state = LoadAndRun("cpo.bin");
	EXPECT_EQ(0x11, state[State::A]);
}

TEST_F(MachineTest, ANI)
{
	auto state = LoadAndRun("ani.bin");
	EXPECT_EQ(0x0A, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, RPE)
{
	auto state = LoadAndRun("rpe.bin");
	EXPECT_EQ(0x11, state[State::A]);
}

TEST_F(MachineTest, PCHL)
{
	auto state = LoadAndRun("pchl.bin");
	EXPECT_EQ(0x11, state[State::A]);
}

TEST_F(MachineTest, JPE)
{
	auto state = LoadAndRun("jpe.bin");
	EXPECT_EQ(0x10, state[State::A]);
}

TEST_F(MachineTest, XCHG)
{
	auto state = LoadAndRun("xchg.bin");
	EXPECT_EQ(0x01, state[State::D]);
	EXPECT_EQ(0x23, state[State::E]);
	EXPECT_EQ(0xAB, state[State::H]);
	EXPECT_EQ(0xCD, state[State::L]);
}

TEST_F(MachineTest, CPE)
{
	auto state = LoadAndRun("cpe.bin");
	EXPECT_EQ(0x10, state[State::A]);
}

TEST_F(MachineTest, XRI)
{
	auto state = LoadAndRun("xri.bin");
	EXPECT_EQ(0xBA, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, false, false, false);
}

TEST_F(MachineTest, RP)
{
	auto state = LoadAndRun("rp.bin");
	EXPECT_EQ(0x11, state[State::A]);
}

TEST_F(MachineTest, PUSH_POP_PSW)
{
	auto state = LoadAndRun("pushpoppsw.bin");
	EXPECT_EQ(0x00, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], true, false, true, true, false);
}

TEST_F(MachineTest, JP)
{
	auto state = LoadAndRun("jp.bin");
	EXPECT_EQ(0x10, state[State::A]);
}

/*
TEST_F(MachineTest, DI)
{
	LoadAndRun("di.bin");
}
*/

TEST_F(MachineTest, CP)
{
	auto state = LoadAndRun("cp.bin");
	EXPECT_EQ(0x10, state[State::A]);
}

TEST_F(MachineTest, ORI)
{
	auto state = LoadAndRun("ori.bin");
	EXPECT_EQ(0xBF, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, false, false, false);
}

TEST_F(MachineTest, RM)
{
	auto state = LoadAndRun("rm.bin");
	EXPECT_EQ(0x10, state[State::A]);
}

TEST_F(MachineTest, SPHL)
{
	auto state = LoadAndRun("sphl.bin");
	EXPECT_EQ(0x0123, (state[State::SP] << 8) | state[State::SP + 1]);
}

TEST_F(MachineTest, JM)
{
	auto state = LoadAndRun("jm.bin");
	EXPECT_EQ(0x11, state[State::A]);
}

/*
TEST_F(MachineTest, EI)
{
	auto state = LoadAndRun("ei.bin");
}
*/

TEST_F(MachineTest, CM)
{
	auto state = LoadAndRun("cm.bin");
	EXPECT_EQ(0x11, state[State::A]);
}

TEST_F(MachineTest, CPI)
{
	auto state = LoadAndRun("cpi.bin");
	EXPECT_EQ(0x4A, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, false, true, true, false);
}

TEST_F(MachineTest, CPI0)
{
	auto state = LoadAndRun("cpi0.bin");
	EXPECT_EQ(0xF5, state[State::A]);
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state[State::S], false, true, true, true, false);
}

TEST_F(MachineTest, ISR_1)
{
	//Load the interrupt service routine at the correct address
	//The ioController fires rst 1 every second.
	memoryController_->Load(PROGRAMS_DIR"/isr.bin", 0x08);

	// Set to the resolution to 50Hz
	machine_->SetClockResolution(20000000);

	// There is a chance for an infinate spin if the test fails.
	// TODO: since we fire every second we need to test that the Run
	// method takes a second to complete, pass back the Run value
	// from LoadAndRun or pass it in to LoadAndRun as a reference
	// parameter to be set.
	auto state = LoadAndRun("spinUntilIsr.bin");

	EXPECT_EQ(0x00, state[State::A]);
	EXPECT_EQ(0x01, state[State::B]);

	// restore back to as fast as possible
	EXPECT_EQ(ErrorCode::NoError, machine_->SetClockResolution(-1));
}

TEST_F(MachineTest, Tst8080)
{
	//CP/M Warm Boot is at memory address 0x00, this will be
	//emulated with the exitTest subroutine.
	memoryController_->Load(PROGRAMS_DIR"/exitTest.bin", 0x00);
	//CP/M BDOS print message system call is at memory address 0x05,
	//this will be emulated with the bdosMsg subroutine.
	memoryController_->Load(PROGRAMS_DIR"/bdosMsg.bin", 0x05);
	// use the cpm io controller for cpm based tests
	machine_->SetIoController(cpmIoController_);
	auto state = LoadAndRun("TST8080.COM");
	EXPECT_EQ(74, static_pointer_cast<CpmIoController>(cpmIoController_)->Message().find ("CPU IS OPERATIONAL"));
	// restore the default io controller
	machine_->SetIoController(testIoController_);
}

TEST_F(MachineTest, 8080Pre)
{
	//CP/M Warm Boot is at memory address 0x00, this will be
	//emulated with the exitTest subroutine.
	memoryController_->Load(PROGRAMS_DIR"/exitTest.bin", 0x00);
	//CP/M BDOS print message system call is at memory address 0x05,
	//this will be emulated with the bdosMsg subroutine.
	memoryController_->Load(PROGRAMS_DIR"/bdosMsg.bin", 0x05);
	// use the cpm io controller for cpm based tests
	machine_->SetIoController(cpmIoController_);
	auto state = LoadAndRun("8080PRE.COM");
	EXPECT_EQ(0, static_pointer_cast<CpmIoController>(cpmIoController_)->Message().find("8080 Preliminary tests complete"));
	// restore the default io controller
	machine_->SetIoController(testIoController_);
}

TEST_F(MachineTest, CpuTest)
{
	//CP/M Warm Boot is at memory address 0x00, this will be
	//emulated with the exitTest subroutine.
	memoryController_->Load(PROGRAMS_DIR"/exitTest.bin", 0x00);
	//CP/M BDOS print message system call is at memory address 0x05,
	//this will be emulated with the bdosMsg subroutine.
	memoryController_->Load(PROGRAMS_DIR"/bdosMsg.bin", 0x05);
	// use the cpm io controller for cpm based tests
	machine_->SetIoController(cpmIoController_);
	auto state = LoadAndRun("CPUTEST.COM");
	EXPECT_EQ(168, static_pointer_cast<CpmIoController>(cpmIoController_)->Message().find("CPU TESTS OK"));
	// restore the default io controller
	machine_->SetIoController(testIoController_);
}

TEST_F(MachineTest, 8080Exm)
{
	//CP/M Warm Boot is at memory address 0x00, this will be
	//emulated with the exitTest subroutine.
	memoryController_->Load(PROGRAMS_DIR"/exitTest.bin", 0x00);
	//CP/M BDOS print message system call is at memory address 0x05,
	//this will be emulated with the bdosMsg subroutine.
	memoryController_->Load(PROGRAMS_DIR"/bdosMsg.bin", 0x05);
	// use the cpm io controller for cpm based tests
	machine_->SetIoController(cpmIoController_);
	auto state = LoadAndRun("8080EXM.COM");
	EXPECT_EQ(static_pointer_cast<CpmIoController>(cpmIoController_)->Message().find("ERROR"), std::string::npos);
	// restore the default io controller
	machine_->SetIoController(testIoController_);
}
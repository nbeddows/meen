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

TEST_F(MachineTest, LXI_B)
{
	LoadAndRun("lxib.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":255,"c":18,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, STAX_B)
{
	LoadAndRun("staxb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":4,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INX_B)
{
	LoadAndRun("inxb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":255,"c":19,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INR_B)
{
	LoadAndRun("inrb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":16,"c":0,"d":0,"e":0,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_B)
{
	LoadAndRun("dcrb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_B)
{
	LoadAndRun("mvib.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":18,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RLC)
{
	LoadAndRun("rlc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":229,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DAD_B)
{
	LoadAndRun("dadb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":51,"c":159,"d":0,"e":0,"h":213,"l":26,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, LDAX_B)
{
	LoadAndRun("ldaxb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":1,"c":3,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCX_B)
{
	LoadAndRun("dcxb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":255,"c":255,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INR_C)
{
	LoadAndRun("inrc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":16,"d":0,"e":0,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_C)
{
	LoadAndRun("dcrc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_C)
{
	LoadAndRun("mvic.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":18,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RRC)
{
	LoadAndRun("rrc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":121,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, LXI_D)
{
	LoadAndRun("lxid.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":255,"e":18,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, STAX_D)
{
	LoadAndRun("staxd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":4,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INX_D)
{
	LoadAndRun("inxd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":255,"e":19,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INR_D)
{
	LoadAndRun("inrd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":16,"e":0,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_D)
{
	LoadAndRun("dcrd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_D)
{
	LoadAndRun("mvid.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":18,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RAL)
{
	LoadAndRun("ral.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":106,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DAD_D)
{
	LoadAndRun("dadd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":1,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, LDAX_D)
{
	LoadAndRun("ldaxd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":26,"b":0,"c":0,"d":1,"e":3,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCX_D)
{
	LoadAndRun("dcxd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":255,"e":255,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INR_E)
{
	LoadAndRun("inre.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":16,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_E)
{
	LoadAndRun("dcre.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_E)
{
	LoadAndRun("mvie.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":18,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RAR)
{
	LoadAndRun("rar.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":181,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, LXI_H)
{
	LoadAndRun("lxih.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":255,"l":18,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SHLD)
{
	LoadAndRun("shld.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":171,"b":205,"c":0,"d":0,"e":0,"h":171,"l":205,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INX_H)
{
	LoadAndRun("inxh.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":255,"l":19,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INR_H)
{
	LoadAndRun("inrh.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":16,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_H)
{
	LoadAndRun("dcrh.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_H)
{
	LoadAndRun("mvih.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":18,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DAA)
{
	LoadAndRun("daa.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":19},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DAD_H)
{
	LoadAndRun("dadh.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":66,"l":246,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, LHLD)
{
	LoadAndRun("lhld.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":123,"c":161,"d":0,"e":0,"h":42,"l":123,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCX_H)
{
	LoadAndRun("dcxh.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":255,"l":255,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INR_L)
{
	LoadAndRun("inrl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":16,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_L)
{
	LoadAndRun("dcrl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_L)
{
	LoadAndRun("mvil.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":18,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMA)
{
	LoadAndRun("cma.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, LXI_SP)
{
	LoadAndRun("lxisp.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":52651})");
}

TEST_F(MachineTest, STA)
{
	LoadAndRun("sta.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INX_SP)
{
	LoadAndRun("inxsp.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":52652})");
}

TEST_F(MachineTest, INR_M)
{
	LoadAndRun("inrm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":34,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_M)
{
	LoadAndRun("dcrm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":32,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_M)
{
	LoadAndRun("mvim.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":221,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, STC)
{
	LoadAndRun("stc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DAD_SP)
{
	LoadAndRun("dadsp.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":87,"l":154,"s":3},"pc":2,"sp":43981})");
}

TEST_F(MachineTest, LDA)
{
	LoadAndRun("lda.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":58,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCX_SP)
{
	LoadAndRun("dcxsp.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":52650})");
}

TEST_F(MachineTest, INR_A)
{
	LoadAndRun("inra.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":59,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

// The inr/dcr programs start out by setting the carry bit
// which tests the fact that the inr/dcr programs don't modify the carry bit
TEST_F(MachineTest, DCR_A)
{
	LoadAndRun("dcra.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":57,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":23},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_A)
{
	LoadAndRun("mvia.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":18,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMC)
{
	LoadAndRun("cmc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BB)
{
	LoadAndRun("movbb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BC)
{
	LoadAndRun("movbc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":33,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BD)
{
	LoadAndRun("movbd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BE)
{
	LoadAndRun("movbe.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BH)
{
	LoadAndRun("movbh.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BL)
{
	LoadAndRun("movbl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BM)
{
	LoadAndRun("movbm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":0,"h":1,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BA)
{
	LoadAndRun("movba.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":33,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CB)
{
	LoadAndRun("movcb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":33,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CC)
{
	LoadAndRun("movcc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CD)
{
	LoadAndRun("movcd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CE)
{
	LoadAndRun("movce.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CH)
{
	LoadAndRun("movch.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CL)
{
	LoadAndRun("movcl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CM)
{
	LoadAndRun("movcm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":0,"h":1,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CA)
{
	LoadAndRun("movca.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":33,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DB)
{
	LoadAndRun("movdb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DC)
{
	LoadAndRun("movdc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DD)
{
	LoadAndRun("movdd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DE)
{
	LoadAndRun("movde.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DH)
{
	LoadAndRun("movdh.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DL)
{
	LoadAndRun("movdl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DM)
{
	LoadAndRun("movdm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":0,"h":1,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DA)
{
	LoadAndRun("movda.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EB)
{
	LoadAndRun("moveb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EC)
{
	LoadAndRun("movec.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_ED)
{
	LoadAndRun("moved.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EE)
{
	LoadAndRun("movee.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EH)
{
	LoadAndRun("moveh.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":33,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EL)
{
	LoadAndRun("movel.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":33,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EM)
{
	LoadAndRun("movem.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":33,"h":1,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EA)
{
	LoadAndRun("movea.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HB)
{
	LoadAndRun("movhb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HC)
{
	LoadAndRun("movhc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HD)
{
	LoadAndRun("movhd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HE)
{
	LoadAndRun("movhe.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":33,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HH)
{
	LoadAndRun("movhh.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HL)
{
	LoadAndRun("movhl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":33,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HM)
{
	LoadAndRun("movhm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HA)
{
	LoadAndRun("movha.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LB)
{
	LoadAndRun("movlb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LC)
{
	LoadAndRun("movlc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LD)
{
	LoadAndRun("movld.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LE)
{
	LoadAndRun("movle.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":33,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LH)
{
	LoadAndRun("movlh.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":33,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LL)
{
	LoadAndRun("movll.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LM)
{
	LoadAndRun("movlm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":1,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LA)
{
	LoadAndRun("movla.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

//Using MovA for the memory tests since
//we store the result in the accumulator
TEST_F(MachineTest, MOV_MB)
{
	LoadAndRun("movmb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":33,"c":0,"d":0,"e":0,"h":0,"l":4,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_MC)
{
	LoadAndRun("movmc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":33,"d":0,"e":0,"h":0,"l":4,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_MD)
{
	LoadAndRun("movmd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":33,"e":0,"h":0,"l":4,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_ME)
{
	LoadAndRun("movme.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":33,"h":0,"l":4,"s":2},"pc":2,"sp":0})");
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
	LoadAndRun("movma.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":0,"l":4,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AB)
{
	LoadAndRun("movab.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":33,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AC)
{
	LoadAndRun("movac.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":33,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AD)
{
	LoadAndRun("movad.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AE)
{
	LoadAndRun("movae.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AH)
{
	LoadAndRun("movah.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AL)
{
	LoadAndRun("moval.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AM)
{
	LoadAndRun("movam.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":1,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AA)
{
	LoadAndRun("movaa.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MovIncDec)
{
	LoadAndRun("movIncDec.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":119,"b":119,"c":119,"d":119,"e":119,"h":119,"l":119,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_B)
{
	LoadAndRun("addb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":154,"b":46,"c":0,"d":0,"e":0,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_C)
{
	LoadAndRun("addc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":154,"b":0,"c":46,"d":0,"e":0,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_D)
{
	LoadAndRun("addd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":154,"b":0,"c":0,"d":46,"e":0,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_E)
{
	LoadAndRun("adde.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":154,"b":0,"c":0,"d":0,"e":46,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_H)
{
	LoadAndRun("addh.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":154,"b":0,"c":0,"d":0,"e":0,"h":46,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_L)
{
	LoadAndRun("addl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":154,"b":0,"c":0,"d":0,"e":0,"h":0,"l":46,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_M)
{
	LoadAndRun("addm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":216,"b":0,"c":0,"d":0,"e":0,"h":1,"l":1,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_A)
{
	LoadAndRun("adda.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":216,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

#if 0
TEST_F(MachineTest, ADC_B)
{
	auto state = LoadAndRun("adcb.bin");
	EXPECT_EQ(0x80, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, false, true, false);
}

TEST_F(MachineTest, ADC_C)
{
	auto state = LoadAndRun("adcc.bin");
	EXPECT_EQ(0x80, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, false, true, false);
}

TEST_F(MachineTest, ADC_D)
{
	auto state = LoadAndRun("adcd.bin");
	EXPECT_EQ(0x80, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, false, true, false);
}

TEST_F(MachineTest, ADC_E)
{
	auto state = LoadAndRun("adce.bin");
	EXPECT_EQ(0x80, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, false, true, false);
}

TEST_F(MachineTest, ADC_H)
{
	auto state = LoadAndRun("adch.bin");
	EXPECT_EQ(0x80, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, false, true, false);
}

TEST_F(MachineTest, ADC_L)
{
	auto state = LoadAndRun("adcl.bin");
	EXPECT_EQ(0x80, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, false, true, false);
}

TEST_F(MachineTest, ADC_M)
{
	auto state = LoadAndRun("adcm.bin");
	EXPECT_EQ(0x85, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, false, false, false);
}

TEST_F(MachineTest, ADC_A)
{
	auto state = LoadAndRun("adca.bin");
	EXPECT_EQ(0x85, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, false, false, false);
}

TEST_F(MachineTest, SUB_B)
{
	auto state = LoadAndRun("subb.bin");
	EXPECT_EQ(0x00, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), true, false, true, true, false);
}

TEST_F(MachineTest, SUB_C)
{
	auto state = LoadAndRun("subc.bin");
	EXPECT_EQ(0x00, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), true, false, true, true, false);
}

TEST_F(MachineTest, SUB_D)
{
	auto state = LoadAndRun("subd.bin");
	EXPECT_EQ(0x00, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), true, false, true, true, false);
}

TEST_F(MachineTest, SUB_E)
{
	auto state = LoadAndRun("sube.bin");
	EXPECT_EQ(0x00, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), true, false, true, true, false);
}

TEST_F(MachineTest, SUB_H)
{
	auto state = LoadAndRun("subh.bin");
	EXPECT_EQ(0x00, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), true, false, true, true, false);
}

TEST_F(MachineTest, SUB_L)
{
	auto state = LoadAndRun("subl.bin");
	EXPECT_EQ(0x00, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), true, false, true, true, false);
}

TEST_F(MachineTest, SUB_M)
{
	auto state = LoadAndRun("subm.bin");
	EXPECT_EQ(0x00, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), true, false, true, true, false);
}

TEST_F(MachineTest, SUB_A)
{
	auto state = LoadAndRun("suba.bin");
	EXPECT_EQ(0x00, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), true, false, true, true, false);
}

TEST_F(MachineTest, SBB_B)
{
	auto state = LoadAndRun("sbbb.bin");
	EXPECT_EQ(0x01, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, true, false);
}

TEST_F(MachineTest, SBB_C)
{
	auto state = LoadAndRun("sbbc.bin");
	EXPECT_EQ(0x01, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, true, false);
}

TEST_F(MachineTest, SBB_D)
{
	auto state = LoadAndRun("sbbd.bin");
	EXPECT_EQ(0x01, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, true, false);
}

TEST_F(MachineTest, SBB_E)
{
	auto state = LoadAndRun("sbbe.bin");
	EXPECT_EQ(0x01, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, true, false);
}

TEST_F(MachineTest, SBB_H)
{
	auto state = LoadAndRun("sbbh.bin");
	EXPECT_EQ(0x01, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, true, false);
}

TEST_F(MachineTest, SBB_L)
{
	auto state = LoadAndRun("sbbl.bin");
	EXPECT_EQ(0x01, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, true, false);
}

TEST_F(MachineTest, SBB_M)
{
	auto state = LoadAndRun("sbbm.bin");
	EXPECT_EQ(0xFF, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, false, true);
}

TEST_F(MachineTest, SBB_A)
{
	auto state = LoadAndRun("sbba.bin");
	EXPECT_EQ(0xFF, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, false, true);
}

TEST_F(MachineTest, ANA_B)
{
	auto state = LoadAndRun("anab.bin");
	EXPECT_EQ(0x0C, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, ANA_C)
{
	auto state = LoadAndRun("anac.bin");
	EXPECT_EQ(0x0C, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, ANA_D)
{
	auto state = LoadAndRun("anad.bin");
	EXPECT_EQ(0x0C, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, ANA_E)
{
	auto state = LoadAndRun("anae.bin");
	EXPECT_EQ(0x0C, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, ANA_H)
{
	auto state = LoadAndRun("anah.bin");
	EXPECT_EQ(0x0C, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, ANA_L)
{
	auto state = LoadAndRun("anal.bin");
	EXPECT_EQ(0x0C, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, ANA_M)
{
	auto state = LoadAndRun("anam.bin");
	EXPECT_EQ(0x20, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, true, false);
}

TEST_F(MachineTest, ANA_A)
{
	auto state = LoadAndRun("anaa.bin");
	EXPECT_EQ(0xFC, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, true, false);
}

TEST_F(MachineTest, XRA_B)
{
	auto state = LoadAndRun("xrab.bin");
	EXPECT_EQ(0xF0, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, false, false);
}

TEST_F(MachineTest, XRA_C)
{
	auto state = LoadAndRun("xrac.bin");
	EXPECT_EQ(0xF0, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, false, false);
}

TEST_F(MachineTest, XRA_D)
{
	auto state = LoadAndRun("xrad.bin");
	EXPECT_EQ(0xF0, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, false, false);
}

TEST_F(MachineTest, XRA_E)
{
	auto state = LoadAndRun("xrae.bin");
	EXPECT_EQ(0xF0, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, false, false);
}

TEST_F(MachineTest, XRA_H)
{
	auto state = LoadAndRun("xrah.bin");
	EXPECT_EQ(0xF0, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, false, false);
}

TEST_F(MachineTest, XRA_L)
{
	auto state = LoadAndRun("xral.bin");
	EXPECT_EQ(0xF0, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, false, false);
}

TEST_F(MachineTest, XRA_M)
{
	auto state = LoadAndRun("xram.bin");
	EXPECT_EQ(0xDE, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, false, false);
}

TEST_F(MachineTest, XRA_A)
{
	auto state = LoadAndRun("xraa.bin");
	EXPECT_EQ(0x00, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), true, false, true, false, false);
}

TEST_F(MachineTest, ORA_B)
{
	auto state = LoadAndRun("orab.bin");
	EXPECT_EQ(0x3F, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, false, false);
}

TEST_F(MachineTest, ORA_C)
{
	auto state = LoadAndRun("orac.bin");
	EXPECT_EQ(0x3F, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, false, false);
}

TEST_F(MachineTest, ORA_D)
{
	auto state = LoadAndRun("orad.bin");
	EXPECT_EQ(0x3F, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, false, false);
}

TEST_F(MachineTest, ORA_E)
{
	auto state = LoadAndRun("orae.bin");
	EXPECT_EQ(0x3F, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, false, false);
}

TEST_F(MachineTest, ORA_H)
{
	auto state = LoadAndRun("orah.bin");
	EXPECT_EQ(0x3F, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, false, false);
}

TEST_F(MachineTest, ORA_L)
{
	auto state = LoadAndRun("oral.bin");
	EXPECT_EQ(0x3F, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, false, false);
}

TEST_F(MachineTest, ORA_M)
{
	auto state = LoadAndRun("oram.bin");
	EXPECT_EQ(0xFF, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, false, false);
}

TEST_F(MachineTest, ORA_A)
{
	auto state = LoadAndRun("oraa.bin");
	EXPECT_EQ(0xFC, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, false, false);
}

TEST_F(MachineTest, CMP_B)
{
	auto state = LoadAndRun("cmpb.bin");
	EXPECT_EQ(0x0A, state["cpu"]["registers"]["a"].get<uint8_t>());
	EXPECT_EQ(0x05, state["cpu"]["registers"]["b"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, CMP_C)
{
	auto state = LoadAndRun("cmpc.bin");
	EXPECT_EQ(0x0A, state["cpu"]["registers"]["a"].get<uint8_t>());
	EXPECT_EQ(0x05, state["cpu"]["registers"]["c"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, CMP_D)
{
	auto state = LoadAndRun("cmpd.bin");
	EXPECT_EQ(0x0A, state["cpu"]["registers"]["a"].get<uint8_t>());
	EXPECT_EQ(0x05, state["cpu"]["registers"]["d"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, CMP_E)
{
	auto state = LoadAndRun("cmpe.bin");
	EXPECT_EQ(0x0A, state["cpu"]["registers"]["a"].get<uint8_t>());
	EXPECT_EQ(0x05, state["cpu"]["registers"]["e"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, CMP_H)
{
	auto state = LoadAndRun("cmph.bin");
	EXPECT_EQ(0x0A, state["cpu"]["registers"]["a"].get<uint8_t>());
	EXPECT_EQ(0x05, state["cpu"]["registers"]["h"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, CMP_L)
{
	auto state = LoadAndRun("cmpl.bin");
	EXPECT_EQ(0x0A, state["cpu"]["registers"]["a"].get<uint8_t>());
	EXPECT_EQ(0x05, state["cpu"]["registers"]["l"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, CMP_M)
{
	auto state = LoadAndRun("cmpm.bin");
	//The contents of the accumulator and the memory must not change.
	EXPECT_EQ(0x0A, state["cpu"]["registers"]["a"].get<uint8_t>());
	EXPECT_EQ(0x21, memoryController_->Read(0x0102));

	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, false, true, true);
}

/*
	Need to fix git settings, seems to be adding a carraige return to this
	test which causes it to fail
*/
#ifdef ENABLE_CMP_A
TEST_F(MachineTest, CMP_A)
{
	auto state = LoadAndRun("cmpa.bin");
	EXPECT_EQ(0x0A, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), true, false, true, true, false);
}
#endif

TEST_F(MachineTest, RNZ)
{
	auto state = LoadAndRun("rnz.bin");
	EXPECT_EQ(0x00, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), true, false, true, true, false);
}

TEST_F(MachineTest, PUSH_POP_B)
{
	auto state = LoadAndRun("pushpopb.bin");
	EXPECT_EQ(0xAB, state["cpu"]["registers"]["b"].get<uint8_t>());
	EXPECT_EQ(0xCD, state["cpu"]["registers"]["c"].get<uint8_t>());
}

TEST_F(MachineTest, JNZ)
{
	auto state = LoadAndRun("jnz.bin");
	EXPECT_EQ(0xFF, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, false, false);
}

TEST_F(MachineTest, JMP)
{
	auto state = LoadAndRun("jmp.bin");
	EXPECT_EQ(0x3F, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, false, false);
}

TEST_F(MachineTest, CNZ)
{
	auto state = LoadAndRun("cnz.bin");
	EXPECT_EQ(0x30, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, false, false);
}

TEST_F(MachineTest, ADI_1)
{
	auto state = LoadAndRun("adi1.bin");
	EXPECT_EQ(0x56, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, false, false);
}

TEST_F(MachineTest, ADI_2)
{
	auto state = LoadAndRun("adi2.bin");
	EXPECT_EQ(0x14, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, true);
}

TEST_F(MachineTest, RST_0)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x00);
	auto state = LoadAndRun("rst0.bin");
	EXPECT_EQ(0x02, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, false, false);
}

TEST_F(MachineTest, RST_1)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x08);
	auto state = LoadAndRun("rst1.bin");
	EXPECT_EQ(0x02, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, false, false);
}

TEST_F(MachineTest, RST_2)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x10);
	auto state = LoadAndRun("rst2.bin");
	EXPECT_EQ(0x02, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, false, false);
}

TEST_F(MachineTest, RST_3)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x18);
	auto state = LoadAndRun("rst3.bin");
	EXPECT_EQ(0x02, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, false, false);
}

TEST_F(MachineTest, RST_4)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x20);
	auto state = LoadAndRun("rst4.bin");
	EXPECT_EQ(0x02, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, false, false);
}

TEST_F(MachineTest, RST_5)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x28);
	auto state = LoadAndRun("rst5.bin");
	EXPECT_EQ(0x02, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, false, false);
}

TEST_F(MachineTest, RST_6)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x30);
	auto state = LoadAndRun("rst6.bin");
	EXPECT_EQ(0x02, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, false, false);
}

TEST_F(MachineTest, RST_7)
{
	memoryController_->Load(PROGRAMS_DIR"/rst.bin", 0x38);
	auto state = LoadAndRun("rst7.bin");
	EXPECT_EQ(0x02, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, false, false);
}

TEST_F(MachineTest, RZ)
{
	auto state = LoadAndRun("rz.bin");
	EXPECT_EQ(0x20, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, false, false);
}

TEST_F(MachineTest, RET)
{
	auto state = LoadAndRun("ret.bin");
	EXPECT_EQ(0x40, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, false, false);
}

TEST_F(MachineTest, JZ)
{
	auto state = LoadAndRun("jz.bin");
	EXPECT_EQ(0x00, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), true, false, true, true, false);
}

TEST_F(MachineTest, CZ)
{
	auto state = LoadAndRun("cz.bin");
	EXPECT_EQ(0x10, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, false, false);
}

TEST_F(MachineTest, CALL)
{
	auto state = LoadAndRun("call.bin");
	EXPECT_EQ(0x20, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, false, false);
}

TEST_F(MachineTest, ACI_1)
{
	auto state = LoadAndRun("aci1.bin");
	EXPECT_EQ(0x14, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, true);
}

TEST_F(MachineTest, ACI_2)
{
	auto state = LoadAndRun("aci2.bin");
	EXPECT_EQ(0x57, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, false, false, false);
}

TEST_F(MachineTest, RNC)
{
	auto state = LoadAndRun("rnc.bin");
	EXPECT_EQ(0x10, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, PUSH_POP_D)
{
	auto state = LoadAndRun("pushpopd.bin");
	EXPECT_EQ(0xAB, state["cpu"]["registers"]["d"].get<uint8_t>());
	EXPECT_EQ(0xCD, state["cpu"]["registers"]["e"].get<uint8_t>());
}

TEST_F(MachineTest, JNC)
{
	auto state = LoadAndRun("jnc.bin");
	EXPECT_EQ(0xFF, state["cpu"]["registers"]["a"].get<uint8_t>());
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
	EXPECT_EQ(0xFF, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, SUI)
{
	auto state = LoadAndRun("sui.bin");
	EXPECT_EQ(0xFF, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, false, true);
}

TEST_F(MachineTest, RC)
{
	auto state = LoadAndRun("rc.bin");
	EXPECT_EQ(0xFF, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, JC)
{
	auto state = LoadAndRun("jc.bin");
	EXPECT_EQ(0x10, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, IN)
{
	auto state = LoadAndRun("in.bin");
	EXPECT_EQ(0xAA, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, CC)
{
	auto state = LoadAndRun("cc.bin");
	EXPECT_EQ(0x10, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, SBI)
{
	auto state = LoadAndRun("sbi.bin");
	EXPECT_EQ(0xFE, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, false, false, true);
}

TEST_F(MachineTest, RPO)
{
	auto state = LoadAndRun("rpo.bin");
	EXPECT_EQ(0x10, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, PUSH_POP_H)
{
	auto state = LoadAndRun("pushpoph.bin");
	EXPECT_EQ(0xAB, state["cpu"]["registers"]["h"].get<uint8_t>());
	EXPECT_EQ(0xCD, state["cpu"]["registers"]["l"].get<uint8_t>());
}

TEST_F(MachineTest, JPO)
{
	auto state = LoadAndRun("jpo.bin");
	EXPECT_EQ(0x11, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, XTHL)
{
	auto state = LoadAndRun("xthl.bin");
	EXPECT_EQ(0xAB, state["cpu"]["registers"]["h"].get<uint8_t>());
	EXPECT_EQ(0xCD, state["cpu"]["registers"]["l"].get<uint8_t>());
}

TEST_F(MachineTest, CPO)
{
	auto state = LoadAndRun("cpo.bin");
	EXPECT_EQ(0x11, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, ANI)
{
	auto state = LoadAndRun("ani.bin");
	EXPECT_EQ(0x0A, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, RPE)
{
	auto state = LoadAndRun("rpe.bin");
	EXPECT_EQ(0x11, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, PCHL)
{
	auto state = LoadAndRun("pchl.bin");
	EXPECT_EQ(0x11, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, JPE)
{
	auto state = LoadAndRun("jpe.bin");
	EXPECT_EQ(0x10, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, XCHG)
{
	auto state = LoadAndRun("xchg.bin");
	EXPECT_EQ(0x01, state["cpu"]["registers"]["d"].get<uint8_t>());
	EXPECT_EQ(0x23, state["cpu"]["registers"]["e"].get<uint8_t>());
	EXPECT_EQ(0xAB, state["cpu"]["registers"]["h"].get<uint8_t>());
	EXPECT_EQ(0xCD, state["cpu"]["registers"]["l"].get<uint8_t>());
}

TEST_F(MachineTest, CPE)
{
	auto state = LoadAndRun("cpe.bin");
	EXPECT_EQ(0x10, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, XRI)
{
	auto state = LoadAndRun("xri.bin");
	EXPECT_EQ(0xBA, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, false, false, false);
}

TEST_F(MachineTest, RP)
{
	auto state = LoadAndRun("rp.bin");
	EXPECT_EQ(0x11, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, PUSH_POP_PSW)
{
	auto state = LoadAndRun("pushpoppsw.bin");
	EXPECT_EQ(0x00, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), true, false, true, true, false);
}

TEST_F(MachineTest, JP)
{
	auto state = LoadAndRun("jp.bin");
	EXPECT_EQ(0x10, state["cpu"]["registers"]["a"].get<uint8_t>());
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
	EXPECT_EQ(0x10, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, ORI)
{
	auto state = LoadAndRun("ori.bin");
	EXPECT_EQ(0xBF, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, false, false, false);
}

TEST_F(MachineTest, RM)
{
	auto state = LoadAndRun("rm.bin");
	EXPECT_EQ(0x10, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, SPHL)
{
	auto state = LoadAndRun("sphl.bin");
	EXPECT_EQ(0x0123, state["cpu"]["sp"].get<uint16_t>());
}

TEST_F(MachineTest, JM)
{
	auto state = LoadAndRun("jm.bin");
	EXPECT_EQ(0x11, state["cpu"]["registers"]["a"].get<uint8_t>());
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
	EXPECT_EQ(0x11, state["cpu"]["registers"]["a"].get<uint8_t>());
}

TEST_F(MachineTest, CPI)
{
	auto state = LoadAndRun("cpi.bin");
	EXPECT_EQ(0x4A, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, false, true, true, false);
}

TEST_F(MachineTest, CPI0)
{
	auto state = LoadAndRun("cpi0.bin");
	EXPECT_EQ(0xF5, state["cpu"]["registers"]["a"].get<uint8_t>());
	//zero, sign, parity, auxCarry, carry
	CheckStatus(state["cpu"]["registers"]["s"].get<uint8_t>(), false, true, true, true, false);
}

TEST_F(MachineTest, ISR_1)
{
	//Load the interrupt service routine at the correct address
	//The ioController fires rst 1 every second.
	memoryController_->Load(PROGRAMS_DIR"/isr.bin", 0x08);

	// Set to the resolution to 50Hz
	auto err = machine_->SetOptions(R"({"clockResolution":25000000})");
	EXPECT_EQ(ErrorCode::NoError, err);

	// There is a chance for an infinite spin if the test fails.
	// TODO: since we fire every second we need to test that the Run
	// method takes a second to complete, pass back the Run value
	// from LoadAndRun or pass it in to LoadAndRun as a reference
	// parameter to be set.
	auto state = LoadAndRun("spinUntilIsr.bin");

	EXPECT_EQ(0x00, state["cpu"]["registers"]["a"].get<uint8_t>());
	EXPECT_EQ(0x01, state["cpu"]["registers"]["b"].get<uint8_t>());
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

#endif

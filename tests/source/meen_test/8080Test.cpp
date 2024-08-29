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

TEST_F(MachineTest, ADC_B)
{
	LoadAndRun("adcb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":128,"b":61,"c":0,"d":0,"e":0,"h":0,"l":0,"s":146},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_C)
{
	LoadAndRun("adcc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":128,"b":0,"c":61,"d":0,"e":0,"h":0,"l":0,"s":146},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_D)
{
	LoadAndRun("adcd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":128,"b":0,"c":0,"d":61,"e":0,"h":0,"l":0,"s":146},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_E)
{
	LoadAndRun("adce.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":128,"b":0,"c":0,"d":0,"e":61,"h":0,"l":0,"s":146},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_H)
{
	LoadAndRun("adch.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":128,"b":0,"c":0,"d":0,"e":0,"h":61,"l":0,"s":146},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_L)
{
	LoadAndRun("adcl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":128,"b":0,"c":0,"d":0,"e":0,"h":0,"l":61,"s":146},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_M)
{
	LoadAndRun("adcm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":133,"b":0,"c":0,"d":0,"e":0,"h":1,"l":2,"s":130},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_A)
{
	LoadAndRun("adca.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":133,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":130},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_B)
{
	LoadAndRun("subb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":62,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_C)
{
	LoadAndRun("subc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":62,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_D)
{
	LoadAndRun("subd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":62,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_E)
{
	LoadAndRun("sube.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":62,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_H)
{
	LoadAndRun("subh.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":62,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_L)
{
	LoadAndRun("subl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":62,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_M)
{
	LoadAndRun("subm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":1,"l":1,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_A)
{
	LoadAndRun("suba.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_B)
{
	LoadAndRun("sbbb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":2,"c":0,"d":0,"e":0,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_C)
{
	LoadAndRun("sbbc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":0,"c":2,"d":0,"e":0,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_D)
{
	LoadAndRun("sbbd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":0,"c":0,"d":2,"e":0,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_E)
{
	LoadAndRun("sbbe.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":0,"c":0,"d":0,"e":2,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_H)
{
	LoadAndRun("sbbh.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":0,"c":0,"d":0,"e":0,"h":2,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_L)
{
	LoadAndRun("sbbl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":0,"c":0,"d":0,"e":0,"h":0,"l":2,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_M)
{
	LoadAndRun("sbbm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":1,"l":2,"s":135},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_A)
{
	LoadAndRun("sbba.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":135},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_B)
{
	LoadAndRun("anab.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":12,"b":15,"c":0,"d":0,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_C)
{
	LoadAndRun("anac.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":12,"b":0,"c":15,"d":0,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_D)
{
	LoadAndRun("anad.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":12,"b":0,"c":0,"d":15,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_E)
{
	LoadAndRun("anae.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":12,"b":0,"c":0,"d":0,"e":15,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_H)
{
	LoadAndRun("anah.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":12,"b":0,"c":0,"d":0,"e":0,"h":15,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_L)
{
	LoadAndRun("anal.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":12,"b":0,"c":0,"d":0,"e":0,"h":0,"l":15,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_M)
{
	LoadAndRun("anam.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":32,"b":0,"c":0,"d":0,"e":0,"h":1,"l":2,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_A)
{
	LoadAndRun("anaa.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":252,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_B)
{
	LoadAndRun("xrab.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":240,"b":15,"c":0,"d":0,"e":0,"h":0,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_C)
{
	LoadAndRun("xrac.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":240,"b":0,"c":15,"d":0,"e":0,"h":0,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_D)
{
	LoadAndRun("xrad.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":240,"b":0,"c":0,"d":15,"e":0,"h":0,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_E)
{
	LoadAndRun("xrae.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":240,"b":0,"c":0,"d":0,"e":15,"h":0,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_H)
{
	LoadAndRun("xrah.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":240,"b":0,"c":0,"d":0,"e":0,"h":15,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_L)
{
	LoadAndRun("xral.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":240,"b":0,"c":0,"d":0,"e":0,"h":0,"l":15,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_M)
{
	LoadAndRun("xram.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":222,"b":0,"c":0,"d":0,"e":0,"h":1,"l":2,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_A)
{
	LoadAndRun("xraa.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":70},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_B)
{
	LoadAndRun("orab.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":15,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_C)
{
	LoadAndRun("orac.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":0,"c":15,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_D)
{
	LoadAndRun("orad.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":0,"c":0,"d":15,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_E)
{
	LoadAndRun("orae.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":0,"c":0,"d":0,"e":15,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_H)
{
	LoadAndRun("orah.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":0,"c":0,"d":0,"e":0,"h":15,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_L)
{
	LoadAndRun("oral.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":0,"c":0,"d":0,"e":0,"h":0,"l":15,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_M)
{
	LoadAndRun("oram.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":1,"l":2,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_A)
{
	LoadAndRun("oraa.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":252,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_B)
{
	LoadAndRun("cmpb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":5,"c":0,"d":0,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_C)
{
	LoadAndRun("cmpc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":5,"d":0,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_D)
{
	LoadAndRun("cmpd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":0,"d":5,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_E)
{
	LoadAndRun("cmpe.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":0,"d":0,"e":5,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_H)
{
	LoadAndRun("cmph.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":0,"d":0,"e":0,"h":5,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_L)
{
	LoadAndRun("cmpl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":0,"d":0,"e":0,"h":0,"l":5,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_M)
{
	LoadAndRun("cmpm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":0,"d":0,"e":0,"h":1,"l":2,"s":147},"pc":2,"sp":0})");
}

/*
	Need to fix git settings, seems to be adding a carraige return to this
	test which causes it to fail
*/
#ifdef ENABLE_CMP_A
TEST_F(MachineTest, CMP_A)
{
	LoadAndRun("cmpa.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":211,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}
#endif

TEST_F(MachineTest, RNZ)
{
	LoadAndRun("rnz.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, PUSH_POP_B)
{
	LoadAndRun("pushpopb.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":171,"c":205,"d":0,"e":0,"h":0,"l":0,"s":70},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JNZ)
{
	LoadAndRun("jnz.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JMP)
{
	LoadAndRun("jmp.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CNZ)
{
	LoadAndRun("cnz.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":48,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADI_1)
{
	LoadAndRun("adi1.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":86,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADI_2)
{
	LoadAndRun("adi2.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":20,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":23},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RST_0)
{
	memoryController_->Load((programsDir_ + "/rst.bin").c_str(), 0x00);
	LoadAndRun("rst0.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":261,"sp":0})");
}

TEST_F(MachineTest, RST_1)
{
	memoryController_->Load((programsDir_ + "/rst.bin").c_str(), 0x08);
	LoadAndRun("rst1.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RST_2)
{
	memoryController_->Load((programsDir_ + "/rst.bin").c_str(), 0x10);
	LoadAndRun("rst2.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RST_3)
{
	memoryController_->Load((programsDir_ + "/rst.bin").c_str(), 0x18);
	LoadAndRun("rst3.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RST_4)
{
	memoryController_->Load((programsDir_ + "/rst.bin").c_str(), 0x20);
	LoadAndRun("rst4.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RST_5)
{
	memoryController_->Load((programsDir_ + "/rst.bin").c_str(), 0x28);
	LoadAndRun("rst5.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RST_6)
{
	memoryController_->Load((programsDir_ + "/rst.bin").c_str(), 0x30);
	LoadAndRun("rst6.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RST_7)
{
	memoryController_->Load((programsDir_ + "/rst.bin").c_str(), 0x38);
	LoadAndRun("rst7.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RZ)
{
	LoadAndRun("rz.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":32,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RET)
{
	LoadAndRun("ret.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":64,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JZ)
{
	LoadAndRun("jz.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CZ)
{
	LoadAndRun("cz.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, CALL)
{
	LoadAndRun("call.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":32,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, ACI_1)
{
	LoadAndRun("aci1.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":20,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":23},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ACI_2)
{
	LoadAndRun("aci2.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":87,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RNC)
{
	LoadAndRun("rnc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, PUSH_POP_D)
{
	LoadAndRun("pushpopd.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":171,"e":205,"h":0,"l":0,"s":70},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JNC)
{
	LoadAndRun("jnc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":135},"pc":2,"sp":0})");	
}

TEST_F(MachineTest, OUT)
{
	LoadAndRun("out.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");

	//Read the contents of device 0
	EXPECT_EQ(0xFF, testIoController_->Read(0x00));
	//Reset the value of the device to it's default (0xAA)
	testIoController_->Write (0x00, 0xAA);
}

TEST_F(MachineTest, CNC)
{
	LoadAndRun("cnc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":135},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUI)
{
	LoadAndRun("sui.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":135},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RC)
{
	LoadAndRun("rc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":135},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JC)
{
	LoadAndRun("jc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, IN)
{
	LoadAndRun("in.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":70},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CC)
{
	LoadAndRun("cc.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, SBI)
{
	LoadAndRun("sbi.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":254,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":131},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RPO)
{
	LoadAndRun("rpo.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":70},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, PUSH_POP_H)
{
	LoadAndRun("pushpoph.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":171,"l":205,"s":70},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JPO)
{
	LoadAndRun("jpo.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XTHL)
{
	LoadAndRun("xthl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":171,"b":0,"c":0,"d":0,"e":0,"h":171,"l":205,"s":2},"pc":2,"sp":256})");
}

TEST_F(MachineTest, CPO)
{
	LoadAndRun("cpo.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANI)
{
	LoadAndRun("ani.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RPE)
{
	LoadAndRun("rpe.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, PCHL)
{
	LoadAndRun("pchl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":1,"l":7,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JPE)
{
	LoadAndRun("jpe.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":70},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XCHG)
{
	LoadAndRun("xchg.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":1,"e":35,"h":171,"l":205,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CPE)
{
	LoadAndRun("cpe.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":70},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, XRI)
{
	LoadAndRun("xri.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":186,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":130},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RP)
{
	LoadAndRun("rp.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, PUSH_POP_PSW)
{
	LoadAndRun("pushpoppsw.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JP)
{
	LoadAndRun("jp.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

/*
TEST_F(MachineTest, DI)
{
	LoadAndRun("di.bin");
}
*/

TEST_F(MachineTest, CP)
{
	LoadAndRun("cp.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, ORI)
{
	LoadAndRun("ori.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":191,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":130},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RM)
{
	LoadAndRun("rm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, SPHL)
{
	LoadAndRun("sphl.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":1,"l":35,"s":2},"pc":2,"sp":291})");
}

TEST_F(MachineTest, JM)
{
	LoadAndRun("jm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

/*
TEST_F(MachineTest, EI)
{
	auto state = LoadAndRun("ei.bin");
}
*/

TEST_F(MachineTest, CM)
{
	LoadAndRun("cm.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CPI)
{
	LoadAndRun("cpi.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":74,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CPI0)
{
	LoadAndRun("cpi0.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":245,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ISR_1)
{
	//Load the interrupt service routine at the correct address
	//The ioController fires rst 1 every second.
	memoryController_->Load((programsDir_ + "/isr.bin").c_str(), 0x08);

	// Set to the resolution to 50Hz
	auto err = machine_->SetOptions(R"({"clockResolution":25000000})");
	EXPECT_FALSE(err);

	// There is a chance for an infinite spin if the test fails.
	// TODO: since we fire every second we need to test that the Run
	// method takes a second to complete, pass back the Run value
	// from LoadAndRun.
	LoadAndRun("spinUntilIsr.bin", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":1,"c":0,"d":0,"e":0,"h":0,"l":0,"s":135},"pc":2,"sp":0})");
}

TEST_F(MachineTest, Tst8080)
{
	// use the cpm io controller for cpm based tests
	auto err = machine_->SetIoController(cpmIoController_);
    EXPECT_FALSE(err);
	err = machine_->SetOptions(R"({"isrFreq":0.02})");
    EXPECT_FALSE(err);
	LoadAndRun("TST8080.COM", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981})");
	EXPECT_EQ(74, static_pointer_cast<CpmIoController>(cpmIoController_)->Message().find("CPU IS OPERATIONAL"));
}

TEST_F(MachineTest, 8080Pre)
{
	// use the cpm io controller for cpm based tests
	auto err = machine_->SetIoController(cpmIoController_);
    EXPECT_FALSE(err);
	err = machine_->SetOptions(R"({"isrFreq":0.02})");
	EXPECT_FALSE(err);
	LoadAndRun("8080PRE.COM", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":9,"d":3,"e":50,"h":1,"l":0,"s":86},"pc":2,"sp":1280})");
	EXPECT_EQ(0, static_pointer_cast<CpmIoController>(cpmIoController_)->Message().find("8080 Preliminary tests complete"));
}

TEST_F(MachineTest, CpuTest)
{
	// use the cpm io controller for cpm based tests
	auto err = machine_->SetIoController(cpmIoController_);
	EXPECT_FALSE(err);
	err = machine_->SetOptions(R"({"isrFreq":0.02})");
    EXPECT_FALSE(err);
	LoadAndRun("CPUTEST.COM", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":247,"d":4,"e":23,"h":0,"l":0,"s":70},"pc":2,"sp":12283})");
	EXPECT_EQ(168, static_pointer_cast<CpmIoController>(cpmIoController_)->Message().find("CPU TESTS OK"));
}

TEST_F(MachineTest, 8080Exm)
{
	// use the cpm io controller for cpm based tests
	auto err = machine_->SetIoController(cpmIoController_);
	EXPECT_FALSE(err);
	err = machine_->SetOptions(R"({"isrFreq":0.02})");
    EXPECT_FALSE(err);
	LoadAndRun("8080EXM.COM", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":10,"c":9,"d":14,"e":30,"h":1,"l":109,"s":70},"pc":2,"sp":54137})");
	EXPECT_EQ(static_pointer_cast<CpmIoController>(cpmIoController_)->Message().find("ERROR"), std::string::npos);
}

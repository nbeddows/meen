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

TEST_F(MachineTest, LXI_B)
{
	LoadAndRun("base64://ARL/wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":255,"c":18,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, STAX_B)
{
	LoadAndRun("base64://PiEBBAACPgAKwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":4,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INX_B)
{
	LoadAndRun("base64://ARL/A8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":255,"c":19,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INR_B)
{
	LoadAndRun("base64://Bg8EwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":16,"c":0,"d":0,"e":0,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_B)
{
	LoadAndRun("base64://BgEFwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_B)
{
	LoadAndRun("base64://BhLDAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":18,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RLC)
{
	LoadAndRun("base64://PvIHwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":229,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DAD_B)
{
	LoadAndRun("base64://IXuhAZ8zCcMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":51,"c":159,"d":0,"e":0,"h":213,"l":26,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, LDAX_B)
{
	LoadAndRun("base64://AQMBCsMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":1,"c":3,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCX_B)
{
	LoadAndRun("base64://AQAAC8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":255,"c":255,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INR_C)
{
	LoadAndRun("base64://Dg8MwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":16,"d":0,"e":0,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_C)
{
	LoadAndRun("base64://DgENwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_C)
{
	LoadAndRun("base64://DhLDAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":18,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RRC)
{
	LoadAndRun("base64://PvIPwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":121,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, LXI_D)
{
	LoadAndRun("base64://ERL/wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":255,"e":18,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, STAX_D)
{
	LoadAndRun("base64://PiERBAASPgAawwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":4,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INX_D)
{
	LoadAndRun("base64://ERL/E8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":255,"e":19,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INR_D)
{
	LoadAndRun("base64://Fg8UwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":16,"e":0,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_D)
{
	LoadAndRun("base64://FgEVwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_D)
{
	LoadAndRun("base64://FhLDAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":18,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RAL)
{
	LoadAndRun("base64://PrUXwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":106,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DAD_D)
{
	LoadAndRun("base64://If//EQEAGcMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":1,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, LDAX_D)
{
	LoadAndRun("base64://EQMBGsMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":26,"b":0,"c":0,"d":1,"e":3,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCX_D)
{
	LoadAndRun("base64://EQAAG8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":255,"e":255,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INR_E)
{
	LoadAndRun("base64://Hg8cwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":16,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_E)
{
	LoadAndRun("base64://HgEdwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_E)
{
	LoadAndRun("base64://HhLDAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":18,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RAR)
{
	LoadAndRun("base64://Nz5qH8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":181,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, LXI_H)
{
	LoadAndRun("base64://IRL/wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":255,"l":18,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SHLD)
{
	LoadAndRun("base64://Ic2rIv4AOv4ARzr/AMMAAA==", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":171,"b":205,"c":0,"d":0,"e":0,"h":171,"l":205,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INX_H)
{
	LoadAndRun("base64://IRL/I8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":255,"l":19,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INR_H)
{
	LoadAndRun("base64://Jg8kwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":16,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_H)
{
	LoadAndRun("base64://JgElwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_H)
{
	LoadAndRun("base64://JhLDAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":18,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DAA)
{
	LoadAndRun("base64://PpsnwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":19},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DAD_H)
{
	LoadAndRun("base64://IXuhKcMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":66,"l":246,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, LHLD)
{
	LoadAndRun("base64://AaF7KgIBwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":123,"c":161,"d":0,"e":0,"h":42,"l":123,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCX_H)
{
	LoadAndRun("base64://IQAAK8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":255,"l":255,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INR_L)
{
	LoadAndRun("base64://Lg8swwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":16,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_L)
{
	LoadAndRun("base64://LgEtwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_L)
{
	LoadAndRun("base64://LhLDAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":18,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMA)
{
	LoadAndRun("base64://Pv8vwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, LXI_SP)
{
	LoadAndRun("base64://MavNwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":52651})");
}

TEST_F(MachineTest, STA)
{
	LoadAndRun("base64://Pv8y/wA6/wDDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, INX_SP)
{
	LoadAndRun("base64://MavNM8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":52652})");
}

TEST_F(MachineTest, INR_M)
{
	LoadAndRun("base64://IQABNCoAAcMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":34,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCR_M)
{
	LoadAndRun("base64://IQABNSoAAcMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":32,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_M)
{
	LoadAndRun("base64://If4ANt0q/gDDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":221,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, STC)
{
	LoadAndRun("base64://N8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DAD_SP)
{
	LoadAndRun("base64://Mc2rIc2rOcMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":87,"l":154,"s":3},"pc":2,"sp":43981})");
}

TEST_F(MachineTest, LDA)
{
	LoadAndRun("base64://NzoBAcMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":58,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, DCX_SP)
{
	LoadAndRun("base64://MavNO8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":52650})");
}

TEST_F(MachineTest, INR_A)
{
	LoadAndRun("base64://NzoBATzDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":59,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

// The inr/dcr programs start out by setting the carry bit
// which tests the fact that the inr/dcr programs don't modify the carry bit
TEST_F(MachineTest, DCR_A)
{
	LoadAndRun("base64://NzoBAT3DAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":57,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":23},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MVI_A)
{
	LoadAndRun("base64://PhLDAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":18,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMC)
{
	LoadAndRun("base64://P8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BB)
{
	LoadAndRun("base64://BiFAwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BC)
{
	LoadAndRun("base64://DiFBwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":33,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BD)
{
	LoadAndRun("base64://FiFCwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BE)
{
	LoadAndRun("base64://HiFDwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BH)
{
	LoadAndRun("base64://JiFEwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BL)
{
	LoadAndRun("base64://LiFFwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BM)
{
	LoadAndRun("base64://IQABRsMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":0,"h":1,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_BA)
{
	LoadAndRun("base64://PiFHwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":33,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CB)
{
	LoadAndRun("base64://BiFIwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":33,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CC)
{
	LoadAndRun("base64://DiFJwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CD)
{
	LoadAndRun("base64://FiFKwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CE)
{
	LoadAndRun("base64://HiFLwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CH)
{
	LoadAndRun("base64://JiFMwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CL)
{
	LoadAndRun("base64://LiFNwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CM)
{
	LoadAndRun("base64://IQABTsMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":0,"h":1,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_CA)
{
	LoadAndRun("base64://PiFPwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":33,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DB)
{
	LoadAndRun("base64://BiFQwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DC)
{
	LoadAndRun("base64://DiFRwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DD)
{
	LoadAndRun("base64://FiFSwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DE)
{
	LoadAndRun("base64://HiFTwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DH)
{
	LoadAndRun("base64://JiFUwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DL)
{
	LoadAndRun("base64://LiFVwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DM)
{
	LoadAndRun("base64://IQABVsMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":0,"h":1,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_DA)
{
	LoadAndRun("base64://PiFXwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EB)
{
	LoadAndRun("base64://BiFYwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EC)
{
	LoadAndRun("base64://DiFZwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_ED)
{
	LoadAndRun("base64://FiFawwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EE)
{
	LoadAndRun("base64://HiFbwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EH)
{
	LoadAndRun("base64://JiFcwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":33,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EL)
{
	LoadAndRun("base64://LiFdwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":33,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EM)
{
	LoadAndRun("base64://IQABXsMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":33,"h":1,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_EA)
{
	LoadAndRun("base64://PiFfwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HB)
{
	LoadAndRun("base64://BiFgwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HC)
{
	LoadAndRun("base64://DiFhwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HD)
{
	LoadAndRun("base64://FiFiwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HE)
{
	LoadAndRun("base64://HiFjwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":33,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HH)
{
	LoadAndRun("base64://JiFkwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HL)
{
	LoadAndRun("base64://LiFlwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":33,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HM)
{
	LoadAndRun("base64://IQABZsMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_HA)
{
	LoadAndRun("base64://PiFnwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LB)
{
	LoadAndRun("base64://BiFowwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":33,"c":0,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LC)
{
	LoadAndRun("base64://DiFpwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":33,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LD)
{
	LoadAndRun("base64://FiFqwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":33,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LE)
{
	LoadAndRun("base64://HiFrwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":33,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LH)
{
	LoadAndRun("base64://JiFswwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":33,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LL)
{
	LoadAndRun("base64://LiFtwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LM)
{
	LoadAndRun("base64://IQABbsMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":1,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_LA)
{
	LoadAndRun("base64://PiFvwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

//Using MovA for the memory tests since
//we store the result in the accumulator
TEST_F(MachineTest, MOV_MB)
{
	LoadAndRun("base64://BiEhBABwOgQAwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":33,"c":0,"d":0,"e":0,"h":0,"l":4,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_MC)
{
	LoadAndRun("base64://DiEhBABxOgQAwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":33,"d":0,"e":0,"h":0,"l":4,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_MD)
{
	LoadAndRun("base64://FiEhBAByOgQAwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":33,"e":0,"h":0,"l":4,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_ME)
{
	LoadAndRun("base64://HiEhBABzOgQAwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":33,"h":0,"l":4,"s":2},"pc":2,"sp":0})");
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
	LoadAndRun("base64://PiEhBAB3OgQAwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":0,"l":4,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AB)
{
	LoadAndRun("base64://BiF4wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":33,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AC)
{
	LoadAndRun("base64://DiF5wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":33,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AD)
{
	LoadAndRun("base64://FiF6wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":33,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AE)
{
	LoadAndRun("base64://HiF7wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":33,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AH)
{
	LoadAndRun("base64://JiF8wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":33,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AL)
{
	LoadAndRun("base64://LiF9wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":0,"l":33,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AM)
{
	LoadAndRun("base64://IQABfsMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":1,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MOV_AA)
{
	LoadAndRun("base64://PiF/wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":33,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, MovIncDec)
{
	LoadAndRun("base64://Pnc8RwRIDVFaY2x9PU9Za0VQYnxXFGpNDGFEBVh7XxxDYCRMaVUVemclVEJoLF0dS3lvLWVcU0pBeMMAAA==", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":119,"b":119,"c":119,"d":119,"e":119,"h":119,"l":119,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_B)
{
	LoadAndRun("base64://PmwGLoDDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":154,"b":46,"c":0,"d":0,"e":0,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_C)
{
	LoadAndRun("base64://PmwOLoHDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":154,"b":0,"c":46,"d":0,"e":0,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_D)
{
	LoadAndRun("base64://PmwWLoLDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":154,"b":0,"c":0,"d":46,"e":0,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_E)
{
	LoadAndRun("base64://PmweLoPDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":154,"b":0,"c":0,"d":0,"e":46,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_H)
{
	LoadAndRun("base64://PmwmLoTDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":154,"b":0,"c":0,"d":0,"e":0,"h":46,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_L)
{
	LoadAndRun("base64://PmwuLoXDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":154,"b":0,"c":0,"d":0,"e":0,"h":0,"l":46,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_M)
{
	LoadAndRun("base64://PmwhAQGGwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":216,"b":0,"c":0,"d":0,"e":0,"h":1,"l":1,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADD_A)
{
	LoadAndRun("base64://PmyHwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":216,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_B)
{
	LoadAndRun("base64://Nz5CBj2IwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":128,"b":61,"c":0,"d":0,"e":0,"h":0,"l":0,"s":146},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_C)
{
	LoadAndRun("base64://Nz5CDj2JwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":128,"b":0,"c":61,"d":0,"e":0,"h":0,"l":0,"s":146},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_D)
{
	LoadAndRun("base64://Nz5CFj2KwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":128,"b":0,"c":0,"d":61,"e":0,"h":0,"l":0,"s":146},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_E)
{
	LoadAndRun("base64://Nz5CHj2LwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":128,"b":0,"c":0,"d":0,"e":61,"h":0,"l":0,"s":146},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_H)
{
	LoadAndRun("base64://Nz5CJj2MwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":128,"b":0,"c":0,"d":0,"e":0,"h":61,"l":0,"s":146},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_L)
{
	LoadAndRun("base64://Nz5CLj2NwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":128,"b":0,"c":0,"d":0,"e":0,"h":0,"l":61,"s":146},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_M)
{
	LoadAndRun("base64://Nz5CIQIBjsMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":133,"b":0,"c":0,"d":0,"e":0,"h":1,"l":2,"s":130},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADC_A)
{
	LoadAndRun("base64://Nz5Cj8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":133,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":130},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_B)
{
	LoadAndRun("base64://Pj4GPpDDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":62,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_C)
{
	LoadAndRun("base64://Pj4OPpHDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":62,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_D)
{
	LoadAndRun("base64://Pj4WPpLDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":62,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_E)
{
	LoadAndRun("base64://Pj4ePpPDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":62,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_H)
{
	LoadAndRun("base64://Pj4mPpTDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":62,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_L)
{
	LoadAndRun("base64://Pj4uPpXDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":62,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_M)
{
	LoadAndRun("base64://Pj4hAQGWwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":1,"l":1,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUB_A)
{
	LoadAndRun("base64://Pj6XwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_B)
{
	LoadAndRun("base64://Nz4EBgKYwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":2,"c":0,"d":0,"e":0,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_C)
{
	LoadAndRun("base64://Nz4EDgKZwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":0,"c":2,"d":0,"e":0,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_D)
{
	LoadAndRun("base64://Nz4EFgKawwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":0,"c":0,"d":2,"e":0,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_E)
{
	LoadAndRun("base64://Nz4EHgKbwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":0,"c":0,"d":0,"e":2,"h":0,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_H)
{
	LoadAndRun("base64://Nz4EJgKcwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":0,"c":0,"d":0,"e":0,"h":2,"l":0,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_L)
{
	LoadAndRun("base64://Nz4ELgKdwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":1,"b":0,"c":0,"d":0,"e":0,"h":0,"l":2,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_M)
{
	LoadAndRun("base64://Nz4EIQIBnsMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":1,"l":2,"s":135},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SBB_A)
{
	LoadAndRun("base64://Nz4En8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":135},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_B)
{
	LoadAndRun("base64://PvwGD6DDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":12,"b":15,"c":0,"d":0,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_C)
{
	LoadAndRun("base64://PvwOD6HDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":12,"b":0,"c":15,"d":0,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_D)
{
	LoadAndRun("base64://PvwWD6LDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":12,"b":0,"c":0,"d":15,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_E)
{
	LoadAndRun("base64://PvweD6PDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":12,"b":0,"c":0,"d":0,"e":15,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_H)
{
	LoadAndRun("base64://PvwmD6TDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":12,"b":0,"c":0,"d":0,"e":0,"h":15,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_L)
{
	LoadAndRun("base64://PvwuD6XDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":12,"b":0,"c":0,"d":0,"e":0,"h":0,"l":15,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_M)
{
	LoadAndRun("base64://PvwhAgGmwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":32,"b":0,"c":0,"d":0,"e":0,"h":1,"l":2,"s":18},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANA_A)
{
	LoadAndRun("base64://PvynwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":252,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_B)
{
	LoadAndRun("base64://Pv8GD6jDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":240,"b":15,"c":0,"d":0,"e":0,"h":0,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_C)
{
	LoadAndRun("base64://Pv8OD6nDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":240,"b":0,"c":15,"d":0,"e":0,"h":0,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_D)
{
	LoadAndRun("base64://Pv8WD6rDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":240,"b":0,"c":0,"d":15,"e":0,"h":0,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_E)
{
	LoadAndRun("base64://Pv8eD6vDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":240,"b":0,"c":0,"d":0,"e":15,"h":0,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_H)
{
	LoadAndRun("base64://Pv8mD6zDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":240,"b":0,"c":0,"d":0,"e":0,"h":15,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_L)
{
	LoadAndRun("base64://Pv8uD63DAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":240,"b":0,"c":0,"d":0,"e":0,"h":0,"l":15,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_M)
{
	LoadAndRun("base64://Pv8hAgGuwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":222,"b":0,"c":0,"d":0,"e":0,"h":1,"l":2,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XRA_A)
{
	LoadAndRun("base64://PvyvwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":70},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_B)
{
	LoadAndRun("base64://PjMGD7DDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":15,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_C)
{
	LoadAndRun("base64://PjMOD7HDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":0,"c":15,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_D)
{
	LoadAndRun("base64://PjMWD7LDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":0,"c":0,"d":15,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_E)
{
	LoadAndRun("base64://PjMeD7PDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":0,"c":0,"d":0,"e":15,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_H)
{
	LoadAndRun("base64://PjMmD7TDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":0,"c":0,"d":0,"e":0,"h":15,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_L)
{
	LoadAndRun("base64://PjMuD7XDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":0,"c":0,"d":0,"e":0,"h":0,"l":15,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_M)
{
	LoadAndRun("base64://Pv8hAgG2wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":1,"l":2,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ORA_A)
{
	LoadAndRun("base64://Pvy3wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":252,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_B)
{
	LoadAndRun("base64://PgoGBbjDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":5,"c":0,"d":0,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_C)
{
	LoadAndRun("base64://PgoOBbnDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":5,"d":0,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_D)
{
	LoadAndRun("base64://PgoWBbrDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":0,"d":5,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_E)
{
	LoadAndRun("base64://PgoeBbvDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":0,"d":0,"e":5,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_H)
{
	LoadAndRun("base64://PgomBbzDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":0,"d":0,"e":0,"h":5,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_L)
{
	LoadAndRun("base64://PgouBb3DAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":0,"d":0,"e":0,"h":0,"l":5,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CMP_M)
{
	LoadAndRun("base64://PgohAgG+wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":0,"d":0,"e":0,"h":1,"l":2,"s":147},"pc":2,"sp":0})");
}

/*
	Need to fix git settings, seems to be adding a carraige return to this
	test which causes it to fail
*/
#ifdef ENABLE_CMP_A
TEST_F(MachineTest, CMP_A)
{
	LoadAndRun("cmpa.bin", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":211,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}
#endif

TEST_F(MachineTest, RNZ)
{
	LoadAndRun("base64://PgE9zQsBxiDDDAHAwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, PUSH_POP_B)
{
	LoadAndRun("base64://rwHNq8VHT8HDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":171,"c":205,"d":0,"e":0,"h":0,"l":0,"s":70},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JNZ)
{
	LoadAndRun("base64://PgE9wgcBPcMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":134},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JMP)
{
	LoadAndRun("base64://zQgBxiDDCwE+IMk9wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":63,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CNZ)
{
	LoadAndRun("base64://PgE9xAgBPiDGEMMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":48,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADI_1)
{
	LoadAndRun("base64://PhTGQsMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":86,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ADI_2)
{
	LoadAndRun("base64://PhTGQsa+wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":20,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":23},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RST_0)
{
	LoadAndRun("base64://PgHH0/7T/w", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":261,"sp":0})", "base64://PMk", 0x00);
}

TEST_F(MachineTest, RST_1)
{
	LoadAndRun("base64://PgHPwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})", "base64://PMk", 0x08);
}

TEST_F(MachineTest, RST_2)
{
	LoadAndRun("base64://PgHXwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})", "base64://PMk", 0x10);
}

TEST_F(MachineTest, RST_3)
{
	LoadAndRun("base64://PgHfwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})", "base64://PMk", 0x18);
}

TEST_F(MachineTest, RST_4)
{
	LoadAndRun("base64://PgHnwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})", "base64://PMk", 0x20);
}

TEST_F(MachineTest, RST_5)
{
	LoadAndRun("base64://PgHvwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})", "base64://PMk", 0x28);
}

TEST_F(MachineTest, RST_6)
{
	LoadAndRun("base64://PgH3wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})", "base64://PMk", 0x30);
}

TEST_F(MachineTest, RST_7)
{
	LoadAndRun("base64://PgH/wwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":2,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})", "base64://PMk", 0x38);
}

TEST_F(MachineTest, RZ)
{
	LoadAndRun("base64://PgE9zQsBxiDDDAHIwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":32,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RET)
{
	LoadAndRun("base64://zQgBxiDDCwE+IMnDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":64,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JZ)
{
	LoadAndRun("base64://PgE9ygcBPcMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CZ)
{
	LoadAndRun("base64://PgE9zAgBPiDGEMMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, CALL)
{
	LoadAndRun("base64://PhDNBwE+IMYQwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":32,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, ACI_1)
{
	LoadAndRun("base64://PlbOvsMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":20,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":23},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ACI_2)
{
	LoadAndRun("base64://PlbOvs5CwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":87,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RNC)
{
	LoadAndRun("base64://Nz4QzQoBn8MLAdDDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, PUSH_POP_D)
{
	LoadAndRun("base64://rxHNq9VXX9HDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":171,"e":205,"h":0,"l":0,"s":70},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JNC)
{
	LoadAndRun("base64://Nz4Q0gcBn8MAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":135},"pc":2,"sp":0})");	
}

TEST_F(MachineTest, OUT)
{
	LoadAndRun("base64://Pv/TAMMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":2},"pc":2,"sp":0})");

	auto controller = machine_->DetachIoController();
	ASSERT_TRUE(controller);
	//Read the contents of device 0
	EXPECT_EQ(0xFF, controller.value()->Read(0x00, nullptr));
	//Reset the value of the device to it's default (0xAA)
	controller.value()->Write(0x00, 0xAA, nullptr);
	auto err = machine_->AttachIoController(std::move(controller.value()));
	EXPECT_FALSE(err);
}

TEST_F(MachineTest, CNC)
{
	LoadAndRun("base64://Nz4Q1AcBn8MAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":135},"pc":2,"sp":0})");
}

TEST_F(MachineTest, SUI)
{
	LoadAndRun("base64://PgDWAcMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":135},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RC)
{
	LoadAndRun("base64://Nz4QzQoBn8MLAdjDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":255,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":135},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JC)
{
	LoadAndRun("base64://Nz4Q2gcBn8MAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":0})");
}

TEST_F(MachineTest, IN)
{
	LoadAndRun("base64://r9sAwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":70},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CC)
{
	LoadAndRun("base64://Nz4Q3AcBn8MAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":3},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, SBI)
{
	LoadAndRun("base64://rzfeAcMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":254,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":131},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RPO)
{
	LoadAndRun("base64://rz4QzQoBPMMLAeDDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":70},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, PUSH_POP_H)
{
	LoadAndRun("base64://ryHNq+Vnb+HDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":171,"l":205,"s":70},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JPO)
{
	LoadAndRun("base64://rz4Q4gcBPMMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XTHL)
{
	LoadAndRun("base64://Ps0yAAE+qzIBATEAASEjAePDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":171,"b":0,"c":0,"d":0,"e":0,"h":171,"l":205,"s":2},"pc":2,"sp":256})");
}

TEST_F(MachineTest, CPO)
{
	LoadAndRun("base64://rz4Q5AcBPMMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ANI)
{
	LoadAndRun("base64://PjrmD8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":10,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RPE)
{
	LoadAndRun("base64://rz4QzQoBPMMLAejDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, PCHL)
{
	LoadAndRun("base64://PhAhBwHpPDzDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":1,"l":7,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JPE)
{
	LoadAndRun("base64://rz4Q6gcBPMMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":70},"pc":2,"sp":0})");
}

TEST_F(MachineTest, XCHG)
{
	LoadAndRun("base64://FqsezSYBLiPrwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":1,"e":35,"h":171,"l":205,"s":2},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CPE)
{
	LoadAndRun("base64://rz4Q7AcBPMMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":70},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, XRI)
{
	LoadAndRun("base64://PjvugcMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":186,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":130},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RP)
{
	LoadAndRun("base64://vz4QzQoBPMMLAfDDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, PUSH_POP_PSW)
{
	LoadAndRun("base64://v/U+/zzxwwAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

TEST_F(MachineTest, JP)
{
	LoadAndRun("base64://vz4Q8gcBPMMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":0})");
}

/*
TEST_F(MachineTest, DI)
{
	LoadAndRun("di.bin");
}
*/

TEST_F(MachineTest, CP)
{
	LoadAndRun("base64://vz4Q9AcBPMMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, ORI)
{
	LoadAndRun("base64://PrX2D8MAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":191,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":130},"pc":2,"sp":0})");
}

TEST_F(MachineTest, RM)
{
	LoadAndRun("base64://vz4QzQoBPMMLAfjDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":16,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":86},"pc":2,"sp":65534})");
}

TEST_F(MachineTest, SPHL)
{
	LoadAndRun("base64://JgEuI/nDAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":0,"d":0,"e":0,"h":1,"l":35,"s":2},"pc":2,"sp":291})");
}

TEST_F(MachineTest, JM)
{
	LoadAndRun("base64://vz4Q+gcBPMMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

/*
TEST_F(MachineTest, EI)
{
	auto state = LoadAndRun("ei.bin");
}
*/

TEST_F(MachineTest, CM)
{
	LoadAndRun("base64://vz4Q/AcBPMMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":17,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":6},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CPI)
{
	LoadAndRun("base64://Pkr+QMMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":74,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":22},"pc":2,"sp":0})");
}

TEST_F(MachineTest, CPI0)
{
	LoadAndRun("base64://PvX+AMMAAA", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":245,"b":0,"c":0,"d":0,"e":0,"h":0,"l":0,"s":150},"pc":2,"sp":0})");
}

TEST_F(MachineTest, ISR_1)
{
	auto err = machine_->SetOptions(R"(json://{"clockSamplingFreq":40})");
	EXPECT_FALSE(err);

	// There is a chance for an infinite spin if the test fails.
	// TODO: since we fire every second we need to test that the Run
	// method takes a second to complete, pass back the Run value
	// from LoadAndRun.
	LoadAndRun("base64://+6+ouMoDAcMAAA=", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":1,"c":0,"d":0,"e":0,"h":0,"l":0,"s":135},"pc":2,"sp":0})", "base64://+wTJ", 0x08);
}
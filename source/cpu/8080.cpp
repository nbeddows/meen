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

#include <assert.h>
#ifdef ENABLE_MEEN_SAVE
#ifdef ENABLE_NLOHMANN_JSON
#include <nlohmann/json.hpp>
#else
#include <ArduinoJson.h>
#endif // ENABLE_NLOHMANN_JSON
#include "meen/utils/Utils.h"
#endif // ENABLE_MEEN_SAVE
#include "meen/MEEN_Error.h"
#include "meen/cpu/8080.h"

namespace meen
{

Intel8080::Intel8080()
{
#ifdef ENABLE_OPCODE_TABLE
	opcodeTable_ = std::unique_ptr<std::function <uint8_t()>[]>(new std::function <uint8_t()>[256]
	{
		[&] { return Nop(); },
		[&] { return Lxi(b_, c_); },
		[&] { return Stax(b_, c_); },
		[&] { return Inx(b_, c_); },
		[&] { return Inr(b_); },
		[&] { return Dcr(b_); },
		[&] { return Mvi(b_); },
		[&] { return Rlc(); },
		[&] { return NotImplemented(); },
		[&] { return Dad(b_, c_); },
		[&] { return Ldax(b_, c_); },
		[&] { return Dcx(b_, c_); },
		[&] { return Inr(c_); },
		[&] { return Dcr(c_); },
		[&] { return Mvi(c_); },
		[&] { return Rrc(); },
		[&] { return NotImplemented(); },
		[&] { return Lxi(d_, e_); },
		[&] { return Stax(d_, e_); },
		[&] { return Inx(d_, e_); },
		[&] { return Inr(d_); },
		[&] { return Dcr(d_); },
		[&] { return Mvi(d_); },
		[&] { return Ral(); },
		[&] { return NotImplemented(); },
		[&] { return Dad(d_, e_); },
		[&] { return Ldax(d_, e_); },
		[&] { return Dcx(d_, e_); },
		[&] { return Inr(e_); },
		[&] { return Dcr(e_); },
		[&] { return Mvi(e_); },
		[&] { return Rar(); },
		[&] { return NotImplemented(); },
		[&] { return Lxi(h_, l_); },
		[&] { return Shld(); },
		[&] { return Inx(h_, l_); },
		[&] { return Inr(h_); },
		[&] { return Dcr(h_); },
		[&] { return Mvi(h_); },
		[&] { return Daa(); },
		[&] { return NotImplemented(); },
		[&] { return Dad(h_, l_); },
		[&] { return Lhld(); },
		[&] { return Dcx(h_, l_); },
		[&] { return Inr(l_); },
		[&] { return Dcr(l_); },
		[&] { return Mvi(l_); },
		[&] { return Cma(); },
		[&] { return NotImplemented(); },
		[&] { return Lxi(); },
		[&] { return Sta(); },
		[&] { return Inx(); },
		[&] { return Inr(); },
		[&] { return Dcr(Uint16(h_, l_)); },
		[&] { return Mvi(); },
		[&] { return Stc(); },
		[&] { return NotImplemented(); },
		[&] { return Dad(); },
		[&] { return Lda(); },
		[&] { return Dcx(); },
		[&] { return Inr(a_); },
		[&] { return Dcr(a_); },
		[&] { return Mvi(a_); },
		[&] { return Cmc(); },
		[&] { return Nop(); },
		[&] { return Mov(b_, c_); },
		[&] { return Mov(b_, d_); },
		[&] { return Mov(b_, e_); },
		[&] { return Mov(b_, h_); },
		[&] { return Mov(b_, l_); },
		[&] { return Mov(b_); },
		[&] { return Mov(b_, a_); },
		[&] { return Mov(c_, b_); },
		[&] { return Nop(); },
		[&] { return Mov(c_, d_); },
		[&] { return Mov(c_, e_); },
		[&] { return Mov(c_, h_); },
		[&] { return Mov(c_, l_); },
		[&] { return Mov(c_); },
		[&] { return Mov(c_, a_); },
		[&] { return Mov(d_, b_); },
		[&] { return Mov(d_, c_); },
		[&] { return Nop(); },
		[&] { return Mov(d_, e_); },
		[&] { return Mov(d_, h_); },
		[&] { return Mov(d_, l_); },
		[&] { return Mov(d_); },
		[&] { return Mov(d_, a_); },
		[&] { return Mov(e_, b_); },
		[&] { return Mov(e_, c_); },
		[&] { return Mov(e_, d_); },
		[&] { return Nop(); },
		[&] { return Mov(e_, h_); },
		[&] { return Mov(e_, l_); },
		[&] { return Mov(e_); },
		[&] { return Mov(e_, a_); },
		[&] { return Mov(h_, b_); },
		[&] { return Mov(h_, c_); },
		[&] { return Mov(h_, d_); },
		[&] { return Mov(h_, e_); },
		[&] { return Nop(); },
		[&] { return Mov(h_, l_); },
		[&] { return Mov(h_); },
		[&] { return Mov(h_, a_); },
		[&] { return Mov(l_, b_); },
		[&] { return Mov(l_, c_); },
		[&] { return Mov(l_, d_); },
		[&] { return Mov(l_, e_); },
		[&] { return Mov(l_, h_); },
		[&] { return Nop(); },
		[&] { return Mov(l_); },
		[&] { return Mov(l_, a_); },
		[&] { return Mov(Value(b_)); },
		[&] { return Mov(Value(c_)); },
		[&] { return Mov(Value(d_)); },
		[&] { return Mov(Value(e_)); },
		[&] { return Mov(Value(h_)); },
		[&] { return Mov(Value(l_)); },
		[&] { return Hlt(); },
		[&] { return Mov(Value(a_)); },
		[&] { return Mov(a_, b_); },
		[&] { return Mov(a_, c_); },
		[&] { return Mov(a_, d_); },
		[&] { return Mov(a_, e_); },
		[&] { return Mov(a_, h_); },
		[&] { return Mov(a_, l_); },
		[&] { return Mov(a_); },
		[&] { return Nop(); },
		[&] { return Add(b_, "ADD"); },
		[&] { return Add(c_, "ADD"); },
		[&] { return Add(d_, "ADD"); },
		[&] { return Add(e_, "ADD"); },
		[&] { return Add(h_, "ADD"); },
		[&] { return Add(l_, "ADD"); },
		[&] { return Add(Uint16(h_, l_), "ADD"); },
		[&] { return Add(a_, "ADD"); },
		[&] { return Adc(b_, "ADC"); },
		[&] { return Adc(c_, "ADC"); },
		[&] { return Adc(d_, "ADC"); },
		[&] { return Adc(e_, "ADC"); },
		[&] { return Adc(h_, "ADC"); },
		[&] { return Adc(l_, "ADC"); },
		[&] { return Adc(Uint16(h_, l_), "ADC"); },
		[&] { return Adc(a_, "ADC"); },
		[&] { return Sub(b_, "SUB"); },
		[&] { return Sub(c_, "SUB"); },
		[&] { return Sub(d_, "SUB"); },
		[&] { return Sub(e_, "SUB"); },
		[&] { return Sub(h_, "SUB"); },
		[&] { return Sub(l_, "SUB"); },
		[&] { return Sub(Uint16(h_, l_), "SUB"); },
		[&] { return Sub(a_, "SUB"); },
		[&] { return Sbb(b_, "SBB"); },
		[&] { return Sbb(c_, "SBB"); },
		[&] { return Sbb(d_, "SBB"); },
		[&] { return Sbb(e_, "SBB"); },
		[&] { return Sbb(h_, "SBB"); },
		[&] { return Sbb(l_, "SBB"); },
		[&] { return Sbb(Uint16(h_, l_), "SBB"); },
		[&] { return Sbb(a_, "SBB"); },
		[&] { return Ana(b_, "ANA"); },
		[&] { return Ana(c_, "ANA"); },
		[&] { return Ana(d_, "ANA"); },
		[&] { return Ana(e_, "ANA"); },
		[&] { return Ana(h_, "ANA"); },
		[&] { return Ana(l_, "ANA"); },
		[&] { return Ana(Uint16(h_, l_), "ANA"); },
		[&] { return Ana(a_, "ANA"); },
		[&] { return Xra(b_, "XRA"); },
		[&] { return Xra(c_, "XRA"); },
		[&] { return Xra(d_, "XRA"); },
		[&] { return Xra(e_, "XRA"); },
		[&] { return Xra(h_, "XRA"); },
		[&] { return Xra(l_, "XRA"); },
		[&] { return Xra(Uint16(h_, l_), "XRA"); },
		[&] { return Xra(a_, "XRA"); },
		[&] { return Ora(b_, "ORA"); },
		[&] { return Ora(c_, "ORA"); },
		[&] { return Ora(d_, "ORA"); },
		[&] { return Ora(e_, "ORA"); },
		[&] { return Ora(h_, "ORA"); },
		[&] { return Ora(l_, "ORA"); },
		[&] { return Ora(Uint16(h_, l_), "ORA"); },
		[&] { return Ora(a_, "ORA"); },
		[&] { return Cmp(b_, "CMP"); },
		[&] { return Cmp(c_, "CMP"); },
		[&] { return Cmp(d_, "CMP"); },
		[&] { return Cmp(e_, "CMP"); },
		[&] { return Cmp(h_, "CMP"); },
		[&] { return Cmp(l_, "CMP"); },
		[&] { return Cmp(Uint16(h_, l_), "CMP"); },
		[&] { return Cmp(a_, "CMP"); },
		[&] { return RetOnFlag(status_.test(Condition::ZeroFlag) == false, "RNZ"); },
		[&] { return Pop(b_, c_); },
		[&] { return JmpOnFlag(status_.test(Condition::ZeroFlag) == false, "JNZ"); },
		[&] { return JmpOnFlag(true, "JMP"); },
		[&] { return CallOnFlag(status_.test(Condition::ZeroFlag) == false, "CNZ"); },
		[&] { return Push(b_, c_); },
		[&] { return Add(++pc_, "ADI"); },
		[&] { return Rst(); },
		[&] { return RetOnFlag(status_.test(Condition::ZeroFlag) == true, "RZ"); },
		[&] { return RetOnFlag(true, "RET"); },
		[&] { return JmpOnFlag(status_.test(Condition::ZeroFlag) == true, "JZ"); },
		[&] { return NotImplemented(); },
		[&] { return CallOnFlag(status_.test(Condition::ZeroFlag) == true, "CZ"); },
		[&] { return CallOnFlag(true, "CALL"); },
		[&] { return Adc(++pc_, "ACI"); },
		[&] { return Rst(); },
		[&] { return RetOnFlag(status_.test(Condition::CarryFlag) == false, "RNC"); },
		[&] { return Pop(d_, e_); },
		[&] { return JmpOnFlag(status_.test(Condition::CarryFlag) == false, "JNC"); },
		[&] { return Out(); },
		[&] { return CallOnFlag(status_.test(Condition::CarryFlag) == false, "CNC"); },
		[&] { return Push(d_, e_); },
		[&] { return Sub(++pc_, "SUI"); },
		[&] { return Rst(); },
		[&] { return RetOnFlag(status_.test(Condition::CarryFlag) == true, "RC"); },
		[&] { return NotImplemented(); },
		[&] { return JmpOnFlag(status_.test(Condition::CarryFlag) == true, "JC"); },
		[&] { return In(); },
		[&] { return CallOnFlag(status_.test(Condition::CarryFlag) == true, "CC"); },
		[&] { return NotImplemented(); },
		[&] { return Sbb(++pc_, "SBI"); },
		[&] { return Rst(); },
		[&] { return RetOnFlag(status_.test(Condition::ParityFlag) == false, "RPO"); },
		[&] { return Pop(h_, l_); },
		[&] { return JmpOnFlag(status_.test(Condition::ParityFlag) == false, "JPO"); },
		[&] { return Xthl(); },
		[&] { return CallOnFlag(status_.test(Condition::ParityFlag) == false, "CPO"); },
		[&] { return Push(h_, l_); },
		[&] { return Ana(++pc_, "ANI"); },
		[&] { return Rst(); },
		[&] { return RetOnFlag(status_.test(Condition::ParityFlag) == true, "RPE"); },
		[&] { return Pchl(); },
		[&] { return JmpOnFlag(status_.test(Condition::ParityFlag) == true, "JPE"); },
		[&] { return Xchg(); },
		[&] { return CallOnFlag(status_.test(Condition::ParityFlag) == true, "CPE"); },
		[&] { return NotImplemented(); },
		[&] { return Xra(++pc_, "XRI"); },
		[&] { return Rst(); },
		[&] { return RetOnFlag(status_.test(Condition::SignFlag) == false, "RP"); },
		[&] { auto ticks = 0; ticks = Pop(a_, status_); status_ = (status_ & Register(0xD7)) | Register(0x02); return ticks; },
		[&] { return JmpOnFlag(status_.test(Condition::SignFlag) == false, "JP"); },
		[&] { return Di(); },
		[&] { return CallOnFlag(status_.test(Condition::SignFlag) == false, "CP"); },
		[&] { return Push(a_, status_); },
		[&] { return Ora(++pc_, "ORI"); },
		[&] { return Rst(); },
		[&] { return RetOnFlag(status_.test(Condition::SignFlag) == true, "RM"); },
		[&] { return Sphl(); },
		[&] { return JmpOnFlag(status_.test(Condition::SignFlag) == true, "JM"); },
		[&] { return Ei(); },
		[&] { return CallOnFlag(status_.test(Condition::SignFlag) == true, "CM"); },
		[&] { return NotImplemented(); },
		[&] { return Cmp(++pc_, "CPI"); },
		[&] { return Rst(); },
	});
#endif
}

std::unique_ptr<uint8_t[]> Intel8080::GetState(int* size) const
{
	auto state = std::make_unique<uint8_t[]>(12);

	state[0] = Value(a_);
	state[1] = Value(b_);
	state[2] = Value(c_);
	state[3] = Value(d_);
	state[4] = Value(e_);
	state[5] = Value(h_);
	state[6] = Value(l_);
	state[7] = Value(status_);
	state[8] = static_cast<uint8_t>(pc_ >> 8);
	state[9] = static_cast<uint8_t>(pc_ & 0xFF);
	state[10] = static_cast<uint8_t>(sp_ >> 8);
	state[11] = static_cast<uint8_t>(sp_ & 0xFF);

	if (size != nullptr)
	{
		*size = 12;
	}

	return state;
}

#ifdef ENABLE_MEEN_SAVE
std::error_code Intel8080::Load(const std::string&& str)
{
#ifdef ENABLE_NLOHMANN_JSON
	auto json = nlohmann::json::parse(str, nullptr, false);

	if(json.is_discarded() == true)
	{
		return make_error_code(errc::json_parse);
	}

	if(!json.contains("uuid"))
	{
		return make_error_code(errc::json_parse);
	}

	// The cpus must be the same
	auto jsonUuid = Utils::TxtToBin("base64", "none", 16, json["uuid"].get<std::string>());

	if (jsonUuid.size() != uuid_.size() || std::equal(jsonUuid.begin(), jsonUuid.end(), uuid_.begin()) == false)
	{
		return make_error_code(errc::incompatible_uuid);
	}

	// Make sure everything exists and is copied out
	if(!json.contains("registers") || !json.contains("pc") || !json.contains("sp"))
	{
		return make_error_code(errc::json_parse);
	}

	auto registers = json["registers"];

	if(!registers.contains("a") || !registers.contains("b") || !registers.contains("c") ||
	   !registers.contains("d") || !registers.contains("e") || !registers.contains("h") ||
	   !registers.contains("l") || !registers.contains("s"))
	{
		return make_error_code(errc::json_parse);
	}

	// Restore the state of the cpu
	a_ = registers["a"].get<uint8_t>();
	b_ = registers["b"].get<uint8_t>();
	c_ = registers["c"].get<uint8_t>();
	d_ = registers["d"].get<uint8_t>();
	e_ = registers["e"].get<uint8_t>();
	h_ = registers["h"].get<uint8_t>();
	l_ = registers["l"].get<uint8_t>();
	status_ = registers["s"].get<uint8_t>();
	pc_ = json["pc"].get<uint16_t>();
	sp_ = json["sp"].get<uint16_t>();
#else
	JsonDocument json;
	auto e = deserializeJson(json, str);

	if(e)
	{
		return make_error_code(errc::json_parse);
	}

	if(json["uuid"] == nullptr)
	{
		return make_error_code(errc::json_parse);
	}

	// The cpus must be the same
	auto jsonUuid = Utils::TxtToBin("base64", "none", 16, json["uuid"].as<std::string>());

	if (jsonUuid.size() != uuid_.size() || std::equal(jsonUuid.begin(), jsonUuid.end(), uuid_.begin()) == false)
	{
		return make_error_code(errc::incompatible_uuid);
	}

	// Make sure everything exists and is copied out
	if(json["registers"] == nullptr || json["pc"] == nullptr || json["sp"] == nullptr)
	{
		return make_error_code(errc::json_parse);
	}

	auto registers = json["registers"];

	if(registers["a"] == nullptr || registers["b"] == nullptr || registers["c"] == nullptr ||
	   registers["d"] == nullptr || registers["e"] == nullptr || registers["h"] == nullptr ||
	   registers["l"] == nullptr || registers["s"] == nullptr)
	{
		return make_error_code(errc::json_parse);
	}

	// Restore the state of the cpu
	a_ = registers["a"].as<uint8_t>();
	b_ = registers["b"].as<uint8_t>();
	c_ = registers["c"].as<uint8_t>();
	d_ = registers["d"].as<uint8_t>();
	e_ = registers["e"].as<uint8_t>();
	h_ = registers["h"].as<uint8_t>();
	l_ = registers["l"].as<uint8_t>();
	status_ = registers["s"].as<uint8_t>();
	pc_ = json["pc"].as<uint16_t>();
	sp_ = json["sp"].as<uint16_t>();
#endif
	return make_error_code(errc::no_error);
}

std::string Intel8080::Save() const
{
	auto b64 = Utils::BinToTxt("base64", "none", uuid_.data(), uuid_.size());
	auto fmtStr = "{\"uuid\":\"%s\",\"registers\":{\"a\":%d,\"b\":%d,\"c\":%d,\"d\":%d,\"e\":%d,\"h\":%d,\"l\":%d,\"s\":%d},\"pc\":%d,\"sp\":%d}";
	auto count = snprintf(nullptr, 0, fmtStr, b64.c_str(), Value(a_), Value(b_), Value(c_), Value(d_), Value(e_), Value(h_), Value(l_), Value(status_), pc_, sp_);
	std::string str(count + 1, '\0');
	snprintf(str.data(), count + 1, fmtStr, b64.c_str(), Value(a_), Value(b_), Value(c_), Value(d_), Value(e_), Value(h_), Value(l_), Value(status_), pc_, sp_);
	return str;
}
#endif // ENABLE_MEEN_SAVE

uint8_t Intel8080::Interrupt(ISR isr)
{
	uint8_t timePeriods = 0;

	if (iff_ == true)
	{
		timePeriods = Rst(0xC7 | (static_cast<uint8_t>(isr) << 3));
		//the interrupt enable system is automatically
		//disabled whenever an interrupt is acknowledged
		iff_ = false;
	}

	return timePeriods;
}

uint8_t Intel8080::Execute()
{
	opcode_ = memoryController_->Read(pc_);

#ifdef ENABLE_OPCODE_TABLE
	return opcodeTable_[opcode_]();
#else
	uint8_t timePeriods = 0;

	switch(opcode_)
	{
		case 0x00: timePeriods = Nop(); break;
		case 0x01: timePeriods = Lxi(b_, c_); break;
		case 0x02: timePeriods = Stax(b_, c_); break;
		case 0x03: timePeriods = Inx(b_, c_); break;
		case 0x04: timePeriods = Inr(b_); break;
		case 0x05: timePeriods = Dcr(b_); break;
		case 0x06: timePeriods = Mvi(b_); break;
		case 0x07: timePeriods = Rlc(); break;
		case 0x08: timePeriods = NotImplemented(); break;
		case 0x09: timePeriods = Dad(b_, c_); break;
		case 0x0A: timePeriods = Ldax(b_, c_); break;
		case 0x0B: timePeriods = Dcx(b_, c_); break;
		case 0x0C: timePeriods = Inr(c_); break;
		case 0x0D: timePeriods = Dcr(c_); break;
		case 0x0E: timePeriods = Mvi(c_); break;
		case 0x0F: timePeriods = Rrc(); break;
		case 0x10: timePeriods = NotImplemented(); break;
		case 0x11: timePeriods = Lxi(d_, e_); break;
		case 0x12: timePeriods = Stax(d_, e_); break;
		case 0x13: timePeriods = Inx(d_, e_); break;
		case 0x14: timePeriods = Inr(d_); break;
		case 0x15: timePeriods = Dcr(d_); break;
		case 0x16: timePeriods = Mvi(d_); break;
		case 0x17: timePeriods = Ral(); break;
		case 0x18: timePeriods = NotImplemented(); break;
		case 0x19: timePeriods = Dad(d_, e_); break;
		case 0x1A: timePeriods = Ldax(d_, e_); break;
		case 0x1B: timePeriods = Dcx(d_, e_); break;
		case 0x1C: timePeriods = Inr(e_); break;
		case 0x1D: timePeriods = Dcr(e_); break;
		case 0x1E: timePeriods = Mvi(e_); break;
		case 0x1F: timePeriods = Rar(); break;
		case 0x20: timePeriods = NotImplemented(); break;
		case 0x21: timePeriods = Lxi(h_, l_); break;
		case 0x22: timePeriods = Shld(); break;
		case 0x23: timePeriods = Inx(h_, l_); break;
		case 0x24: timePeriods = Inr(h_); break;
		case 0x25: timePeriods = Dcr(h_); break;
		case 0x26: timePeriods = Mvi(h_); break;
		case 0x27: timePeriods = Daa(); break;
		case 0x28: timePeriods = NotImplemented(); break;
		case 0x29: timePeriods = Dad(h_, l_); break;
		case 0x2A: timePeriods = Lhld(); break;
		case 0x2B: timePeriods = Dcx(h_, l_); break;
		case 0x2C: timePeriods = Inr(l_); break;
		case 0x2D: timePeriods = Dcr(l_); break;
		case 0x2E: timePeriods = Mvi(l_); break;
		case 0x2F: timePeriods = Cma(); break;
		case 0x30: timePeriods = NotImplemented(); break;
		case 0x31: timePeriods = Lxi(); break;
		case 0x32: timePeriods = Sta(); break;
		case 0x33: timePeriods = Inx(); break;
		case 0x34: timePeriods = Inr(); break;
		case 0x35: timePeriods = Dcr(Uint16(h_, l_)); break;
		case 0x36: timePeriods = Mvi(); break;
		case 0x37: timePeriods = Stc(); break;
		case 0x38: timePeriods = NotImplemented(); break;
		case 0x39: timePeriods = Dad(); break;
		case 0x3A: timePeriods = Lda(); break;
		case 0x3B: timePeriods = Dcx(); break;
		case 0x3C: timePeriods = Inr(a_); break;
		case 0x3D: timePeriods = Dcr(a_); break;
		case 0x3E: timePeriods = Mvi(a_); break;
		case 0x3F: timePeriods = Cmc(); break;
		case 0x40: timePeriods = Nop(); break;
		case 0x41: timePeriods = Mov(b_, c_); break;
		case 0x42: timePeriods = Mov(b_, d_); break;
		case 0x43: timePeriods = Mov(b_, e_); break;
		case 0x44: timePeriods = Mov(b_, h_); break;
		case 0x45: timePeriods = Mov(b_, l_); break;
		case 0x46: timePeriods = Mov(b_); break;
		case 0x47: timePeriods = Mov(b_, a_); break;
		case 0x48: timePeriods = Mov(c_, b_); break;
		case 0x49: timePeriods = Nop(); break;
		case 0x4A: timePeriods = Mov(c_, d_); break;
		case 0x4B: timePeriods = Mov(c_, e_); break;
		case 0x4C: timePeriods = Mov(c_, h_); break;
		case 0x4D: timePeriods = Mov(c_, l_); break;
		case 0x4E: timePeriods = Mov(c_); break;
		case 0x4F: timePeriods = Mov(c_, a_); break;
		case 0x50: timePeriods = Mov(d_, b_); break;
		case 0x51: timePeriods = Mov(d_, c_); break;
		case 0x52: timePeriods = Nop(); break;
		case 0x53: timePeriods = Mov(d_, e_); break;
		case 0x54: timePeriods = Mov(d_, h_); break;
		case 0x55: timePeriods = Mov(d_, l_); break;
		case 0x56: timePeriods = Mov(d_); break;
		case 0x57: timePeriods = Mov(d_, a_); break;
		case 0x58: timePeriods = Mov(e_, b_); break;
		case 0x59: timePeriods = Mov(e_, c_); break;
		case 0x5A: timePeriods = Mov(e_, d_); break;
		case 0x5B: timePeriods = Nop(); break;
		case 0x5C: timePeriods = Mov(e_, h_); break;
		case 0x5D: timePeriods = Mov(e_, l_); break;
		case 0x5E: timePeriods = Mov(e_); break;
		case 0x5F: timePeriods = Mov(e_, a_); break;
		case 0x60: timePeriods = Mov(h_, b_); break;
		case 0x61: timePeriods = Mov(h_, c_); break;
		case 0x62: timePeriods = Mov(h_, d_); break;
		case 0x63: timePeriods = Mov(h_, e_); break;
		case 0x64: timePeriods = Nop(); break;
		case 0x65: timePeriods = Mov(h_, l_); break;
		case 0x66: timePeriods = Mov(h_); break;
		case 0x67: timePeriods = Mov(h_, a_); break;
		case 0x68: timePeriods = Mov(l_, b_); break;
		case 0x69: timePeriods = Mov(l_, c_); break;
		case 0x6A: timePeriods = Mov(l_, d_); break;
		case 0x6B: timePeriods = Mov(l_, e_); break;
		case 0x6C: timePeriods = Mov(l_, h_); break;
		case 0x6D: timePeriods = Nop(); break;
		case 0x6E: timePeriods = Mov(l_); break;
		case 0x6F: timePeriods = Mov(l_, a_); break;
		case 0x70: timePeriods = Mov(Value(b_)); break;
		case 0x71: timePeriods = Mov(Value(c_)); break;
		case 0x72: timePeriods = Mov(Value(d_)); break;
		case 0x73: timePeriods = Mov(Value(e_)); break;
		case 0x74: timePeriods = Mov(Value(h_)); break;
		case 0x75: timePeriods = Mov(Value(l_)); break;
		case 0x76: timePeriods = Hlt(); break;
		case 0x77: timePeriods = Mov(Value(a_)); break;
		case 0x78: timePeriods = Mov(a_, b_); break;
		case 0x79: timePeriods = Mov(a_, c_); break;
		case 0x7A: timePeriods = Mov(a_, d_); break;
		case 0x7B: timePeriods = Mov(a_, e_); break;
		case 0x7C: timePeriods = Mov(a_, h_); break;
		case 0x7D: timePeriods = Mov(a_, l_); break;
		case 0x7E: timePeriods = Mov(a_); break;
		case 0x7F: timePeriods = Nop(); break;
		case 0x80: timePeriods = Add(b_, "ADD"); break;
		case 0x81: timePeriods = Add(c_, "ADD"); break;
		case 0x82: timePeriods = Add(d_, "ADD"); break;
		case 0x83: timePeriods = Add(e_, "ADD"); break;
		case 0x84: timePeriods = Add(h_, "ADD"); break;
		case 0x85: timePeriods = Add(l_, "ADD"); break;
		case 0x86: timePeriods = Add(Uint16(h_, l_), "ADD"); break;
		case 0x87: timePeriods = Add(a_, "ADD"); break;
		case 0x88: timePeriods = Adc(b_, "ADC"); break;
		case 0x89: timePeriods = Adc(c_, "ADC"); break;
		case 0x8A: timePeriods = Adc(d_, "ADC"); break;
		case 0x8B: timePeriods = Adc(e_, "ADC"); break;
		case 0x8C: timePeriods = Adc(h_, "ADC"); break;
		case 0x8D: timePeriods = Adc(l_, "ADC"); break;
		case 0x8E: timePeriods = Adc(Uint16(h_, l_), "ADC"); break;
		case 0x8F: timePeriods = Adc(a_, "ADC"); break;
		case 0x90: timePeriods = Sub(b_, "SUB"); break;
		case 0x91: timePeriods = Sub(c_, "SUB"); break;
		case 0x92: timePeriods = Sub(d_, "SUB"); break;
		case 0x93: timePeriods = Sub(e_, "SUB"); break;
		case 0x94: timePeriods = Sub(h_, "SUB"); break;
		case 0x95: timePeriods = Sub(l_, "SUB"); break;
		case 0x96: timePeriods = Sub(Uint16(h_, l_), "SUB"); break;
		case 0x97: timePeriods = Sub(a_, "SUB"); break;
		case 0x98: timePeriods = Sbb(b_, "SBB"); break;
		case 0x99: timePeriods = Sbb(c_, "SBB"); break;
		case 0x9A: timePeriods = Sbb(d_, "SBB"); break;
		case 0x9B: timePeriods = Sbb(e_, "SBB"); break;
		case 0x9C: timePeriods = Sbb(h_, "SBB"); break;
		case 0x9D: timePeriods = Sbb(l_, "SBB"); break;
		case 0x9E: timePeriods = Sbb(Uint16(h_, l_), "SBB"); break;
		case 0x9F: timePeriods = Sbb(a_, "SBB"); break;
		case 0xA0: timePeriods = Ana(b_, "ANA"); break;
		case 0xA1: timePeriods = Ana(c_, "ANA"); break;
		case 0xA2: timePeriods = Ana(d_, "ANA"); break;
		case 0xA3: timePeriods = Ana(e_, "ANA"); break;
		case 0xA4: timePeriods = Ana(h_, "ANA"); break;
		case 0xA5: timePeriods = Ana(l_, "ANA"); break;
		case 0xA6: timePeriods = Ana(Uint16(h_, l_), "ANA"); break;
		case 0xA7: timePeriods = Ana(a_, "ANA"); break;
		case 0xA8: timePeriods = Xra(b_, "XRA"); break;
		case 0xA9: timePeriods = Xra(c_, "XRA"); break;
		case 0xAA: timePeriods = Xra(d_, "XRA"); break;
		case 0xAB: timePeriods = Xra(e_, "XRA"); break;
		case 0xAC: timePeriods = Xra(h_, "XRA"); break;
		case 0xAD: timePeriods = Xra(l_, "XRA"); break;
		case 0xAE: timePeriods = Xra(Uint16(h_, l_), "XRA"); break;
		case 0xAF: timePeriods = Xra(a_, "XRA"); break;
		case 0xB0: timePeriods = Ora(b_, "ORA"); break;
		case 0xB1: timePeriods = Ora(c_, "ORA"); break;
		case 0xB2: timePeriods = Ora(d_, "ORA"); break;
		case 0xB3: timePeriods = Ora(e_, "ORA"); break;
		case 0xB4: timePeriods = Ora(h_, "ORA"); break;
		case 0xB5: timePeriods = Ora(l_, "ORA"); break;
		case 0xB6: timePeriods = Ora(Uint16(h_, l_), "ORA"); break;
		case 0xB7: timePeriods = Ora(a_, "ORA"); break;
		case 0xB8: timePeriods = Cmp(b_, "CMP"); break;
		case 0xB9: timePeriods = Cmp(c_, "CMP"); break;
		case 0xBA: timePeriods = Cmp(d_, "CMP"); break;
		case 0xBB: timePeriods = Cmp(e_, "CMP"); break;
		case 0xBC: timePeriods = Cmp(h_, "CMP"); break;
		case 0xBD: timePeriods = Cmp(l_, "CMP"); break;
		case 0xBE: timePeriods = Cmp(Uint16(h_, l_), "CMP"); break;
		case 0xBF: timePeriods = Cmp(a_, "CMP"); break;
		case 0xC0: timePeriods = RetOnFlag(status_.test(Condition::ZeroFlag) == false, "RNZ"); break;
		case 0xC1: timePeriods = Pop(b_, c_); break;
		case 0xC2: timePeriods = JmpOnFlag(status_.test(Condition::ZeroFlag) == false, "JNZ"); break;
		case 0xC3: timePeriods = JmpOnFlag(true, "JMP"); break;
		case 0xC4: timePeriods = CallOnFlag(status_.test(Condition::ZeroFlag) == false, "CNZ"); break;
		case 0xC5: timePeriods = Push(b_, c_); break;
		case 0xC6: timePeriods = Add(++pc_, "ADI"); break;
		case 0xC7: timePeriods = Rst(); break;
		case 0xC8: timePeriods = RetOnFlag(status_.test(Condition::ZeroFlag) == true, "RZ"); break;
		case 0xC9: timePeriods = RetOnFlag(true, "RET"); break;
		case 0xCA: timePeriods = JmpOnFlag(status_.test(Condition::ZeroFlag) == true, "JZ"); break;
		case 0xCB: timePeriods = NotImplemented(); break;
		case 0xCC: timePeriods = CallOnFlag(status_.test(Condition::ZeroFlag) == true, "CZ"); break;
		case 0xCD: timePeriods = CallOnFlag(true, "CALL"); break;
		case 0xCE: timePeriods = Adc(++pc_, "ACI"); break;
		case 0xCF: timePeriods = Rst(); break;
		case 0xD0: timePeriods = RetOnFlag(status_.test(Condition::CarryFlag) == false, "RNC"); break;
		case 0xD1: timePeriods = Pop(d_, e_); break;
		case 0xD2: timePeriods = JmpOnFlag(status_.test(Condition::CarryFlag) == false, "JNC"); break;
		case 0xD3: timePeriods = Out(); break;
		case 0xD4: timePeriods = CallOnFlag(status_.test(Condition::CarryFlag) == false, "CNC"); break;
		case 0xD5: timePeriods = Push(d_, e_); break;
		case 0xD6: timePeriods = Sub(++pc_, "SUI"); break;
		case 0xD7: timePeriods = Rst(); break;
		case 0xD8: timePeriods = RetOnFlag(status_.test(Condition::CarryFlag) == true, "RC"); break;
		case 0xD9: timePeriods = NotImplemented(); break;
		case 0xDA: timePeriods = JmpOnFlag(status_.test(Condition::CarryFlag) == true, "JC"); break;
		case 0xDB: timePeriods = In(); break;
		case 0xDC: timePeriods = CallOnFlag(status_.test(Condition::CarryFlag) == true, "CC"); break;
		case 0xDD: timePeriods = NotImplemented(); break;
		case 0xDE: timePeriods = Sbb(++pc_, "SBI"); break;
		case 0xDF: timePeriods = Rst(); break;
		case 0xE0: timePeriods = RetOnFlag(status_.test(Condition::ParityFlag) == false, "RPO"); break;
		case 0xE1: timePeriods = Pop(h_, l_); break;
		case 0xE2: timePeriods = JmpOnFlag(status_.test(Condition::ParityFlag) == false, "JPO"); break;
		case 0xE3: timePeriods = Xthl(); break;
		case 0xE4: timePeriods = CallOnFlag(status_.test(Condition::ParityFlag) == false, "CPO"); break;
		case 0xE5: timePeriods = Push(h_, l_); break;
		case 0xE6: timePeriods = Ana(++pc_, "ANI"); break;
		case 0xE7: timePeriods = Rst(); break;
		case 0xE8: timePeriods = RetOnFlag(status_.test(Condition::ParityFlag) == true, "RPE"); break;
		case 0xE9: timePeriods = Pchl(); break;
		case 0xEA: timePeriods = JmpOnFlag(status_.test(Condition::ParityFlag) == true, "JPE"); break;
		case 0xEB: timePeriods = Xchg(); break;
		case 0xEC: timePeriods = CallOnFlag(status_.test(Condition::ParityFlag) == true, "CPE"); break;
		case 0xED: timePeriods = NotImplemented(); break;
		case 0xEE: timePeriods = Xra(++pc_, "XRI"); break;
		case 0xEF: timePeriods = Rst(); break;
		case 0xF0: timePeriods = RetOnFlag(status_.test(Condition::SignFlag) == false, "RP"); break;
		case 0xF1: timePeriods = Pop(a_, status_); status_ = (status_ & Register(0xD7)) | Register(0x02); break;
		case 0xF2: timePeriods = JmpOnFlag(status_.test(Condition::SignFlag) == false, "JP"); break;
		case 0xF3: timePeriods = Di(); break;
		case 0xF4: timePeriods = CallOnFlag(status_.test(Condition::SignFlag) == false, "CP"); break;
		case 0xF5: timePeriods = Push(a_, status_); break;
		case 0xF6: timePeriods = Ora(++pc_, "ORI"); break;
		case 0xF7: timePeriods = Rst(); break;
		case 0xF8: timePeriods = RetOnFlag(status_.test(Condition::SignFlag) == true, "RM"); break;
		case 0xF9: timePeriods = Sphl(); break;
		case 0xFA: timePeriods = JmpOnFlag(status_.test(Condition::SignFlag) == true, "JM"); break;
		case 0xFB: timePeriods = Ei(); break;
		case 0xFC: timePeriods = CallOnFlag(status_.test(Condition::SignFlag) == true, "CM"); break;
		case 0xFD: timePeriods = NotImplemented(); break;
		case 0xFE: timePeriods = Cmp(++pc_, "CPI"); break;
		case 0xFF: timePeriods = Rst(); break;
		default: assert(0); break;
	}

	return timePeriods;
#endif
}

std::error_code Intel8080::SetMemoryController(const std::shared_ptr<IController>& memoryController)
{
	if(memoryController == nullptr)
	{
		return make_error_code(errc::invalid_argument);
	}

	memoryController_ = memoryController;

	return make_error_code(errc::no_error);
}

std::error_code Intel8080::SetIoController(const std::shared_ptr<IController>& ioController)
{
	if(ioController == nullptr)
	{
		return make_error_code(errc::invalid_argument);
	}

	ioController_ = ioController;

	return make_error_code(errc::no_error);
}

//This essentially powers on the cpu
void Intel8080::Reset(uint16_t pc)
{
	a_.reset();
	b_.reset();
	c_.reset();
	d_.reset();
	e_.reset();
	h_.reset();
	l_.reset();
	pc_ = pc;
	sp_ = 0;
	status_ = 0b00000010;
	iff_ = false;
}

/**
	INR

	The specified register or memory byte is incremented by one.
*/
uint8_t Intel8080::Inr(Register& r)
{
	r = Add(r, 0x01, 0, false, "INR");
	return 5;
}

uint8_t Intel8080::Inr()
{
	auto addr = Uint16(h_, l_);
	Register r = memoryController_->Read(addr);
	r = Add(r, 0x01, 0, false, "INR");
	//Inr(r);
	memoryController_->Write(addr, Value(r));
	return 10;
}

/**
	DCR

	The specified register or memory byte is
	decremented by one.
*/
uint8_t Intel8080::Dcr(Register& r)
{
	//Using twos compliment add for subtraciton.
	r = Add(r, 0xFF, 0, false, "DCR");
	return 5;
}

uint8_t Intel8080::Dcr(uint16_t addr)
{
	Register r = memoryController_->Read(addr);
	r = Add(r, 0xFF, 0, false, "DCR");
	//Dcr(r);
	memoryController_->Write(addr, Value(r));
	return 10;
}

uint8_t Intel8080::Mvi(Register& reg)
{
	reg = memoryController_->Read(++pc_);

	if constexpr (dbg == true)
	{
		printf("0x%04X MVI %c, 0x%02X\n", pc_ - 1, registerName_[(opcode_ & 0x38) >> 3], Value(reg));
	}

	++pc_;
	return 7;
}

uint8_t Intel8080::Mvi()
{
	auto data = memoryController_->Read(++pc_);
	auto addr = Uint16(h_, l_);

	if constexpr (dbg == true)
	{
		printf("0x%04X MVI [0x%04X], 0x%02X\n", pc_ - 1, addr, data);
	}

	memoryController_->Write(addr, data);
	++pc_;
	return 10;
}

/**
	DAA: Decimal Adjust Accumulator

	The eight-bit hexadecimal number in the
	accumulator is adjusted to form two four bit
	binary-coded-decimal digits.
*/
uint8_t Intel8080::Daa()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X DAA\n", pc_);
	}

	uint8_t adjustment = 0;
	uint8_t highNibble = Value(a_) >> 4;
	uint8_t lowNibble = Value(a_) & 0x0F;

	if (lowNibble > 0x09 || status_[Condition::AuxCarryFlag] == true)
	{
		adjustment += 6;
	}

	if (highNibble > 0x09 || status_[Condition::CarryFlag] == true || (highNibble >= 9 && lowNibble > 9))
	{
		adjustment += 0x60;
		status_[Condition::CarryFlag] = true;
	}

	a_ = Add(a_, Register(adjustment), 0, false, "");
	return 4;
}

/**
	RLC

	The Carry bit is set equal to the high order bit of the accumulator.
	The contents of the accumulator are rotated one bit position to the left
	with the high order bit being transferred to the low-order bit position of
	the accumulator.
*/
uint8_t Intel8080::Rlc()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X RLC\n", pc_);
	}

	status_[CarryFlag] = a_[7];
	a_ <<= 1;
	a_[0] = status_[CarryFlag];
	++pc_;
	return 4;
}

/**
	RRC

	The carry bit is set equal to the low-order
	bit of the accumulator. The contents of the accumulator are
	rotated one bit position to the right, with the low-order bit
	being transferred to the high-order bit position of the accumulator.
*/
uint8_t Intel8080::Rrc()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X RRC\n", pc_);
	}

	status_[CarryFlag] = a_[0];
	a_ >>= 1;
	a_[7] = status_[CarryFlag];
	++pc_;
	return 4;
}

/**
	RAL: Rotate Accumulator Left Through Carry

	The contents of the accumulator are rotated one bit position to the left.
	The high-order bit of the accumulator replaces the
	Carry bit, while the Carry bit replaces the high-order bit of
	the accumulator.
*/
uint8_t Intel8080::Ral()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X RAL\n", pc_);
	}

	bool tmp = status_[CarryFlag];
	status_[CarryFlag] = a_[7];
	a_ <<= 1;
	a_[0] = tmp;
	++pc_;
	return 4;
}

/**
	RAR: Rotate Accumulator Right Through Carry

	The contents of the accumulator are rotated one bit position to the right.
	The low-order bit of the accumulator replaces the
	carry bit, while the carry bit replaces the high-order bit of
	the accumulator.
*/
uint8_t Intel8080::Rar()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X RAR\n", pc_);
	}

	bool tmp = status_[CarryFlag];
	status_[CarryFlag] = a_[0];
	a_ >>= 1;
	a_[7] = tmp;
	++pc_;
	return 4;
}

uint8_t Intel8080::Lxi(Register& regHi, Register& regLow)
{
	regLow = memoryController_->Read(++pc_);
	regHi = memoryController_->Read(++pc_);

	if constexpr (dbg == true)
	{
		printf("0x%04X LXI %c, 0x%04X\n", pc_ - 2, registerName_[(opcode_ & 0x30) >> 3], Uint16(Value(regHi), Value(regLow)));
	}

	++pc_;
	return 10;
}

uint8_t Intel8080::Lxi()
{
	auto spLow = memoryController_->Read(++pc_);
	sp_ = Uint16(memoryController_->Read(++pc_), spLow);

	if constexpr (dbg == true)
	{
		printf("0x%04X LXI SP, 0x%04X\n", pc_ - 2, sp_);
	}

	++pc_;
	return 10;
}

/**
	SHLD: Store H and L Direct

	The contents of the L register are stored
	at the memory address formed by concatenating HI ADD
	with LOW ADO. The contents of the H register are stored at
	the next higher memory address.
*/
uint8_t Intel8080::Shld()
{
	auto addrLow = memoryController_->Read(++pc_);
	uint16_t addr = Uint16(memoryController_->Read(++pc_), addrLow);


	if constexpr (dbg == true)
	{
		printf("0x%04X SHLD, [0x%04X]\n", pc_ - 2, addr);
	}

	memoryController_->Write(addr, Value(l_));
	memoryController_->Write(addr + 1, Value(h_));
	++pc_;
	return 16;
}

/**
	STAX

	Description: The contents of the accumulator are
	stored in the memory location addressed by registers B and
	C, or by registers D and E.
	Condition bits affected: None
	Example:
	If register B contains 3FH and register C contains
	16H, the instruction: STAX B
	will store the contents of the accumulator at memory location 3F16H.
*/
uint8_t Intel8080::Stax(const Register& hi, const Register& low)
{
	if constexpr (dbg == true)
	{
		printf("0x%04X STAX %c\n", pc_, registerName_[(opcode_ & 0x10) >> 3]);
	}

	memoryController_->Write(Uint16(hi, low), Value(a_));
	++pc_;
	return 7;
}

/**
	INX

	The 16-bit number held in the specified
	register pair is incremented by one.
*/
uint8_t Intel8080::Inx(Register& hi, Register& low)
{
	if constexpr (dbg == true)
	{
		printf("0x%04X INX %c\n", pc_, registerName_[(opcode_ & 0x30) >> 3]);
	}

	uint16_t val = Uint16(hi, low) + 1;
	hi = (val >> 8) & 0xFF;
	low = val & 0xFF;
	++pc_;
	return 5;
}

uint8_t Intel8080::Inx()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X INX SP\n", pc_);
	}

	++sp_;
	++pc_;
	return 5;
}

/**
	DAD

	The 16-bit number in the BC/DE pair is added to the 16-bit number held in the H and L
	registers using two's complement arithmetic. The result replaces the contents of
	the H and L registers.
*/
uint8_t Intel8080::Dad(const Register& hi, const Register& low)
{
	if constexpr (dbg == true)
	{
		if ((opcode_ & 0x30) == 0x30)
		{
			printf("0x%04X DAD SP\n", sp_);
		}
		else
		{
			printf("0x%04X DAD %c\n", pc_, registerName_[(opcode_ & 0x30) >> 3]);
		}
	}

	uint32_t val = Uint16(hi, low) + Uint16(h_, l_);
	h_ = (val >> 8) & 0xFF;
	l_ = val & 0xFF;
	status_[Condition::CarryFlag] = (val > 0xFFFF);
	++pc_;
	return 10;
}

uint8_t Intel8080::Dad()
{
	return Dad((sp_ >> 8) & 0xFF, sp_ & 0xFF);
}

/**
	LHLD: load H and L direct

	The byte at the memory address formed
	by concatenating HI ADD with LOW ADD replaces the contents of the L register.
	The byte at the next higher memory address replaces the contents of the H register.
*/
uint8_t Intel8080::Lhld()
{
	auto addrLow = memoryController_->Read(++pc_);
	uint16_t addr = Uint16(memoryController_->Read(++pc_), addrLow);

	if constexpr (dbg == true)
	{
		printf("0x%04X LHLD, [0x%04X]\n", pc_ - 2, addr);
	}

	l_ = memoryController_->Read(addr);
	h_ = memoryController_->Read(addr + 1);
	++pc_;
	return 16;
}

/**
	LDAX

	The contents of the memory location
	addressed by registers BC/DE replace the contents of the accumulator.
*/
uint8_t Intel8080::Ldax(const Register& hi, const Register& low)
{
	if constexpr (dbg == true)
	{
		printf("0x%04X LDAX, %c\n", pc_, registerName_[(opcode_ & 0x10) >> 3]);
	}

	a_ = memoryController_->Read(Uint16(hi, low));
	++pc_;
	return 7;
}

/**
	DCX

	The 16-bit number held in the specified
	register pair is decremented by one.
*/
uint8_t Intel8080::Dcx(Register& hi, Register& low)
{
	if constexpr (dbg == true)
	{
		printf("0x%04X DCX %c\n", pc_, registerName_[(opcode_ & 0x30) >> 3]);
	}

	uint16_t val = Uint16(hi, low) + 0xFFFF;
	hi = (val >> 8) & 0xFF;
	low = val & 0xFF;
	++pc_;
	return 5;
}

uint8_t Intel8080::Dcx()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X DCX SP\n", pc_);
	}

	sp_ += 0xFFFF;
	++pc_;
	return 5;
}

/**
	CMA

	Each bit of the contents of the accumulator is complemented (producing the one's complement).
*/
uint8_t Intel8080::Cma()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X CMA\n", pc_);
	}

	a_.flip();
	++pc_;
	return 4;
}

uint8_t Intel8080::Sta()
{
	auto addrLow = memoryController_->Read(++pc_);
	uint16_t addr = Uint16(memoryController_->Read(++pc_), addrLow);

	if constexpr (dbg == true)
	{
		printf("0x%04X STA, [0x%04X]\n", pc_ - 2, addr);
	}

	memoryController_->Write(addr, Value(a_));
	++pc_;
	return 13;
}

uint8_t Intel8080::Stc()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X STC\n", pc_);
	}

	status_[Condition::CarryFlag] = true;
	++pc_;
	return 4;
}

uint8_t Intel8080::Lda()
{
	auto addrLow = memoryController_->Read(++pc_);
	uint16_t addr = Uint16(memoryController_->Read(++pc_), addrLow);

	if constexpr (dbg == true)
	{
		printf("0x%04X LDA, [0x%04X]\n", pc_ - 2, addr);
	}

	a_ = memoryController_->Read(addr);
	++pc_;
	return 13;
}

uint8_t Intel8080::Cmc()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X CMC\n", pc_);
	}

	status_[Condition::CarryFlag] = !status_[Condition::CarryFlag];
	pc_++;
	return 4;
}

uint8_t Intel8080::Mov(Register& lhs, const Register& rhs)
{
	if constexpr (dbg == true)
	{
		printf("0x%04X MOV %c, %c\n", pc_, registerName_[(opcode_ & 0x38) >> 3], registerName_[opcode_ & 0x07]);
	}

	lhs = rhs;
	pc_++;
	return 5;
}

uint8_t Intel8080::Mov(Register& lhs)
{
	auto addr = Uint16(h_, l_);

	if constexpr (dbg == true)
	{
		printf("0x%04X MOV %c, [0x%04X]\n", pc_, registerName_[(opcode_ & 0x38) >> 3], addr);
	}

	lhs = memoryController_->Read(addr);
	pc_++;
	return 7;
}

uint8_t Intel8080::Mov(uint8_t value)
{
	uint16_t addr = Uint16(h_, l_);

	if constexpr (dbg == true)
	{
		printf("0x%04X MOV [0x%04X], %c\n", pc_, addr, registerName_[opcode_ & 0x07]);
	}

	memoryController_->Write(addr, value);
	pc_++;
	return 7;
}

uint8_t Intel8080::Nop()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X NOP\n", pc_);
	}

	pc_++;
	return 4;
}

/**
	HLT

	The program counter is incremented to
	the address of the next sequential instruction.The CPU then
	enters the STOPPED state and no further activity takes
	place until an interrupt occurs.
*/
/*
	Implementation of the HLT instruction steps the
	Program Counter to the next instruction address and stops
	the computer until an interrupt occurs. The HLT instruction
	should not normally be implemented when a DI instruction
	has been executed. Since the DI instruction causes the computer
	to ignore interrupts, the computer will not operate again
	until the main power switch is turned off and then back on.
*/
uint8_t Intel8080::Hlt()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X HLT\n", pc_);
	}

	// Untested
	assert(0);
	pc_++;
	return 7;
}

Intel8080::Register Intel8080::Add(const Register& lhs, const Register& rhs, uint8_t carry, bool setCarryFlag, [[maybe_unused]] std::string_view instructionName)
{
	uint8_t a = Value(lhs);
	uint8_t b = Value(rhs);

	if (setCarryFlag == true)
	{
		status_[Condition::CarryFlag] = (a + b + carry) > 0xFF;
	}

	status_[Condition::AuxCarryFlag] = ((a & 0x0F) + (b & 0x0F) + carry) > 0x0F;
	auto r = Register(a + b + carry);
	status_[Condition::ZeroFlag] = Zero(r);
	status_[Condition::SignFlag] = Sign(r);
	status_[Condition::ParityFlag] = Parity(r);
	pc_++;
	return r;
}

uint8_t Intel8080::Add(const Register& r, std::string_view instructionName)
{
	a_ = Add(a_, r, 0, true, instructionName);
	return 4;
}

uint8_t Intel8080::Add(uint16_t addr, std::string_view instructionName)
{
	a_ = Add(a_, Register(memoryController_->Read(addr)), 0, true, instructionName);
	return 7;
}

uint8_t Intel8080::Adc(const Register& r, std::string_view instructionName)
{
	a_ = Add(a_, r, status_[Condition::CarryFlag], true, instructionName);
	return 4;
}

uint8_t Intel8080::Adc(uint16_t addr, std::string_view instructionName)
{
	a_ = Add(a_, Register(memoryController_->Read(addr)), status_[Condition::CarryFlag], true, instructionName);
	return 7;
}

Intel8080::Register Intel8080::Sub(const Register& r, uint8_t withCarry, std::string_view instructionName)
{
	auto reg = Add(a_, ~Value(r), !withCarry /* carry flag */, true, instructionName);
	status_.flip(Condition::CarryFlag);
	return reg;
}

uint8_t Intel8080::Sub(const Register& r, std::string_view instructionName)
{
	a_ = Sub(r, 0, instructionName);
	return 4;
}

uint8_t Intel8080::Sub(uint16_t addr, std::string_view instructionName)
{
	a_ = Sub(Register(memoryController_->Read(addr)), 0, instructionName);
	return 7;
}

uint8_t Intel8080::Sbb(const Register& r, std::string_view instructionName)
{
	a_ = Sub(r, status_[Condition::CarryFlag], instructionName);
	return 4;
}

uint8_t Intel8080::Sbb(uint16_t addr, std::string_view instructionName)
{
	a_ = Sub(Register(memoryController_->Read(addr)), status_[Condition::CarryFlag], instructionName);
	return 7;
}

void Intel8080::Ana(const Register& r)
{
	status_[Condition::AuxCarryFlag] = ((a_ | r) & Register(0x08)) != 0;

	a_ &= r;

	status_[Condition::CarryFlag] = false;
	status_[Condition::ZeroFlag] = Zero(a_);
	status_[Condition::SignFlag] = Sign(a_);
	status_[Condition::ParityFlag] = Parity(a_);
	pc_++;
}

uint8_t Intel8080::Ana(const Register& r, std::string_view instructionName)
{
	if constexpr (dbg == true)
	{
		printf("0x%04X %s %c\n", pc_, instructionName.data(), opcode_ & 0x80 ? registerName_[opcode_ & 0x07] : registerName_[(opcode_ & 0x38) >> 3]);
	}

	Ana (r);
	return 4;
}

uint8_t Intel8080::Ana(uint16_t addr, std::string_view instructionName)
{
	Register r = memoryController_->Read(addr);

	if constexpr (dbg == true)
	{
		if (instructionName.data() == "ANI")
		{
			printf("0x%04X %s 0x%02X\n", pc_ - 1, instructionName.data(), Value(r));
		}
		else
		{
			printf("0x%04X %s %c\n", pc_, instructionName.data(), opcode_ & 0x80 ? registerName_[opcode_ & 0x07] : registerName_[(opcode_ & 0x38) >> 3]);
		}
	}

	Ana(r);
	return 7;
}

void Intel8080::Xra(const Register& r)
{
	a_ ^= r;

	status_[Condition::AuxCarryFlag] = false;
	status_[Condition::CarryFlag] = false;
	status_[Condition::ZeroFlag] = Zero(a_);
	status_[Condition::SignFlag] = Sign(a_);
	status_[Condition::ParityFlag] = Parity(a_);
	pc_++;
}

uint8_t Intel8080::Xra(const Register& r, std::string_view instructionName)
{
	if constexpr (dbg == true)
	{
		printf("0x%04X %s %c\n", pc_, instructionName.data(), opcode_ & 0x80 ? registerName_[opcode_ & 0x07] : registerName_[(opcode_ & 0x38) >> 3]);
	}

	Xra(r);
	return 4;
}

uint8_t Intel8080::Xra(uint16_t addr, std::string_view instructionName)
{
	Register r = memoryController_->Read(addr);

	if constexpr (dbg == true)
	{
		if (instructionName.data() == "XRI")
		{
			printf("0x%04X %s 0x%02X\n", pc_ - 1, instructionName.data(), Value(r));
		}
		else
		{
			printf("0x%04X %s %c\n", pc_, instructionName.data(), opcode_ & 0x80 ? registerName_[opcode_ & 0x07] : registerName_[(opcode_ & 0x38) >> 3]);
		}
	}

	Xra(r);
	return 7;
}

void Intel8080::Ora(const Register& r)
{
	a_ |= r;

	status_[Condition::AuxCarryFlag] = false;
	status_[Condition::CarryFlag] = false;
	status_[Condition::ZeroFlag] = Zero(a_);
	status_[Condition::SignFlag] = Sign(a_);
	status_[Condition::ParityFlag] = Parity(a_);
	pc_++;
}

uint8_t Intel8080::Ora(const Register& r, std::string_view instructionName)
{
	if constexpr (dbg == true)
	{
		printf("0x%04X %s %c\n", pc_, instructionName.data(), opcode_ & 0x80 ? registerName_[opcode_ & 0x07] : registerName_[(opcode_ & 0x38) >> 3]);
	}

	Ora(r);
	return 4;
}

uint8_t Intel8080::Ora(uint16_t addr, std::string_view instructionName)
{
	Register r = memoryController_->Read(addr);

	if constexpr (dbg == true)
	{
		if (instructionName.data() == "ORI")
		{
			printf("0x%04X %s 0x%02X\n", pc_ - 1, instructionName.data(), Value(r));
		}
		else
		{
			printf("0x%04X %s %c\n", pc_, instructionName.data(), opcode_ & 0x80 ? registerName_[opcode_ & 0x07] : registerName_[(opcode_ & 0x38) >> 3]);
		}
	}

	Ora(r);
	return 7;
}

uint8_t Intel8080::Cmp(const Register& r, std::string_view instructionName)
{
	Sub(r, 0, instructionName);
	return 4;
}

uint8_t Intel8080::Cmp(uint16_t addr, std::string_view instructionName)
{
	Register r = memoryController_->Read(addr);
	Sub(r, 0, instructionName);
	return 7;
}

uint8_t Intel8080::NotImplemented()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X Instruction %02X not implemented\n", pc_, opcode_);
	}

	if (opcode_ == 0xED || opcode_ == 0xFD || opcode_ == 0xDD)
	{
		pc_++;
	}
	else
	{
		assert(0);
	}

	pc_++;
	return 0;
}

uint8_t Intel8080::RetOnFlag(bool status, std::string_view instructionName)
{
	if constexpr (dbg == true)
	{
		printf("0x%04X %s\n", pc_, instructionName.data());
	}

	pc_++;

	if (status == true)
	{
		auto pcLow = memoryController_->Read(sp_++);
		pc_ = Uint16(memoryController_->Read(sp_++), pcLow);

		if (std::string(instructionName) == "RET")
		{
			return 10;
		}
		else
		{
			return 11;
		}
	}
	else
	{
		return 5;
	}
}

uint8_t Intel8080::Pop(Register& hi, Register& low)
{
	if constexpr (dbg == true)
	{
		if ((opcode_ & 0x30) == 0x30)
		{
			printf("0x%04X POP PSW\n", pc_);
		}
		else
		{
			printf("0x%04X POP %c\n", pc_, registerName_[(opcode_ & 0x30) >> 3]);
		}
	}

	low = memoryController_->Read(sp_++);
	hi = memoryController_->Read(sp_++);
	pc_++;
	return 10;
}

uint8_t Intel8080::JmpOnFlag(bool status, std::string_view instructionName)
{
	auto addrLow = memoryController_->Read(++pc_);
	auto addr = Uint16(memoryController_->Read(++pc_), addrLow);

	if constexpr (dbg == true)
	{
		printf("0x%04X %s 0x%04X\n", pc_ - 2, instructionName.data(), addr);
	}

	status ? pc_ = addr : ++pc_;
	return 10;
}

uint8_t Intel8080::CallOnFlag(bool status, std::string_view instructionName)
{
	auto addrLow = memoryController_->Read(++pc_);
	auto addr = Uint16(memoryController_->Read(++pc_), addrLow);

	if constexpr (dbg == true)
	{
		printf("0x%04X %s 0x%04X\n", pc_ - 2, instructionName.data(), addr);
	}

	++pc_;

	if (status == true)
	{
		sp_ += 0xFFFF;
		memoryController_->Write(sp_, pc_ >> 8);
		sp_ += 0xFFFF;
		memoryController_->Write(sp_, pc_ & 0xFF);

		/*
			This needs to be moved ... by calling push above
			the pc_ will be incremented before the instruction completes.

			This works because we can't interrupt an instruction mid execution.
			If we could this would have to FIXED. It isn't technically correct, but works, a minor issue to fix someday.
		*/
		pc_ = addr;
		return 17;
	}
	else
	{
		return 11;
	}
}

uint8_t Intel8080::Push(const Register& hi, const Register& low)
{
	if constexpr (dbg == true)
	{
		if ((opcode_ & 0x30) == 0x30)
		{
			printf("0x%04X PUSH PSW\n", pc_);
		}
		else
		{
			printf("0x%04X PUSH %c\n", pc_, registerName_[(opcode_ & 0x30) >> 3]);
		}
	}

	sp_ += 0xFFFF;
	memoryController_->Write(sp_, Value(hi));
	sp_ += 0xFFFF;
	memoryController_->Write(sp_, Value(low));
	pc_++;
	return 11;
}

uint8_t Intel8080::Adi(const Register& r)
{
	if constexpr (dbg == true)
	{
		printf("0x%04X ADI %c\n", pc_, registerName_[(opcode_ & 0x30) >> 3]);
	}

	a_ = Value(a_) + memoryController_->Read(++pc_);
	++pc_;
	return 7;
}

/**
	RST

	This section describes the RST (restart) instruction,
	which is a special purpose subroutine jump. This instruction
	occupies one byte.
	The contents of the program counter
	are pushed onto the stack, providing a return address for
	later use by a RETURN instruction.
*/
uint8_t Intel8080::Rst(uint8_t restart)
{
	uint16_t addr = restart & 0x38;

	if constexpr (dbg == true)
	{
		printf("0x%04X INTERRUPT RST %d\n", pc_, addr >> 3);
	}

	sp_ += 0xFFFF;
	memoryController_->Write(sp_, pc_ >> 8);
	sp_ += 0xFFFF;
	memoryController_->Write(sp_, pc_ & 0xFF);

	/*
		This needs to be moved ... by calling push above
		the pc_ will be incremented before the instruction completes.

		This works because we can't interrupt an instruction mid execution.
		If we could this would have to FIXED. It isn't technically correct, but works, a minor issue to fix someday.
	*/
	pc_ = addr;
	return 11;
}

uint8_t Intel8080::Rst()
{
	//We need to increment pc_ before the call to Rst as the address of the next
	//instruction (++pc_) to be executed needs to be pushed to the stack so we
	//can return to it once the Rst completes.
	//++pc_;
	//Rst(opcode_);

	uint16_t addr = opcode_ & 0x38;

	if constexpr (dbg == true)
	{
		printf("0x%04X RST %d\n", pc_, addr >> 3);
	}

	++pc_;

	sp_ += 0xFFFF;
	memoryController_->Write(sp_, pc_ >> 8);
	sp_ += 0xFFFF;
	memoryController_->Write(sp_, pc_ & 0xFF);

	/*
		This needs to be moved ... by calling push above
		the pc_ will be incremented before the instruction completes.

		This works because we can't interrupt an instruction mid execution.
		If we could this would have to be FIXED. It isn't technically correct, but works, a minor issue to fix someday.
	*/
	pc_ = addr;
	return 11;
}

uint8_t Intel8080::Out()
{
	auto out = memoryController_->Read(++pc_);

	if constexpr (dbg == true)
	{
		printf("0x%04X OUT 0x%02X\n", pc_ - 1, out);
	}

	//write to IO port 'out' the accumulator
	ioController_->Write(out, Value(a_));
	++pc_;
	return 10;
}

uint8_t Intel8080::In()
{
	auto in = memoryController_->Read(++pc_);

	if constexpr (dbg == true)
	{
		printf("0x%04X IN 0x%02X\n", pc_ - 1, in);
	}

	//Read into the accumulator the value in IO port 'in'.
	a_ = ioController_->Read(in);
	++pc_;
	return 10;
}

/**
	XTHL

	The contents of the L register are exchanged with the contents of the memory byte whose address is held in the stack pointer SP.
	The contents of the H register are exchanged with the contents of the memory byte whose address is one greater than that held
	in the stack pointer.
*/
uint8_t Intel8080::Xthl()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X XTHL\n", pc_);
	}

	auto spl = memoryController_->Read(sp_);
	auto sph = memoryController_->Read(sp_ + 1);

	uint8_t l = Value(l_);
	uint8_t h = Value(h_);

	std::swap(spl, l);
	std::swap(sph, h);

	l_ = l;
	h_ = h;

	memoryController_->Write(sp_, spl);
	memoryController_->Write(sp_ + 1, sph);
	pc_++;
	return 18;
}

/**
	PCHL

	The contents of the H register replace the most significant 8 bits of the program counter,
	and the contents of the L register replace the least significant 8 bits of the program counter.
	This causes program execution to continue at the address contained in the H and L registers
*/
uint8_t Intel8080::Pchl()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X PCHL\n", pc_);
	}

	pc_ = Uint16(h_, l_);
	return 5;
}

uint8_t Intel8080::Xchg()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X XCHG\n", pc_);
	}

	std::swap(h_, d_);
	std::swap(l_, e_);
	pc_++;
	return 4;
}

/*
Implementation of the DI instruction resets the
interrupt flip - flop. This causes the computer to ignore
any subsequent interrupt signals.
*/
uint8_t Intel8080::Di()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X DI\n", pc_);
	}

	iff_ = false;
	pc_++;
	return 4;
}

uint8_t Intel8080::Sphl()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X SPHL\n", pc_);
	}

	sp_ = Uint16(h_, l_);
	pc_++;
	return 5;
}

/*
	Implementation of the EI instruction sets the
	interrupt flip-flop. This alerts the computer to the presence of interrupts and causes it to respond accordingly
*/
uint8_t Intel8080::Ei()
{
	if constexpr (dbg == true)
	{
		printf("0x%04X EI\n", pc_);
	}

	iff_ = true;
	pc_++;
	return 4;
}

} // namespace meen
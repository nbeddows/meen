#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace MachEmu::Utils
{
	std::string BinToTxt(const std::string& encoder, const std::string& compressor, const uint8_t* bin, const uint32_t binLen);

	std::vector<uint8_t> TxtToBin(const std::string& decoder, const std::string& decompressor, uint32_t dstSize, const std::string& txt);
} // namespace MachEmu::Utils

#endif // UTILS_H
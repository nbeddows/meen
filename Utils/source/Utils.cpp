#include "Utils/base64.hpp"
#include "Utils/Utils.h"

namespace MachEmu::Utils
{
	std::string BinToTxt(const std::string& encoder, const std::string& compressor, const uint8_t* bin, uint32_t binLen)
	{
		if (encoder != "base64")
		{
			throw std::invalid_argument("Invalid binary to text encoder parameter");
		}

		if (compressor != "none")
		{
			throw std::invalid_argument("Invalid compressor parameter");
		}

		return base64::encode_into<std::string>(bin, bin + binLen);
	}

	// dst needs to be of a size equal to the uncompressed input - this needs to be determined by external means
	std::vector<uint8_t> TxtToBin(const std::string& decoder, const std::string& decompressor, uint32_t dstSize, const std::string& src)
	{
		if (decoder != "base64")
		{
			throw std::invalid_argument("Invalid binary to text decoder parameter");
		}

		if (decompressor != "none")
		{
			throw std::invalid_argument("Invalid compressor parameter");
		}

		return base64::decode_into<std::vector<uint8_t>>(src.begin(), src.end());
	}
} // namespace MachEmu::Utils

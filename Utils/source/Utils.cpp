#ifdef ENABLE_ZLIB
#include <zlib.h>
#endif

#include "Utils/Utils.h"
#include "Utils/base64.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "Utils/md5.h"
#ifdef __cplusplus
}
#endif

namespace MachEmu::Utils
{
	std::string BinToTxt(const std::string& encoder, const std::string& compressor, const uint8_t* bin, uint32_t binLen)
	{
		if (encoder != "base64")
		{
			throw std::invalid_argument("Invalid binary to text encoder parameter");
		}

		std::string binToTxt;

		if (compressor != "none")
		{
#ifdef ENABLE_ZLIB
			if (compressor == "zlib")
			{
				std::vector<uint8_t> dst(compressBound(binLen));
				uLongf len = dst.size();

				// do the compression - zlib
				auto err = compress(dst.data(), &len, bin, binLen);

				if (err != Z_OK)
				{
					throw std::runtime_error("Failed to compress binary data");
				}

				binToTxt = base64::encode_into<std::string>(dst.begin(), dst.begin() + len);
			}
			else
#endif
			{
				throw std::invalid_argument("Invalid compressor parameter");
			}
		}
		else
		{
			binToTxt = base64::encode_into<std::string>(bin, bin + binLen);
		}

		return binToTxt;
	}

	// dst needs to be of a size equal to the uncompressed input - this needs to be determined by external means
	std::vector<uint8_t> TxtToBin(const std::string& decoder, const std::string& decompressor, uint32_t dstSize, const std::string& src)
	{
		if (decoder != "base64")
		{
			throw std::invalid_argument("Invalid binary to text decoder parameter");
		}

		auto bin = base64::decode_into<std::vector<uint8_t>>(src.begin(), src.end());

		if (decompressor != "none")
		{
			std::vector<uint8_t> dst;

#ifdef ENABLE_ZLIB			
			if (decompressor == "zlib")
			{
				dst.resize(dstSize);
				uLongf size = dstSize;
				auto err = uncompress(dst.data(), &size, bin.data(), bin.size());

				if (err != Z_OK)
				{
					throw std::runtime_error("Failed to decompress binary data");
				}

				dst.resize(size);
			}
			else
#endif
			{
				throw std::invalid_argument("Invalid compressor parameter");
			}

			return dst;
		}
		else
		{
			return bin;
		}
	}

	std::array<uint8_t, 16> Md5(uint8_t* input, uint32_t len)
	{
		MD5Context ctx;
		md5Init(&ctx);
		md5Update(&ctx, input, len);
		md5Finalize(&ctx);
		auto d = ctx.digest;
	
		return std::array{ d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9], d[10], d[11], d[12], d[13], d[14], d[15] };
	}
} // namespace MachEmu::Utils
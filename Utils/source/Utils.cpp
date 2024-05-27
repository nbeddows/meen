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

#include <bit>
#include <libbase64.h>
#include <stdexcept>
#ifdef ENABLE_ZLIB
#include <zlib.h>
#endif

#include "Utils/Utils.h"

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

		auto encode = [](const char* src, size_t srcLen)
		{
			// dst string needs to be at least 4/3 times the size of the input 
			std::string binToTxt(srcLen * 1.5, '\0');
			base64_encode(src, srcLen, binToTxt.data(), &srcLen, 0);
			binToTxt.resize(srcLen);
			return binToTxt;
		};

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

				return encode(std::bit_cast<const char*>(dst.data()), len);
			}
			else
#endif
			{
				throw std::invalid_argument("Invalid compressor parameter");
			}
		}
		else
		{
			return encode(std::bit_cast<const char*>(bin), binLen);
		}
	}

	// dst needs to be of a size equal to the uncompressed input - this needs to be determined by external means
	std::vector<uint8_t> TxtToBin(const std::string& decoder, const std::string& decompressor, uint32_t dstSize, const std::string& src)
	{
		if (decoder != "base64")
		{
			throw std::invalid_argument("Invalid binary to text decoder parameter");
		}

		std::vector<uint8_t> bin(src.length(), '\0');
		auto binLen = bin.size();
		base64_decode(src.data(), src.length(), std::bit_cast<char*>(bin.data()), &binLen, 0);
		bin.resize(binLen);

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
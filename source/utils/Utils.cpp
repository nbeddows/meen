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

#include <bit>
#ifdef ENABLE_BASE64
#include <libbase64.h>
#endif // ENABLE_BASE64
#ifdef ENABLE_HASH_LIBRARY
#include <md5.h>
#endif // ENABLE_HASH_LIBRARY
#ifdef ENABLE_ZLIB
#include <zlib.h>
#endif // ENABLE_ZLIB

#include "meen/utils/ErrorCode.h"
#include "meen/utils/Utils.h"

namespace meen::Utils
{
	std::expected<std::string, std::error_code> BinToTxt(const std::string& encoder, const std::string& compressor, const uint8_t* bin, uint32_t binLen)
	{
		auto encode = [&encoder](const char* src, size_t srcLen) -> std::expected<std::string, std::error_code>
		{
			std::string binToTxt;
#ifdef ENABLE_BASE64
			if (encoder == "base64")
			{
				// dst string needs to be at least 4/3 times the size of the input
				binToTxt.resize(srcLen * 1.5, '\0');
				base64_encode(src, srcLen, binToTxt.data(), &srcLen, 0);
				binToTxt.resize(srcLen);	
			}
			else
#endif // ENABLE_BASE64
			{
				return std::unexpected<std::error_code>(make_error_code(errc::encoder));
			}

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

				if (err == Z_OK)
				{
					return encode(std::bit_cast<const char*>(dst.data()), len);
				}
				else
				{
					return std::unexpected<std::error_code>(make_error_code(errc::invalid_argument));
				}
			}
			else
#endif // ENABLE_ZLIB
			{
				return std::unexpected<std::error_code>(make_error_code(errc::compressor));
			}
		}
		else
		{
			return encode(std::bit_cast<const char*>(bin), binLen);
		}
	}

	// dst needs to be of a size equal to the uncompressed input - this needs to be determined by external means
	std::expected<std::vector<uint8_t>, std::error_code> TxtToBin(const std::string& decoder, const std::string& decompressor, uint32_t dstSize, const std::string& src)
	{
		std::vector<uint8_t> bin;
#ifdef ENABLE_BASE64
		if (decoder == "base64")
		{
			bin.resize(src.length());
			auto binLen = bin.size();
			base64_decode(src.data(), src.length(), std::bit_cast<char*>(bin.data()), &binLen, 0);
			bin.resize(binLen);
		}
		else
#endif // ENABLE_BASE64
		{
			return std::unexpected<std::error_code>(make_error_code(errc::encoder));
		}

		if (decompressor != "none")
		{
			std::vector<uint8_t> dst;
#ifdef ENABLE_ZLIB
			if (decompressor == "zlib")
			{
				dst.resize(dstSize);
				uLongf size = dstSize;
				auto err = uncompress(dst.data(), &size, bin.data(), bin.size());

				if (err == Z_OK)
				{
					dst.resize(size);
				}
				else
				{
					return std::unexpected<std::error_code>(make_error_code(errc::invalid_argument));
				}
			}
			else
#endif // ENABLE_ZLIB
			{
				return std::unexpected<std::error_code>(make_error_code(errc::compressor));
			}

			return dst;
		}
		else
		{
			return bin;
		}
	}

	std::array<uint8_t, 16> Md5(const uint8_t* input, uint32_t len)
	{
#ifdef ENABLE_HASH_LIBRARY
		std::array<uint8_t, MD5::HashBytes> hash;
		MD5 md5;
		md5.add(input, len);
		md5.getHash(hash.data());
		return hash;
#else
		return std::array<uint8_t, 16>{};
#endif // ENABLE_HASH_LIBRARY
	}
} // namespace meen::Utils
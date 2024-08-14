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

#ifndef UTILS_H
#define UTILS_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

/**	Utility functions

	Free standing methods that provide utility support.
*/
namespace MachEmu::Utils
{
#ifdef ENABLE_BASE64	
	/** Binary to text encoding with optional compression

		@param	encoder					The name of the encoder to use as a string, currently, the only supported encoder is "base64".
		@param	compressor				The name of the compression library to use, currently, the only supported compressor is "zlib".
										Passing "none" as the compressor will encode the binary data WITHOUT compression.
		@param	bin						The binary data to (compress and) encode.
		@param	binLen					The length in bytes of the binary data.

		@return							The encoded text representation of the given binary data as a std::string using the specified encoder.
	
		@throws	std::invalid_argument	Unsupported encoder or compressor parameters.

		@throws	std::runtime_error		The binary data failed to compress.
	*/
	std::string BinToTxt(const std::string& encoder, const std::string& compressor, const uint8_t* bin, const uint32_t binLen);

	/** Text to binary decoding with optional decompression

		@param	decoder					The name of the decoder to use as a string, currently, the only supported decoder is "base64".
		@param	decompressor			The name of the decompression library to use, currently, the only supported compressor is "zlib".
										Passing "none" as the decompressor will decode the binary data WITHOUT decompression. This will cause
										invalid output if the binary data was previously compressed.
		@param	dstSize					The length in bytes of the uncompressed binary data (the size of the data before compression). The value
										is ignored if the compressor is "none".
		@param	txt						The encoded binary data as a std::string which must be in the format specified by the decoder parameter.

		@return							The (decompressed and) decoded binary data obtained from the specified text.

		@throws	std::invalid_argument	Unsupported decoder or decompressor parameters.

		@throws	std::runtime_error		The binary data failed to decompress.
	*/
	std::vector<uint8_t> TxtToBin(const std::string& decoder, const std::string& decompressor, uint32_t dstSize, const std::string& txt);
#endif // ENABLE_BASE64

#ifdef ENABLE_HASH_LIBRARY
	/** MD5 hash
	
		Currently used for ROM hashing, but may have other uses moving forward.

		@param	input					The binary input to the md5 hash function.

		@param	len						The length of the binary data in bytes.

		@return							The 128 bit hash as a std::array of 16 bytes.
	*/
	std::array<uint8_t, 16> Md5(uint8_t* input, uint32_t len);
#endif // ENABLE_HASH_LIBRARY
} // namespace MachEmu::Utils

#endif // UTILS_H
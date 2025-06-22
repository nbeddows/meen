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

#ifndef MEEN_ERROR_H
#define MEEN_ERROR_H

#include <system_error>

namespace meen
{
	/** MEEN Error codes

		Used to define error values that are compatible with std::error_code. These values
		can be used to compare against any error_code value that a MEEN API method returns.
	*/
	enum errc
	{
		no_error,				/**< No error has occurred */
		async,					/**< An asynchronous operation failed to complete */
		busy,					/**< MEEN is currently running */
		clock_sampling_freq,	/**< The sampling frequency can't be set, either it is too high/low or the host clock can't be queried */
		cpu,					/**< The cpu is invalid */
		encoder, 				/**< The binary to text encoding scheme is unknown */
		incompatible_ram,		/**< The ram to load is not compatible with this component */
		incompatible_rom,		/**< The rom to load is not compatible with this component */
		incompatible_uuid,		/**< The uuid to load does not match this component */
		invalid_argument,		/**< An argument passed to this method is invalid */
		io_controller,			/**< The supplied io controller is invalid */
		json_config,			/**< A JSON configuration parameter is invalid */
		json_parse,				/**< The JSON configuration file/string is malformed */
		memory_controller,		/**< The supplied memory controller is invalid */
		compressor,				/**< The compressor scheme is unknown */
		not_implemented,		/**< The method is not implemented */
		unknown_option,			/**< An unknown JSON option was encountered and ignored */
		uri_scheme				/**< A supplied uri scheme is unknown */
	};
} // namespace meen

#endif // MEEN_ERROR_H
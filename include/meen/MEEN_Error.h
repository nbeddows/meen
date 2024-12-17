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

#ifndef MEEN_MEEN_ERROR_H
#define MEEN_MEEN_ERROR_H

#include <system_error>

namespace meen
{
	/** Meen Error codes

		Used to define error values that are compatible with std::error_code.
	*/
	enum errc
	{
		no_error,           /**< No error has occurred. */
		async,				/**< An asynchronous operation failed to complete. */
		busy,               /**< MEEN is currently running. */
		clock_resolution,   /**< The resolution can't be set, either it is too high/low or the host clock can't be queried. */
		cpu,				/**< The cpu is invalid. */
		incompatible_ram,   /**< The ram to load is not compatible with this component. */
		incompatible_rom,   /**< The rom to load is not compatible with this component. */
		incompatible_uuid,  /**< The uuid to load does not match this component. */
		invalid_argument,   /**< An argument passed to this method is invalid. */
		io_controller,		/**< The supplied io controller is invalid. */
		json_config,        /**< A JSON configuration parameter is invalid. */
		json_parse,	        /**< The JSON configuration file/string is malformed. */
		memory_controller,	/**< The supplied memory controller is invalid. */
		no_zlib,            /**< MEEN compiled without ZLIB support */
		not_implemented,    /**< The method is not implemented. */
		unknown_option      /**< An unknown JSON option was encountered and ignored. */
	};

	/** The custom meen error category

		Defines the name of the error category and the messages that each meen::errc returns.
	*/
	const std::error_category& category();

	/** std::error_code wrapper

		A simple convenience wrapper

		@param	ec	The meen::errc to use to create a meen std::error_code
	*/
	inline std::error_code make_error_code(errc ec) { return {ec, meen::category()}; }
} // namespace meen

namespace std
{
	/** Boilerplate

		Inform std that our meen::errc enum is a std::error_code enum.
	*/
	template<>
	struct is_error_code_enum<meen::errc> : public std::true_type {};
} // namespace std

#endif // MEEN_MEEN_ERROR_H
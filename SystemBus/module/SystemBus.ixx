/*
Copyright (c) 2021-2023 Nicolas Beddows <nicolas.beddows@gmail.com>

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

export module SystemBus;

import <bitset>;
import <memory>;

namespace Emulator
{
	export enum Signal
	{
		Clock,
		Interrupt,
		PowerOn,
		PowerOff,
		MemoryRead,
		MemoryWrite,
		IoRead,
		IoWrite,
	};

	export template <class T>
	class DataBus
	{
		private:
			T data_{};
		public:
			constexpr void Send(T data)
			{
				data_ = data;
			}

			constexpr T Receive()
			{
				T data = data_;
				data_ = 0;
				return data;
			}

			constexpr uint8_t Size() const
			{
				return sizeof(T) * 8;
			}
	};
	
	export template <class T>
	class AddressBus
	{
	private:
		T data_{};
	public:
		constexpr void Send(T data)
		{
			data_ = data;
		}

		constexpr T Receive()
		{
			T data = data_;
			data_ = 0;
			return data;
		}

		constexpr uint8_t Size() const
		{
			return sizeof(T) * 8;
		}
	};
	
	export template <uint8_t size>
	class ControlBus
	{
		private:
			std::bitset<size> lanes_;
		public:
			constexpr void Send(Signal signal)
			{
				lanes_.set (signal);
			}

			constexpr bool Receive(Signal signal)
			{
				bool lane = lanes_[signal];
				lanes_.reset (signal);
				return lane;
			}

			constexpr uint8_t Size() const
			{
				return size;
			}
	};

	export template <class addrW, class dataW, uint8_t ctrlW>
	struct SystemBus
	{
		std::shared_ptr<AddressBus<addrW>> addressBus;
		std::shared_ptr<DataBus<dataW>> dataBus;
		std::shared_ptr<ControlBus<ctrlW>> controlBus;

		SystemBus()
		{
			addressBus = std::make_shared<AddressBus<addrW>>();
			dataBus = std::make_shared<DataBus<dataW>>();
			controlBus = std::make_shared<ControlBus<ctrlW>>();
		}

		SystemBus(const std::shared_ptr<AddressBus<addrW>>& ab, const std::shared_ptr<DataBus<dataW>>& db, const std::shared_ptr<ControlBus<ctrlW>>& cb)
		{
			addressBus = ab;
			dataBus = db;
			controlBus = cb;
		}
	};

	export template <class addrW, class dataW, uint8_t ctrlW>
	std::unique_ptr<SystemBus<addrW, dataW, ctrlW>> MakeSystemBus()
	{
		return std::make_unique<SystemBus<addrW, dataW, ctrlW>>();
	}
}

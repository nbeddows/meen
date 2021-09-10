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
		//TransferAck
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

	//export template <class T>
	//using AddressBus<T> = DataBus<T>;
	
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
	};

	export template <class addrW, class dataW, uint8_t ctrlW>
	std::unique_ptr<SystemBus<addrW, dataW, ctrlW>> MakeSystemBus()
	{
		return std::make_unique<SystemBus<addrW, dataW, ctrlW>>();
	}
}

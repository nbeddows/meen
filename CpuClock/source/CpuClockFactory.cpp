module ICpuClock;

import CpuClock;

using namespace std::chrono;

namespace Emulator
{
	//factory free form function
	std::unique_ptr<ICpuClock> MakeCpuClock(nanoseconds timePeriod, nanoseconds correlateFreq)
	{
		return std::make_unique<CpuClock>(timePeriod, correlateFreq);
	}

	std::unique_ptr<ICpuClock> MakeCpuClock(std::shared_ptr<ControlBus<8>> controlBus, nanoseconds timePeriod)
	{
		return std::make_unique<CpuClock>(controlBus, timePeriod);
	}
}
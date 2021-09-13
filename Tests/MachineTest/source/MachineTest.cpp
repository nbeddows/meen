#include <future>
#include <gtest/gtest.h>

import <chrono>;
import ControllerFactory;
import IController;
import IMachine;
import MachineFactory;

using namespace std::chrono;

namespace Emulator::Machine::Tests
{
	class MachineTest : public testing::Test
	{
	protected:
		static std::shared_ptr<IMemoryController> memoryController_;
		static std::shared_ptr<IController> ioController_;
		static std::unique_ptr<IMachine> machine_;
		static const inline std::filesystem::path directory_ = "../../programs";
	public:
		static void SetUpTestCase();
		void SetUp();
	};

	std::shared_ptr<IMemoryController> MachineTest::memoryController_;
	std::shared_ptr<IController> MachineTest::ioController_;
	std::unique_ptr<IMachine> MachineTest::machine_;

	void MachineTest::SetUpTestCase()
	{
		machine_ = MakeMachine();
		memoryController_ = MakeDefaultMemoryController(16); //16 bit memory bus size
		ioController_ = MakeTestIoController(machine_->ControlBus()); //send in control bus here .... machine_->ControlBus(); //return shared_ptr to control bus
	}

	void MachineTest::SetUp()
	{
		machine_->SetMemoryController(nullptr);
		machine_->SetIoController(nullptr);
	}

	/*TEST_F(MachineTest, RunNoIoControllerSet)
	{
		//EXPECT_NO_THROW
		//(
			auto path = directory_;
			memoryController_->Load (path /= "cmc.bin", 0x00);
			machine_->SetMemoryController (memoryController_);

			//run the cmc program for one second, we should end in a timeout
			auto future = std::async(std::launch::async, [&]
			{
				machine_->Run();
			});

			EXPECT_EQ(std::future_status::timeout, future.wait_for(seconds(1)));
		//);
	}*/

	TEST_F(MachineTest, RunNoMemoryControllerSet)
	{
		EXPECT_ANY_THROW
		(
			machine_->Run();
		);
	}

	TEST_F(MachineTest, Run)
	{
		EXPECT_NO_THROW
		(
			auto path = directory_;
			memoryController_->Load(path /= "cmc.bin", 0x00);
			machine_->SetMemoryController(memoryController_);
			machine_->SetIoController(ioController_);
			machine_->Run();
		);
	}
}

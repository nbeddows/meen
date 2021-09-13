export module IMachine;

import <filesystem>;
import Controller;

namespace Emulator
{
	/** IMachine

	*/
	export struct IMachine
	{
		/** Load
		
			Load a rom file into memory at the specified address.

			@param	rom				The read only memory to be loaded.
			@param	memoryOffset	The offset in bytes into memory.

			@throw	std::exception	@see IMemoryController.
			
			@discussion				This function call be called multiple times in order
									to load different rom files at different memory locations.
		*/
		virtual void Load(std::filesystem::path rom, uint32_t memoryOffset) = 0;
		
		/** Run
		
			Run the roms loaded into memory initialising execution at the given
			program counter.

			@param	pc		The program counter.

			@discussion		The program counter is the memory address at which the cpu
							will start executing the instructions contained in the
							rom files that were loaded into memory.
		*/
		virtual void Run(uint16_t pc) = 0;

		/** SetMemoryController
		
		*/
		virtual void SetMemoryController (std::shared_ptr<IController> controller) = 0;

		/** SetIoController

		*/
		virtual void SetIoController (std::shared_ptr<IController> controller) = 0;

		virtual ~IMachine() = default;
	};
}
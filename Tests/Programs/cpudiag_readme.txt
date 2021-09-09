This is a modifed version of the basic cpu diagnostic tool (cpudiag.asm) that can be found at:

http://brainwagon.org/2011/09/08/more-on-my-8080-emulator/
http://www.emulator101.com/files/cpudiag.asm

These modifications are done for a couple of reasons:

1. Eliminate dependence on CP/M.
2. Make the diagnostic tool compatible with the tests which use OUT 0xFF as a means of
   exiting the test.
3. Add the ability to test the OUT instruction which is one of the instructions that
   cpu diag does not test.

The first modification replaces the dependance on CP/M BDOS for printing an output message
with a set IO Out instructions which emulates the same behaviour. There is a mock io controller
called mockCpmIoController in tests/mocks/mockController.ixx which handles the printing of the message to an output buffer.

First, Open cpudiag.bin in a hex editor.

Modify the MSG routine:

1. Replace the value (0x05) at the low memory address of the CALL instruction (0x004A) with 0xEE.
2. Leave the value (0x00) at the high memory address (0x004B) unchanged, this will become a NOP instruction. 

Modify the PCHAR routine:

1. Replace the value (0x05) at the low memory address of the CALL instruction (0x0051) with 0xFA.
2. Leave the value (0x00) at the high memory address (0x0052) unchanged, this will become a NOP instruction. 

Patch cpudiag.bin with cpudiag_patch.bin

Copy the contents of cpudiag_patch.bin and paste them after the first 3 bytes of cpudiag.bin, increasing the size of cpudiag.bin by 21 bytes.
The patch contains two subroutines which use the out instruction to print a single character or a sequence of characters in memory terminated by '$'.

The second modification replaces the call to WBOOT with an OUT instruction. This allows
the IO to handle end of program.

Modify the CPUER routine:

1. Replace the JMP instruction (0xC3) at 0x0598 with an OUT (0xD3) instruction.
2. Replace the low memory address (0x599) with 0xFF (This is the port which signals end of test).
3. Leave the high memory address (0x59A) of the JMP instruction unchanged,
   this will become a NOP instruction.

Modify the CPUOK routine:

1. Replace the JMP instruction (0xC3) at 0x05A1 with an OUT (0xD3) instruction.
2. Replace the low memory address (0x5A2) with 0xFF (This is the port which signals end of test).
3. Leave high memory address (0x5A3) of the JMP instruction unchanged,
   this will become a NOP instruction.

The program cpudiag.bin MUST be loaded into memory at an offset of 0xEB. This must be done in order to maintain the correct memory addresses referenced by various instructions in the diagnositc test.
MACHineEMUlator is a project which aims to define a simple framework for creating an emulated machine.

It represents a challenge I laid out for myself many years ago but has only got up and running
recently thanks to all the spare time we have all had due to the COVID-19 pandemic. It also gave
me the opportunity to explore the lastet features of the C++20 draft and make use of them where
applicable.

Project Goals: 

This list will expand as certain milestones are achieved.

1. Implement a complete Intel 8080 cpu emulator with passing individual unit tests for each instruction. It should also pass the standard Intel 8080 cpudiag tests which can be found online. **COMPLETE**

2. Implement a simple clock which will control the speed of the cpu and have the ability to have its resolution customised. **COMPLETE**

3. Implement an abstract controller interface which can be used to read and write data. The interface can then be used to create custom memory and io controllers which can be targeted towards specific programs and architectures. **COMPLETE**

4. Implement a basic system bus which can be used as 'lines' of communication between the cpu and various controllers. **COMPLETE**

5. Upgrade the 8080 to an 8085 (z80) complete with passing additional individual instruction unit tests. It should also pass the standard z80 zexall tests which can be found online. **NOT STARTED**

Conceptually speaking, MachEmu can be represented by the following diagram:

![](Docs/images/MachineDiagram.png)

As can be seen from the diagram above MachEmu is represented by the inner machine containing a cpu and a clock used to regulate its speed. The speed the clock runs at is dictated by the cpu type, however the resolution of the clock can be externally manipulted (See IMachine::SetClockResolution).

The outer machine represents the inner machine customisation. For example, custom input/output may involve interacting with a keyboard or mouse, or some other proprietry peripheral, whereas custom memory maybe as simple as reading and writing to a block of locally allocated memory, a network socket or some other proprietry memory configuration, this all depends on the machine being built. Basic IO and memory controllers can be found in the unit tests.

Special thanks to the following sites:

https://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf<br>
https://altairclone.com/downloads/cpu_tests/<br>
https://caglrc.cc/~svo/i8080/
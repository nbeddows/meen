.section .rodata
.global bdosMsgStart
bdosMsgStart:
.incbin "bdosMsg.bin"
.global bdosMsgEnd
bdosMsgEnd:
.global exitTestStart
exitTestStart:
.incbin "exitTest.bin"
.global exitTestEnd
exitTestEnd:
.global nopStartStart
nopStartStart:
.incbin "nopStart.bin"
.global nopStartEnd
nopStartEnd:
.global nopEndStart
nopEndStart:
.incbin "nopEnd.bin"
.global nopEndEnd
nopEndEnd:
.global pre8080Start
pre8080Start:
.incbin "8080PRE.COM"
.global pre8080End
pre8080End:
.global exm8080Start
exm8080Start:
.incbin "8080EXM.COM"
.global exm8080End
exm8080End:
.global cpuTestStart
cpuTestStart:
.incbin "CPUTEST.COM"
.global cpuTestEnd
cpuTestEnd:
.global tst8080Start
tst8080Start:
.incbin "TST8080.COM"
.global tst8080End
tst8080End:
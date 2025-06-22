.section .rodata
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
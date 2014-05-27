MODALIGN 	EQU		1<<0
MEMINFO		EQU		1<<1
MBFLAGS		EQU		MODALIGN | MEMINFO
MBMAGIC		EQU		0x1BADB002
MBCHECKSUM	EQU		-(MBFLAGS + MBMAGIC)


SECTION		.bootstrap_stack

ALIGN		4
stack_bottom:
	TIMES	16384	DB	0
stack_top:


SECTION 	.multiboot

ALIGN		4
	DD		MBMAGIC
	DD		MBFLAGS
	DD		MBCHECKSUM
	

SECTION		.text

GLOBAL		_start
EXTERN 		main

_start:
	mov		esp,	stack_top
	call 	main

cli
hlt

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


SECTION		.data

;Following is the GDT which is installed once, before the kernel is handed control.
;This remains constant and is loaded by gdt_install_local.
gdt_start:
	DD		0
	DD		0
	
	DW		0xFFFF
	DW		0
	DB		0
	DB		10011010b
	DB		11001111b
	DB		0
	
	DW		0xFFFF
	DW		0
	DB		0
	DB		10010010b
	DB		11001111b
	DB		0
gdt_end:

;gdtr_info contains the data to be loaded in the GDTR once, before the kernel is handed control.
;It points to the GDT whose base is located at 'gdt_start'.
;This remains constant and is used only once for initialization.
gdtr_info:
	DW		gdt_end - gdt_start - 1
	DD		gdt_start


SECTION 	.multiboot

ALIGN		4
	DD		MBMAGIC
	DD		MBFLAGS
	DD		MBCHECKSUM
	

SECTION		.text

GLOBAL		_start
EXTERN 		main

gdt_install_local:
	lgdt	[gdtr_info]
	ret

_start:
	mov	esp,	stack_top
	
	call	gdt_install_local
	call 	main

	cli
	hlt
	
GLOBAL		_i686_gdt_install
EXTERN		gdtr_default

_i686_gdt_install:
	lgdt	gdtr_default
	ret

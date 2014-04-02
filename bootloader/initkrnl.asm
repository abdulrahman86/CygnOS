BITS 16

org 0x500

jmp __INITKRNL_START__

msgSecondStageLoading 			DB "**************************", 0x0D, 0x0A, "Loading kernel loader...", 0x0D, 0x0A, 0x00
msgEnteringProtectedMode 		DB "Entering protected mode...", 0x0D, 0x0A, "**************************", 0x0D, 0x0A, 0x00
nameKernelFile 				DB "KERNEL  BIN"
msgKernelNotFound 			DB "Kernel file not found.", 0x0D, 0x0A, 0x00
numFileSize				DW 0x0
	
;#########################
;##### Real Mode Functions
;#########################

%include "FLOPPY.inc"
%include "FAT12.inc"

__PrintMsg_:
	pusha
	
	_PrintMsgLoop_:
		lodsb
		or 	al, al
		jz 	_PrintMsgDone_
		mov 	ah, 0Eh
		int 	10h
		jmp 	_PrintMsgLoop_
	
	 _PrintMsgDone_:
 		popa
		ret	

__WAIT_KB_IB_:
	in 	al, 0x64
	test 	al, 0x02
	jnz 	__WAIT_KB_IB_
	
	ret

__WAIT_KB_OB_:
	in 	al, 0x64
	test 	al, 0x01
	jz 	__WAIT_KB_OB_
	
	ret

__ENABLE_A20_:
	cli	
	
	call 	__WAIT_KB_IB_
	mov 	al, 0xAD
	out 	0x64, al

	call 	__WAIT_KB_IB_
	mov 	al, 0xD0
	out 	0x64, al
	
	call 	__WAIT_KB_OB_
	in 	al, 0x60
	push 	ax
	
	call 	__WAIT_KB_IB_
	mov 	al, 0xD1
	out 	0x64, al
	
	call 	__WAIT_KB_IB_
	pop 	ax
	or 	al, 0x02
	out 	0x60, al
	
	call 	__WAIT_KB_IB_
	mov 	al, 0xAE
	out 	0x64, al
	
	call 	__WAIT_KB_IB_
	
	sti
	ret	
	
__LOAD_GDT_:
	cli
	lgdt 	[__GDT_POINTER__]
	sti
	
	ret	

;#########################
;# Global Descriptor Table
;#########################
__GDT_START__:

	_NULL_DESCRIPTOR_:
		DD 0x0
		DD 0x0
	
	_CODE_DESCRIPTOR_:
		DW 0xFFFF
		DW 0x0
		DB 0x0
		DB 10011010b
		DB 11001111b
		DB 0x0
	
	_DATA_DESCRIPTOR_:
		DW 0xFFFF
		DW 0x0
		DB 0x0
		DB 10010010b
		DB 11001111b
		DB 0x0
__GDT_END__:

__GDT_POINTER__:
		DW __GDT_END__ - __GDT_START__ - 1
		DD __GDT_START__

;#########################
;######### Main Code Start
;#########################
BITS 16

__INITKRNL_START__:
	cli	
	
	mov 	ax, 0x0000
	mov 	ds, ax
	mov 	es, ax
	mov 	fs, ax
	mov 	gs, ax
	
	mov 	ax, 0x9000
	mov 	ss, ax
	mov 	sp, 0xFFFF
	
	sti
	
	mov 	si, msgSecondStageLoading
	call 	__PrintMsg_
	
	call 	__LOAD_GDT_
	
	call 	__ENABLE_A20_
	
	mov 	si, msgEnteringProtectedMode
	call 	__PrintMsg_
	
	mov 	si, nameKernelFile
	mov 	dx, 0x1000
	mov 	bx, 0x0
	call 	__LoadFile_

	cmp 	ax, -1
	jnz 	__CHECK_ERROR_2__
	mov 	si, msgKernelNotFound
	call 	__PrintMsg_
	jmp 	__END_AND_FAILURE__

__CHECK_ERROR_2__:
	cmp 	ax, -2
	jnz 	__KERNEL_FOUND__
	jmp 	__END_AND_FAILURE__
	
__KERNEL_FOUND__:
	cli

	mov 	eax, cr0
	or 	eax, 0x1
	mov 	cr0, eax
	
	jmp 	0x8:__LOAD_KERNEL__
	
BITS 32
__LOAD_KERNEL__:

	mov 	ax, 0x10
	mov 	ds, ax
	mov 	es, ax
	mov 	ss, ax
	mov 	esp, 0x90000
	
	xor 	eax, eax
	mov 	ebx, eax
	mov 	ecx, eax
	mov 	ax, WORD [numFileSize]
	mov 	bx, WORD [bpbBytesPerSector]
	mul 	bx
	shr 	ax, 0x02
	cld
	mov 	esi, 0x10000
	mov 	edi, 0x100000
	mov 	ecx, eax
	rep 	movsd
	
	jmp 	0x8:0x100000

__END_AND_FAILURE__:
	cli	
	hlt


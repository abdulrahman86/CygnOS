BITS 16

org 0x500

jmp __INITKRNL_START

msgSecondStageLoading 		db "**************************", 0x0D, 0x0A, "Loading kernel loader...", 0x0D, 0x0A, 0x00
msgEnteringProtectedMode 	db "Entering protected mode...", 0x0D, 0x0A, "**************************", 0x0D, 0x0A, 0x00
kernelFileName 				db "KERNEL  BIN"
msgKernelNotFound 			db "Kernel file not found.", 0x0D, 0x0A, 0x00
fileSize					dw 0x0
	
;#########################
;##### Real Mode Functions
;#########################

%include "FLOPPY.inc"
%include "FAT12.inc"

_PrintMsg:
	pusha
 __PrintMsgLoop:
	lodsb
	or al, al
	jz __PrintMsgDone
	mov ah, 0Eh
	int 10h
	jmp __PrintMsgLoop
 __PrintMsgDone:
 	popa
	ret	

_WAIT_KB_IB:
	in al, 0x64
	test al, 0x02
	jnz _WAIT_KB_IB
	ret

_WAIT_KB_OB:
	in al, 0x64
	test al, 0x01
	jz _WAIT_KB_OB
	ret

_ENABLE_A20:
	cli	
	
	call _WAIT_KB_IB
	mov al, 0xAD
	out 0x64, al

	call _WAIT_KB_IB
	mov al, 0xD0
	out 0x64, al
	
	call _WAIT_KB_OB
	in al, 0x60
	push ax
	
	call _WAIT_KB_IB
	mov al, 0xD1
	out 0x64, al
	
	call _WAIT_KB_IB
	pop ax
	or al, 0x02
	out 0x60, al
	
	call _WAIT_KB_IB
	mov al, 0xAE
	out 0x64, al
	
	call _WAIT_KB_IB
	
	sti
	ret	
	
_LOAD_GDT:	
	cli
	lgdt [__GDT_POINTER]
	sti
	ret	

;#########################
;# Global Descriptor Table
;#########################
__GDT_START:
_NULL_DESCRIPTOR:
	dd 0x0
	dd 0x0
	
_CODE_DESCRIPTOR:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 10011010b
	db 11001111b
	db 0x0
	
_DATA_DESCRIPTOR:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0
__GDT_END:

__GDT_POINTER:
	dw __GDT_END - __GDT_START - 1
	dd __GDT_START

;#########################
;######### Main Code Start
;#########################
BITS 16

__INITKRNL_START:
	cli	
	
	mov ax, 0x0000
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ax, 0x9000
	mov ss, ax
	mov sp, 0xFFFF
	
	sti
	
	mov si, msgSecondStageLoading
	call _PrintMsg
	
	call _LOAD_GDT
	
	call _ENABLE_A20
	
	mov si, msgEnteringProtectedMode
	call _PrintMsg
	
	mov si, kernelFileName
	mov dx, 0x1000
	mov bx, 0x0
	call _LoadFile

	cmp ax, -1
	jnz __CHECK_ERROR_2
	mov si, msgKernelNotFound
	call _PrintMsg
	jmp __END_AND_FAILURE

__CHECK_ERROR_2:
	cmp ax, -2
	jnz __KERNEL_FOUND
	jmp __END_AND_FAILURE
	
__KERNEL_FOUND:
	cli

	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	
	jmp 0x8:__LOAD_KERNEL
	
BITS 32
__LOAD_KERNEL:

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov esp, 0x90000
	
	xor eax, eax
	mov ebx, eax
	mov ecx, eax
	mov ax, WORD [fileSize]
	mov bx, WORD [bpbBytesPerSector]
	mul bx
	shr ax, 0x02
	cld
	mov esi, 0x10000
	mov edi, 0x100000
	mov ecx, eax
	rep movsd
	
	jmp 0x8:0x100000

__END_AND_FAILURE:	
	cli	
	hlt


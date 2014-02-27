BITS 16

org 0x500

jmp __INITKRNL_START

msgSecondStageLoading db "**************************", 0x0D, 0x0A, "Loading kernel loader...", 0x0D, 0x0A, 0x00
msgEnteringProtectedMode db "Entering protected mode...", 0x0D, 0x0A, "**************************", 0x0D, 0x0A, 0x00
	
;#########################
;##### Real Mode Functions
;#########################

_PrintMsg:
	lodsb
	or al, al
	jz __PrintMsgDone
	mov ah, 0Eh
	int 10h
	jmp _PrintMsg
 __PrintMsgDone:
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
; Protected Mode Functions
;#########################
BITS 32

ROWS 		db 25
COLUMNS 	db 80
X 			db 0
Y			db 0
VIDSTART:	dd 0xB8000
CHARATTRIB	db 0x0F
BLANKCHAR 	db " "

PrintChar:
	cmp dl, 0x0A
	je __NEW_ROW

	mov esi, [VIDSTART]

	xor ecx, ecx
	mov eax, ecx
	mov al, BYTE [COLUMNS]
	mov cl, 0x02
	mul cl
	mov cl, BYTE [Y]
	mul cl
	push eax

	mov cl, 0x02
	xor eax, eax
	mov al, BYTE [X]
	mul cl
	pop ecx
	add eax, ecx
	add esi, eax
	
	mov dh, BYTE [CHARATTRIB]
	mov WORD [esi], dx
	
	inc BYTE [X]
	mov bl, BYTE [COLUMNS]
	cmp BYTE [X], bl
	je __NEW_ROW
	jmp __PrintCharDONE
;	jnz __PrintCharDONE
 
  __NEW_ROW:
	mov BYTE [X], 0
	inc BYTE [Y]
	
  __PrintCharDONE:
	ret
	
PrintString:

  __STRING_LOOP:	
	mov dl, BYTE [edi]
	cmp dl, 0x0
	je __PrintStringDONE
	call PrintChar
	
	inc edi
	jmp __STRING_LOOP
	
  __PrintStringDONE:
	ret
	
ClearScreen:
	mov edi, [VIDSTART]
	mov cx, 80*25
	
	mov ah, BYTE [CHARATTRIB]
	mov al, BYTE [BLANKCHAR]
	
	rep stosw
	
	mov BYTE [X], 0x0
	mov BYTE [Y], 0x0

	ret

UpdateCursor:
	xor ax, ax
	mov cx, ax
	
	mov al, BYTE [Y]
	mov cl, BYTE [COLUMNS]
	mul cl
	mov cl, BYTE [X]
	add al, cl
	mov bl, al
	
	mov al, 0x0F
	mov dx, 0x03D4
	out dx, al
	
	mov al, bl
	mov dx, 0x03D5
	out dx, al
	
	mov al, 0x0E
	mov dx, 0x03D4
	out dx, al
	
	mov al, bh
	mov dx, 0x03D5
	out dx, al

	ret

	
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
	
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	
	jmp 0x8:__PMODE
	
BITS 32
__PMODE:

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov esp, 0x90000

	call ClearScreen

	call UpdateCursor

	mov edi, msgInProtectedMode
	call PrintString
	
	mov edi, msgSecondStageLoaded
	call PrintString
	
	call UpdateCursor
	
	cli	
	hlt

	msgInProtectedMode db "Entered protected mode.", 0x0A, 0x00
	msgSecondStageLoaded db "Kernel loader loaded.", 0x0A, "**************************", 0x0A, 0x00	


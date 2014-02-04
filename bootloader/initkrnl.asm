bits 16

jmp __INITKRNL_START

msgSecondStageLoading db "Loading kernel loader...", 0x0D, 0x0A, 0x00
msgTemp db "Second stage loaded", 0x0D, 0x0A, 0x00

;#########################
;############### Functions
;#########################
_PrintMsg:
	lodsb
	or al, al
	jz Done
	mov ah, 0Eh
	int 10h
	jmp _PrintMsg
 Done:
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
	
	cli
	lgdt [__GDT_POINTER]
	sti

;#########################
;######### Main Code Start
;#########################

__INITKRNL_START:
	cli	
	
	mov ax, 0x0050
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
	
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax	
	
	cli	
	hlt

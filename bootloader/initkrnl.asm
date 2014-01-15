bits 16
org 0x0

mov ax,0050
mov ds, ax
mov es, ax

jmp _END

secondStageLoaded DB "Second stage loaded", 0x0D, 0x0A, 0x00

_PrintMessage:
		lodsb
		or al, al
		jz Done
		mov ah, 0Eh
		int 10h
		jmp _PrintMessage
	Done:
		ret

_END:
mov si, secondStageLoaded
call _PrintMessage
cli
hlt 

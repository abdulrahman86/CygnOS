BITS 32

ORG 0x100000

jmp __KERNEL_BEGIN

	msgWelcomeMessage db 0x0A, "          ************************************************************          ", 0x0A, "                              Welcome to Cygnus OS                              ", 0x00
	msgSecondStageLoaded db 0x0A, "                              Kernel loader loaded                              ", 0x0A,  "          ************************************************************          ", 0x00	

ROWS 		db 25
COLUMNS 	db 80
X 			db 0
Y			db 0
VIDSTART:	dd 0xB8000
CHARATTRIB	db 0x0F
BLANKCHAR 	db " "

;######################
;############ Functions
;######################

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
	mov BYTE [X], 0x0
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
	mov bx, ax
	
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

;######################
;############ Main Code
;######################

__KERNEL_BEGIN:

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov esp, 0x90000

	call ClearScreen

	mov edi, msgWelcomeMessage
	call PrintString
	
	mov edi, msgSecondStageLoaded
	call PrintString

	call UpdateCursor

	cli
	hlt

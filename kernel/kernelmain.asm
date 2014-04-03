BITS 32

ORG 0x100000

jmp __KERNEL_BEGIN__

msgWelcomeMessage 		DB 0x0A, "          ************************************************************          ", 0x0A, "                              Welcome to Cygnus OS                              ", 0x00
msgSecondStageLoaded 		DB 0x0A, "                              Kernel loader loaded                              ", 0x0A,  "          ************************************************************          ", 0x00	

ROWS 		DB 25
COLUMNS 	DB 80
X 		DB 0
Y		DB 0
VIDSTART:	DD 0xB8000
CHARATTRIB	DB 0x0F
BLANKCHAR 	DB " "

;######################
;############ Functions
;######################

PrintChar:
	cmp 	dl, 0x0A
	je 	_NEW_ROW_

	mov 	esi, [VIDSTART]

	xor 	ecx, ecx
	mov 	eax, ecx
	mov 	al, BYTE [COLUMNS]
	mov 	cl, 0x02
	mul 	cl
	mov 	cl, BYTE [Y]
	mul 	cl
	push 	eax

	mov 	cl, 0x02
	xor 	eax, eax
	mov 	al, BYTE [X]
	mul 	cl
	pop 	ecx
	add 	eax, ecx
	add 	esi, eax
	
	mov 	dh, BYTE [CHARATTRIB]
	mov 	WORD [esi], dx
	
	inc 	BYTE [X]
	mov 	bl, BYTE [COLUMNS]
	cmp 	BYTE [X], bl
	je 	_NEW_ROW_
	jmp 	_PRINT_CHAR_DONE_
;	jnz __PrintCharDONE
 
	_NEW_ROW_:
		mov 	BYTE [X], 0x0
		inc 	BYTE [Y]
	
	_PRINT_CHAR_DONE_:
		ret
	
PrintString:

	_STRING_LOOP_:	
		mov 	dl, BYTE [edi]
		cmp 	dl, 0x0
		je 	_PRINT_STRING_DONE_
		call 	PrintChar
	
		inc 	edi
		jmp 	_STRING_LOOP_
	
	_PRINT_STRING_DONE_:
		ret
	
ClearScreen:
	mov 	edi, [VIDSTART]
	mov 	cx, 80*25
	
	mov 	ah, BYTE [CHARATTRIB]
	mov 	al, BYTE [BLANKCHAR]
	
	rep 	stosw
	
	mov 	BYTE [X], 0x0
	mov 	BYTE [Y], 0x0

	ret

UpdateCursor:
	xor 	ax, ax
	mov 	cx, ax
	
	mov 	al, BYTE [Y]
	mov 	cl, BYTE [COLUMNS]
	mul 	cl
	mov 	cl, BYTE [X]
	add 	al, cl
	mov 	bx, ax
	
	mov 	al, 0x0F
	mov 	dx, 0x03D4
	out 	dx, al
	
	mov 	al, bl
	mov 	dx, 0x03D5
	out 	dx, al
	
	mov 	al, 0x0E
	mov 	dx, 0x03D4
	out 	dx, al
	
	mov 	al, bh
	mov 	dx, 0x03D5
	out 	dx, al

	ret

;######################
;############ Main Code
;######################

__KERNEL_BEGIN__:

	mov 	ax, 0x10
	mov 	ds, ax
	mov 	es, ax
	mov 	ss, ax
	mov 	esp, 0x90000

	call 	ClearScreen

	mov 	edi, msgWelcomeMessage
	call 	PrintString
	
	mov 	edi, msgSecondStageLoaded
	call 	PrintString

	call 	UpdateCursor

	cli
	hlt

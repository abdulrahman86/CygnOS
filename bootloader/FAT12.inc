BITS 16

dataRegion			dw 0x0
bufStart			dw 0x7E00
nextCluster			dw 0x0
kernelFileName		db "KRNL    BIN"
msgKernelNotFound	db "Kernel file not found.", 0x0D, 0x0A

_LoadRoot:
	xor ax, ax
	mov cx, ax
	mov dx, ax
	
	mov ax, 0x0020
	mul WORD [bpbNumberOfRootEnt]
	div WORD [bpbBytesPerSector]
	xchg ax, cx                     	;cx now has number of sectors used by root directory
	
	mov al, BYTE [bpbNumberOfFATs]
	mul WORD [bpbSectorsPerFAT]
	add ax, WORD [bpbReservedSectors]	;ax now has number of sectors before root directory by 0-based addressing
	
	mov WORD [dataRegion], ax
	add WORD [dataRegion], cx
	
	mov bx, WORD [bufStart]
	call _GetSectors
	
	ret

_LoadFAT:
	mov cx, WORD [bpbSectorsPerFAT]
	mov ax, WORD [bpbReservedSectors]
	
	mov bx, WORD [bufStart]
	call _GetSectors
	
	ret
	
_FIND_FILE:
	mov cx, WORD [bpbNumberOfRootEnt]
	mov di, WORD [bufStart]
	
_ROOT_FIND_FILE:
   	mov si, kernelFileName
   	push cx
   	push di
   	mov cx, 0x000B
   	repe cmpsb
   	cmp cx, 0x0000
   	pop di   	
   	jz _FIND_FIRST_CLUSTER_FILE  	
   	add di, 0x0020
   	pop cx   	
   	loop _ROOT_FIND_FILE
   	
   	mov si, msgKernelNotFound
   	call _PrintMsg
	jmp __END_AND_FAILURE
	
   _FIND_FIRST_CLUSTER_FILE:
   	mov cx, WORD [di + 0x001A]
   	mov WORD [nextCluster], cx
   	
   	ret

BITS 16

dataRegion			dw 0x0
bufStart			dw 0x7E00
nextCluster 		dw 0x0
msgGetSectorsError	db "Error in loading sectors off disk", 0x0D, 0x0A

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
	
_FindFile:
	call _LoadRoot	
	
	mov cx, WORD [bpbNumberOfRootEnt]
	mov di, WORD [bufStart]
	
_ROOT_FIND_FILE:
	push si
	push cx
	push di
	mov cx, 0x000B
  	rep cmpsb
	pop di
	cmp cx, 0x0
   	jz _FIND_FIRST_CLUSTER_FILE
   	add di, 0x0020
   	pop cx   
   	pop si	
   	loop _ROOT_FIND_FILE
   	
   	mov ax, -1
   	ret
	
   _FIND_FIRST_CLUSTER_FILE:
   	mov cx, WORD [di + 0x001A]
   	mov WORD [nextCluster], cx
   	
   	pop cx
   	pop si
 
   	ret
   	
_LoadFile:
	push bx
	push dx	
	
	xor ax, ax
	call _FindFile	
	cmp ax, -1
	jnz _FILE_FOUND
	
	ret
	
   _FILE_FOUND:
   	call _LoadFAT
   
	pop dx
	mov es, dx

   _READ_FILE:
	mov ax, WORD [nextCluster]	
	call _ClusterToLBA
	
	xor cx, cx
	mov cl, BYTE [bpbSectorsPerCluster]
	add WORD [fileSize], cx
	
	pop bx
	call _GetSectors
	cmp ax, -2
	jnz _READ_ALL_SECTS
	
	mov si, msgGetSectorsError
	call _PrintMsg
	ret
   
   _READ_ALL_SECTS:
	push bx   
   	mov ax, WORD [nextCluster]
	mov cx, ax
	mov dx, ax
	shr cx, 0x0001						
	add dx, cx							
	mov bx, WORD [bufStart]
	add bx, dx
	mov dx, WORD [bx]
	test ax, 0x0001						
	jz _EVEN_CLUSTER
	
	shr dx, 0x0004
	jmp _CHECK_LAST_CLUSTER
	
   _EVEN_CLUSTER:
    and dx, 0x0FFF
    
   _CHECK_LAST_CLUSTER:
    cmp dx, 0x0FF0
    mov WORD [nextCluster], dx			
    jnz _READ_FILE
	
	pop bx
    ret

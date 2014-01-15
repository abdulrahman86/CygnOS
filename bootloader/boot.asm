bits 16
org 0

jmp __SEGMENTS_INIT

bpbOEMName		DB "Sunny S "
;##########################################
;#################### BIOS Parameter Block 
;##########################################

bpbBytesPerSector	 :	DW 512
bpbSectorsPerCluster :	DB 1
bpbReservedSectors	 :	DW 1
bpbNumberOfFATs		 :	DB 2
bpbNumberOfRootEnt	 :	DW 224
bpbTotalSectors		 :	DW 2880
bpbMedia			 :	DB 0xF0
bpbSectorsPerFAT	 :	DW 9
bpbSectorsPerTrack	 :	DW 18
bpbNumberOfHeads	 :	DW 2
bpbHiddenSectors	 :	DD 0
bpbBigSectors		 :	DD 0
ebpbDriveNumber		 :	DB 0x0
ebpbFlags			 :	DB 0
ebpbSignature		 :	DB 0x29
ebpbVolumeID		 :	DD 0xAAABACAD
ebpbVolume			 : 	DB "BOOT FLOPPY"
ebpbFileSystem		 :	DB "FAT12   "

;#########################################
;############ Bootloader code begins here
;#########################################

__VARIABLES_DECL:
	msgBootStarting  DB "Loading MyOS...", 0x0D, 0x0A, 0x00
	msgRootDirLoaded DB "Loaded root directory...", 0x0D, 0x0A, 0x00
	msgFailure 		 DB "Error in loading the operating system...", 0x0D, 0x0A, 0x00
	secondStageName  DB "INITKRNLASM"
	firstCluster  :	 DW 0x0000
	bufStart		 DW 0x0200
	
__FUNCTIONS_DECL:
	_PrintMessage:
		lodsb
		or al, al
		jz Done
		mov ah, 0Eh
		int 10h
		jmp _PrintMessage
	Done:
		ret
	
	_GetSectors:
		push ax
		push bx
		push cx
		
		call _LBAtoCHS
		mov ah, 0x02
		mov al, 0x01
		mov dl, BYTE [ebpbDriveNumber]
		int 13h
		jnc ReadSuccess
		
		mov si, msgFailure
		call _PrintMessage
		jmp __END_AND_FAILURE
		
	ReadSuccess:
		pop cx
		pop bx
		pop ax
		inc ax
		add bx, WORD [bpbBytesPerSector]
		loop _GetSectors
		;;;;;mov si, msgRootDirLoaded
		;;;;;call _PrintMessage
		ret
	
	_LBAtoCHS:
		xor dx, dx
		div WORD [bpbSectorsPerTrack]
		inc dl
		mov cl, dl
		xor dx, dx
		div WORD [bpbNumberOfHeads]
		mov dh, dl
		mov ch, al
		ret

__SEGMENTS_INIT:
	cli
	
	mov ax, 0x07C0
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ax, 0x9000
	mov ss, ax
	mov sp, 0xFFFF	
	
	sti
	jmp __LOAD_ROOT
	
__LOAD_ROOT:
	mov si, msgBootStarting
	call _PrintMessage
	
	xor ax, ax
	mov cx, ax
	mov dx, ax
	
	mov ax, 0x0020
	mul WORD [bpbNumberOfRootEnt]
	div WORD [bpbBytesPerSector]
	xchg ax, cx                     ;cx now has number of sectors used by root directory
	
	mov al, BYTE [bpbNumberOfFATs]
	mul WORD [bpbSectorsPerFAT]
	add ax, WORD [bpbReservedSectors]	;ax now has number of sectors before root directory by 0-based addressing
	
	mov bx, bufStart
	call _GetSectors
	
	mov cx, WORD [bpbNumberOfRootEnt]
	mov di, bufStart
	
   _ROOT_FIND_II_STAGE:
   	mov si, secondStageName
   	push cx
   	push di
   	mov cx, 0x000B
   	repe cmpsb
   	cmp cx, 0x0000
   	pop di
   	jz _FIND_FIRST_CLUSTER_II_STAGE
   	pop cx
   	add di, 0x0020
   	loop _ROOT_FIND_II_STAGE
   	
	mov si, msgFailure
	call _PrintMessage
	jmp __END_AND_FAILURE
	
   _FIND_FIRST_CLUSTER_II_STAGE:
   	mov cx, WORD [di + 0x001A]
   	mov WORD [firstCluster], cx
	
__LOAD_FAT:

__LOAD_SECOND_STAGE_SECTS:

__END_AND_FAILURE:
	cli
	hlt

;#########################################
;############### Zero out remaining bytes
;#########################################

times 510 - ($-$$) db 0

;#########################################
;######################### Boot signature
;#########################################

dw 0xAA55

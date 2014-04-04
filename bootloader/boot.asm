bits 16
org 0x0

jmp __SEGMENTS_INIT__

bpbOEMName				DB "Sunny S "
;##########################################
;#################### BIOS Parameter Block 
;##########################################

bpbBytesPerSector		:	DW 512
bpbSectorsPerCluster		:	DB 1
bpbReservedSectors		:	DW 1
bpbNumberOfFATs			:	DB 2
bpbNumberOfRootEnt		:	DW 224
bpbTotalSectors			:	DW 2880
bpbMedia			:	DB 0xF0
bpbSectorsPerFAT		:	DW 9
bpbSectorsPerTrack		:	DW 18
bpbNumberOfHeads		:	DW 2
bpbHiddenSectors		:	DD 0
bpbBigSectors			:	DD 0
ebpbDriveNumber			:	DB 0x0
ebpbFlags			:	DB 0
ebpbSignature			:	DB 0x29
ebpbVolumeID			:	DD 0xAAABACAD
ebpbVolume			: 	DB "BOOT FLOPPY"
ebpbFileSystem			:	DB "FAT12   "

;#########################################
;############ Bootloader code begins here
;#########################################

__VARIABLES_DECL__:
	msgBootStarting  		DB 0x0D, 0x0A, "**************************", 0x0D, 0x0A, 0x0D, 0x0A, "Booting Cygnus OS...", 0x0D, 0x0A, 0x00
	msgFailure 	 		DB "Error in loading the operating system...", 0x0D, 0x0A, 0x00
	msgBootComplete	 		DB "Boot finished.", 0x0D, 0x0A, 0x00
	nameSecondStage  		DB "INITKRNLBIN"
	numNextCluster 	 		DW 0
	addrBufStart	 		DW 0x0200
	addrDataRegion 	 		DW 0
	
__FUNCTIONS_DECL__:

	__PrintMessage_:
		lodsb
		or 	al, al
		jz 	_Done_
		mov 	ah, 0Eh
		int 	10h
		jmp 	__PrintMessage_
		
	_Done_:
		ret
	
	__GetSectors_:
		push 	ax
		push 	bx
		push 	cx
		
		call 	__LBAtoCHS_
		mov 	ah, 0x02
		mov 	al, 0x01
		mov 	dl, BYTE [ebpbDriveNumber]
		int 	13h
		jnc 	_ReadSuccess_
		
		jmp 	__END_AND_FAILURE__
		
	_ReadSuccess_:
		pop 	cx
		pop 	bx
		pop 	ax
		inc 	ax								;Point to next sector to read from 
		add 	bx, WORD [bpbBytesPerSector]					;advance write pointer to next block
		loop 	__GetSectors_
		
		ret
	
	__LBAtoCHS_:
		xor 	dx, dx
		div 	WORD [bpbSectorsPerTrack]
		inc 	dl
		mov 	cl, dl
		xor 	dx, dx
		div 	WORD [bpbNumberOfHeads]
		mov 	dh, dl
		mov 	ch, al
		
		ret
		
	__ClusterToLBA_:
		sub 	ax, 0x0002
		xor 	cx, cx
		mov 	cl, BYTE [bpbSectorsPerCluster]
		mul 	cl
		add 	ax, WORD [addrDataRegion]
		
		ret

__SEGMENTS_INIT__:
	cli
	
	mov 	ax, 0x07C0
	mov 	ds, ax
	mov 	es, ax
	mov 	fs, ax
	mov 	gs, ax
	
	mov 	ax, 0x9000
	mov 	ss, ax
	mov 	sp, 0xFFFF	
	
	sti
	
__LOAD_ROOT__:
	mov 	si, msgBootStarting
	call 	__PrintMessage_
	
	xor 	ax, ax
	mov 	cx, ax
	mov 	dx, ax
	
	mov 	ax, 0x0020
	mul 	WORD [bpbNumberOfRootEnt]
	div 	WORD [bpbBytesPerSector]
	xchg 	ax, cx          		           				;cx now has number of sectors used by root directory
	
	mov 	al, BYTE [bpbNumberOfFATs]
	mul 	WORD [bpbSectorsPerFAT]
	add 	ax, WORD [bpbReservedSectors]						;ax now has number of sectors before root directory by 0-based addressing
	
	mov 	WORD [addrDataRegion], ax
	add 	WORD [addrDataRegion], cx
	
	mov 	bx, WORD [addrBufStart]
	call 	__GetSectors_
	
	mov 	cx, WORD [bpbNumberOfRootEnt]
	mov 	di, WORD [addrBufStart]
	
   _ROOT_FIND_II_STAGE_:
   	mov 	si, nameSecondStage
   	push 	cx
   	push 	di
   	mov 	cx, 0x000B
   	repe 	cmpsb
   	cmp 	cx, 0x0000
   	pop 	di   	
   	jz 	_FIND_FIRST_CLUSTER_II_STAGE_  	
   	add 	di, 0x0020
   	pop 	cx   	
   	loop 	_ROOT_FIND_II_STAGE_
   	
	jmp 	__END_AND_FAILURE__
	
   _FIND_FIRST_CLUSTER_II_STAGE_:
   	mov 	cx, WORD [di + 0x001A]
   	mov 	WORD [numNextCluster], cx
   	
__LOAD_FAT__:
	mov 	cx, WORD [bpbSectorsPerFAT]
	mov 	ax, WORD [bpbReservedSectors]
	
	mov 	bx, WORD [addrBufStart]
	call 	__GetSectors_
	
	mov 	ax, 0x0050								;Location where second stage will be loaded
	mov 	es, ax
	mov 	bx, 0x0000								;Location where second stage will be loaded
	push 	bx
	
__LOAD_SECOND_STAGE_SECTS__:
	mov 	ax, WORD [numNextCluster]	
	call 	__ClusterToLBA_
	
	xor 	cx, cx
	mov 	cl, BYTE [bpbSectorsPerCluster]
	pop 	bx
	call 	__GetSectors_								;Get file cluster
	push 	bx

   _READ_ALL_SECTS_:
   	mov 	ax, WORD [numNextCluster]
	mov 	cx, ax
	mov 	dx, ax
	shr 	cx, 0x0001								;Divide cluster number by two
	add 	dx, cx									;Cluster number*1.5 = index into FAT
	mov 	bx, 0x0200
	add 	bx, dx
	mov 	dx, WORD [bx]
	test 	ax, 0x0001								;Similar to AND but does not change register values
	jz 	_EVEN_CLUSTER_
	
	shr 	dx, 0x0004
	jmp 	_CHECK_LAST_CLUSTER_
	
   _EVEN_CLUSTER_:
    	and 	dx, 0x0FFF
    
   _CHECK_LAST_CLUSTER_:
    	cmp 	dx, 0x0FF0
    	mov 	WORD [numNextCluster], dx						;dx has next cluster number
    	jnz 	__LOAD_SECOND_STAGE_SECTS__

;    	mov 	si, msgBootComplete
;    	call 	__PrintMessage_
    
    	push 	WORD 0x0050								;CS for second stage
    	push 	WORD 0x0000								;IP for second stage
	retf
	
__END_AND_FAILURE__:
	mov 	si, msgFailure
	call 	__PrintMessage_
	mov 	ah, 0x0
	int 	16h									;Wait for keypress (ah=0)
	int 	19h									;Reboot

;#########################################
;############### Zero out remaining bytes
;#########################################

times 510 - ($-$$) db 0

;#########################################
;######################### Boot signature
;#########################################

dw 0xAA55

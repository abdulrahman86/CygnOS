BITS 16	

bpbBytesPerSector	 :	DW 512
bpbSectorsPerCluster 	 :	DB 1
bpbReservedSectors	 :	DW 1
bpbNumberOfFATs		 :	DB 2
bpbNumberOfRootEnt	 :	DW 224
bpbTotalSectors		 :	DW 2880
bpbMedia		 :	DB 0xF0
bpbSectorsPerFAT	 :	DW 9
bpbSectorsPerTrack	 :	DW 18
bpbNumberOfHeads	 :	DW 2
bpbHiddenSectors	 :	DD 0
bpbBigSectors		 :	DD 0
ebpbDriveNumber		 :	DB 0x0
ebpbFlags		 :	DB 0
ebpbSignature		 :	DB 0x29
ebpbVolumeID		 :	DD 0xAAABACAD
ebpbVolume		 : 	DB "BOOT FLOPPY"
ebpbFileSystem		 :	DB "FAT12   "

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
		
	mov 	ax, -2			
	ret
		
	_ReadSuccess_:
		pop 	cx
		pop 	bx
		pop 	ax
		inc 	ax								
		add 	bx, WORD [bpbBytesPerSector]
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
	add 	ax, WORD [dataRegion]
		
	ret

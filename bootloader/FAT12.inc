BITS 16

dataRegion			DW 0x0
bufStart			DW 0x7E00
nextCluster 			DW 0x0
msgGetSectorsError		DB "Error in loading sectors off disk", 0x0D, 0x0A

__LoadRoot_:
	xor 	ax, ax
	mov 	cx, ax
	mov 	dx, ax
	
	mov 	ax, 0x0020
	mul 	WORD [bpbNumberOfRootEnt]
	div 	WORD [bpbBytesPerSector]
	xchg 	ax, cx                     	;cx now has number of sectors used by root directory
	
	mov 	al, BYTE [bpbNumberOfFATs]
	mul 	WORD [bpbSectorsPerFAT]
	add 	ax, WORD [bpbReservedSectors]	;ax now has number of sectors before root directory by 0-based addressing
	
	mov 	WORD [dataRegion], ax
	add 	WORD [dataRegion], cx
	
	mov 	bx, WORD [bufStart]
	call 	__GetSectors_
	
	cmp 	ax, -2
	jnz	_LoadRoot_End_
	
	mov	si, msgGetSectorsError
	call	__PrintMsg_	
	
	_LoadRoot_End_:
		ret

__LoadFAT_:
	mov 	cx, WORD [bpbSectorsPerFAT]
	mov 	ax, WORD [bpbReservedSectors]
	
	mov 	bx, WORD [bufStart]
	call 	__GetSectors_
	
	cmp	ax, -2
	jnz	_LoadFAT_End_
	
	mov	si, msgGetSectorsError
	call	__PrintMsg_


	_LoadFAT_End_:	
		ret
	
__FindFile_:
	call 	__LoadRoot_
	cmp	ax, -2
	jnz	_LoadRoot_Success_

	ret	

	_LoadRoot_Success_:	
		mov 	cx, WORD [bpbNumberOfRootEnt]
		mov 	di, WORD [bufStart]
	
	_Root_Find_File_:
		push 	si
		push 	cx
		push 	di
		mov 	cx, 0x000B
	  	rep 	cmpsb
		pop 	di
		cmp 	cx, 0x0
   		jz 	_Find_First_Cluster_File_
	   	add 	di, 0x0020
   		pop 	cx   
	   	pop 	si	
	   	loop 	_Root_Find_File_
   	
   		mov 	ax, -1
   	
	   	ret
	
   	_Find_First_Cluster_File_:
   		mov 	cx, WORD [di + 0x001A]
   		mov 	WORD [nextCluster], cx
   	
	pop 	cx
	pop 	si

   	ret
   	
__LoadFile_:
	push 	bx
	push 	dx	
	
	xor 	ax, ax
	call 	__FindFile_

	cmp 	ax, -2
	jnz 	_Check_Error_1_LoadFile_
	ret

  _Check_Error_1_LoadFile_:
	cmp	ax, -1
	jnz	_File_Found_
	ret
	
   _File_Found_:
   		call 	__LoadFAT_
		cmp 	ax, -2
		jnz	_LoadFAT_Success_	
		ret

  _LoadFAT_Success_:   
		pop 	dx
		mov 	es, dx

   _Read_File_:
		mov 	ax, WORD [nextCluster]	
		call 	__ClusterToLBA_
	
		xor 	cx, cx
		mov 	cl, BYTE [bpbSectorsPerCluster]
		add 	WORD [numFileSize], cx
	
		pop 	bx
		call 	__GetSectors_
		cmp 	ax, -2
		jnz 	_Read_All_Sects_
	
		mov 	si, msgGetSectorsError
		call 	__PrintMsg_
	
		ret
   
   _Read_All_Sects_:
		push 	bx   
   		mov 	ax, WORD [nextCluster]
		mov 	cx, ax
		mov 	dx, ax
		shr 	cx, 0x0001						
		add 	dx, cx							
		mov 	bx, WORD [bufStart]
		add 	bx, dx
		mov 	dx, WORD [bx]
		test 	ax, 0x0001						
		jz 	_Even_Cluster_
	
		shr 	dx, 0x0004
		jmp 	_Check_Last_Cluster_
	
   _Even_Cluster_:
		and 	dx, 0x0FFF
    
   _Check_Last_Cluster_:
		cmp 	dx, 0x0FF0
		mov 	WORD [nextCluster], dx			
		jnz 	_Read_File_

       pop 	bx
       ret

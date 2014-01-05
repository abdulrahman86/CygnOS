bits 16
org 0

jmp __SEGMENTS_INIT

bpbOEMName		DB "Sunny S "
###########################################
##################### BIOS Parameter Block 
###########################################

bpbBytesPerSector		DW 512
bpbSectorsPerCluster	DB 1
bpbReservedSectors		DW 1
bpbNumberOfFATs			DB 1
bpbNumberOfRootEnt		DW 224
bpbTotalSectors			DW 2880
bpbMedia				DB 0xF0
bpbSectorsPerFAT		DW 9
bpbSectorsPerTrack		DW 18
bpbNumberOfHeads		DW 2
bpbHiddenSectors		DD 0
bpbBigSectors			DD 0
ebpbDriveNumber			DB 0x0
ebpbFlags				DB 0
ebpbSignature			DB 0x29
ebpbVolumeID			DD 0xAAABACAD
ebpbVolume				DB "BOOT FLOPPY"
ebpbFileSystem			DB "FAT12   "

##########################################
############# Bootloader code begins here
##########################################

__SEGMENTS_INIT:
	cli
	
	mov ax, 0x07C0
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ax, 0x9000
	mov ss. ax
	mov sp, 0xFFFF	
	
	sti
	jmp __LOAD_ROOT
	
__VARIABLES_DECL:

__FUNCTIONS_DECL:

__LOAD_ROOT:

__LOAD_FAT:

__LOAD_SECOND_STAGE_SECTS:

##########################################
################ Zero out remaining bytes
##########################################

times 510 - ($-$$) db 0

##########################################
########################## Boot signature
##########################################

dw 0xAA55

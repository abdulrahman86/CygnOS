#!/bin/bash

CURDIR=`pwd`

IMAGES_HOME=$CURDIR/../../images
BUILD_OUTPUT_HOME=${IMAGES_HOME}/build
IMAGE_NAME=floppy.img


SRC_HOME=$CURDIR/../src


BOOT_DIR_NAME=boot


KRNL_DIR_NAME=kernel
KRNL_SRC_PATH=${SRC_HOME}/${KRNL_DIR_NAME}
KRNL_ASM_DIR_NAME=asm
KRNL_ASM_SRC_PATH=${KRNL_SRC_PATH}/${KRNL_ASM_DIR_NAME}
KRNL_LD_DIR_NAME=linker
KRNL_LD_SRC_PATH=${KRNL_SRC_PATH}/${KRNL_LD_DIR_NAME}


KRNL_OUT_PATH=${BUILD_OUTPUT_HOME}/${KRNL_DIR_NAME}
KRNL_ASM_OUT_PATH=${KRNL_OUT_PATH}/${KRNL_ASM_DIR_NAME}
OS_ISO_NAME=cygnusos.iso


if [ ! -d ${IMAGES_HOME} ]
then
	mkdir ${IMAGES_HOME}
fi

if [ ! -d ${BUILD_OUTPUT_HOME} ]
then
	mkdir ${BUILD_OUTPUT_HOME}
else
	rm	  -rf ${BUILD_OUTPUT_HOME}/* 	
fi

if [ ! -d ${KRNL_OUT_PATH} ]
then
	mkdir ${KRNL_OUT_PATH}
else
	rm    -rf ${KRNL_OUT_PATH}/*
fi

if [ ! -d ${KRNL_ASM_OUT_PATH} ]
then
	mkdir ${KRNL_ASM_OUT_PATH}
else
	rm    -rf ${KRNL_ASM_OUT_PATH}/*
fi

cd ${KRNL_SRC_PATH}
nasm 		-felf  	 ${KRNL_ASM_SRC_PATH}/kernelinit.asm 	-o ${KRNL_ASM_OUT_PATH}/kernelinit_s.o
i686-elf-gcc 	-c 	 kernelinit.c 							-o ${KRNL_OUT_PATH}/kernelinit.o		   	-std=gnu99     -ffreestanding -Wall 	-Wextra	-I${SRC_HOME}/include
i686-elf-gcc	-c	 gdt.c									-o ${KRNL_OUT_PATH}/gdt.o					-std=gnu99	   -ffreestanding -Wall		-Wextra	-I${SRC_HOME}/include
i686-elf-gcc 	-T 	 ${KRNL_LD_SRC_PATH}/kernelinit.ld 		-o ${BUILD_OUTPUT_HOME}/kernelinit.bin	 	-ffreestanding -O2 	      -nostdlib ${KRNL_ASM_OUT_PATH}/kernelinit_s.o ${KRNL_OUT_PATH}/kernelinit.o	${KRNL_OUT_PATH}/gdt.o	-lgcc


cd ${IMAGES_HOME}

mkdir 	-p 	isodir
mkdir 	-p 	isodir/boot
mkdir 	-p 	isodir/boot/grub

cp 		${BUILD_OUTPUT_HOME}/kernelinit.bin 	isodir/boot/cygnusos.bin
cp 		${SRC_HOME}/${BOOT_DIR_NAME}/grub.cfg 	isodir/boot/grub/grub.cfg

grub-mkrescue 	-o 	${OS_ISO_NAME} 	isodir

rm		-rf	isodir

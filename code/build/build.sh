#!/bin/bash

CURDIR=`pwd`

IMAGES_HOME=$CURDIR/../../images
BUILD_OUTPUT_HOME=${IMAGES_HOME}/build
IMAGE_NAME=floppy.img



SRC_HOME=$CURDIR/../src



BOOT_DIR_NAME=boot



KRNLINIT_DIR_NAME=kernel
KRNLINIT_SRC_PATH=${SRC_HOME}/${KRNLINIT_DIR_NAME}

KRNLINITASM_O=krnlinit_s.o
KRNLINITC_O=krnlinit.o
KRNLINIT_BIN=krnlinit.bin
KRNLINIT_OUT_PATH=${BUILD_OUTPUT_HOME}/${KRNLINIT_DIR_NAME}



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

if [ ! -d ${KRNLINIT_OUT_PATH} ]
then
	mkdir ${KRNLINIT_OUT_PATH}
else
	rm    -rf ${KRNLINIT_OUT_PATH}/*
fi

cd ${KRNLINIT_SRC_PATH}
nasm 		-felf  	 kernelinit.asm 	-o ${KRNLINIT_OUT_PATH}/${KRNLINITASM_O}
i686-elf-gcc 	-c 	 kernelinit.c 		-o ${KRNLINIT_OUT_PATH}/${KRNLINITC_O}   	-std=gnu99     -ffreestanding -Wall 	-Wextra
i686-elf-gcc 	-T 	 kernelinit_linker.ld 	-o ${BUILD_OUTPUT_HOME}/${KRNLINIT_BIN} 	-ffreestanding -O2 	      -nostdlib ${KRNLINIT_OUT_PATH}/${KRNLINITASM_O} ${KRNLINIT_OUT_PATH}/${KRNLINITC_O}	-lgcc



cd ${IMAGES_HOME}

mkdir 	-p 	isodir
mkdir 	-p 	isodir/boot
mkdir 	-p 	isodir/boot/grub

cp 		${BUILD_OUTPUT_HOME}/${KRNLINIT_BIN} 	isodir/boot/cygnusos.bin
cp 		${SRC_HOME}/${BOOT_DIR_NAME}/grub.cfg 	isodir/boot/grub/grub.cfg

grub-mkrescue 	-o 	${OS_ISO_NAME} 	isodir

rm		-rf	isodir

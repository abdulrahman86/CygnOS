#!/bin/bash

CURDIR=`pwd`

IFS='/' read -a CURDIR_DIR_ARRAY <<< "$CURDIR"
CURDIR_DIR_DEPTH=`expr ${#CURDIR_DIR_ARRAY[@]} - 2`
TARGET_NAME=${CURDIR_DIR_ARRAY[$CURDIR_DIR_DEPTH]}

IMAGES_HOME=$CURDIR/../../../images
OUTPUT_HOME=${IMAGES_HOME}/${TARGET_NAME}
IMAGE_NAME=floppy.img



SRC_HOME=$CURDIR/../src

BOOT_DIR_NAME=boot
BOOT_SRC_PATH=${SRC_HOME}/${BOOT_DIR_NAME}

BOOT_BIN=boot.bin
BOOT_BIN_PATH=${OUTPUT_HOME}/${BOOT_DIR_NAME}/${BOOT_BIN}
LOADKRNL_BIN=LOADKRNL.BIN
LOADKRNL_BIN_PATH=${OUTPUT_HOME}/${BOOT_DIR_NAME}/${LOADKRNL_BIN}

KERNEL_DIR_NAME=kernel
KERNEL_SRC_PATH=${SRC_HOME}/${KERNEL_DIR_NAME}

KRNLINIT_BIN=KRNLINIT.BIN
KRNLINIT_BIN_PATH=${OUTPUT_HOME}/${KERNEL_DIR_NAME}/${KRNLINIT_BIN}



LOOP_DEVICE=/dev/loop0
MOUNT_POINT=/media/${TARGET_NAME}
FSTYPE=vfat



if [ ! -d ${IMAGES_HOME} ]
then
	mkdir ${IMAGES_HOME}
fi

if [ ! -d ${OUTPUT_HOME} ]
then
	mkdir ${OUTPUT_HOME}
fi

cd ${OUTPUT_HOME}

if [ ! -d ${BOOT_DIR_NAME} ]
then
	mkdir ${BOOT_DIR_NAME}
else
	rm -rf ${BOOT_DIR_NAME}/*
fi

if [ ! -d ${KERNEL_DIR_NAME} ]
then
	mkdir ${KERNEL_DIR_NAME}
else
	rm -rf ${KERNEL_DIR_NAME}/*
fi


cd ${BOOT_SRC_PATH}
nasm -f bin 	  boot.asm -o ${BOOT_BIN_PATH}
nasm -f bin   loadkrnl.asm -o ${LOADKRNL_BIN_PATH}

#cd ${KERNEL_SRC_PATH}
#nasm -f bin kernelinit.asm -o ${KRNLINIT_BIN_PATH}

cd ${OUTPUT_HOME}

if [ -f ${IMAGE_NAME} ]
then
	rm ${IMAGE_NAME}
fi
mkfs.vfat -C ${IMAGE_NAME} 1440


dd if=${BOOT_BIN_PATH} of=${IMAGE_NAME} bs=512 conv=notrunc

losetup ${LOOP_DEVICE} ${IMAGE_NAME}
mount -t ${FSTYPE} -o loop ${LOOP_DEVICE} ${MOUNT_POINT}

cp ${LOADKRNL_BIN_PATH} ${MOUNT_POINT}
#cp ${KRNLINIT_BIN_PATH} ${MOUNT_POINT}
cp ~/OS/code/floppy/src/kernel/KERNEL.BIN ${MOUNT_POINT}

sleep 1
umount ${MOUNT_POINT}
losetup -d ${LOOP_DEVICE}

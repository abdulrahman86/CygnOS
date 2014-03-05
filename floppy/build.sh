#!/bin/bash

if [ -f ~/OS/floppy/floppy.img ]
then
	rm ~/OS/floppy/floppy.img
fi
mkfs.vfat -C ~/OS/floppy/floppy.img 1440

curdir=$PWD
cd ~/OS/bootloader/
nasm boot.asm -o boot.bin
dd if=boot.bin of=~/OS/floppy/floppy.img bs=512 conv=notrunc
nasm -f bin initkrnl.asm -o INITKRNL.BIN
cd $curdir

losetup /dev/loop0 ~/OS/floppy/floppy.img
mount -t vfat -o loop /dev/loop0 /media/floppy
cp ~/OS/bootloader/INITKRNL.BIN /media/floppy
sleep 1
umount /media/floppy
losetup -d /dev/loop0

if [ "$1" = "start" ]
then
	qemu -fda ~/OS/floppy/floppy.img
fi


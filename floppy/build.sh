#!/bin/bash

if [ -f ~/OS/floppy/floppy.img ]
then
	rm ~/OS/floppy/floppy.img
fi
mkfs.vfat -C ~/OS/floppy/floppy.img 1440

nasm ~/OS/bootloader/boot.asm -o ~/OS/bootloader/boot.bin
dd if=~/OS/bootloader/boot.bin of=~/OS/floppy/floppy.img bs=512 conv=notrunc

losetup /dev/loop0 ~/OS/floppy/floppy.img
mount -t vfat -o loop /dev/loop0 /media/floppy
nasm -f bin ~/OS/bootloader/initkrnl.asm -o ~/OS/bootloader/INITKRNL.BIN
cp ~/OS/bootloader/INITKRNL.BIN /media/floppy
sleep 1
umount /media/floppy
losetup -d /dev/loop0

if [ "$1" = "start" ]
then
	qemu -fda ~/OS/floppy/floppy.img
fi


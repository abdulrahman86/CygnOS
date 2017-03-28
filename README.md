# CygnOS
A hobby x86 OS created from scratch.

***Note: I haven't had the time to work on this of late (grad school -_-), but intend to get back to it as soon as possible. Currently, I'm in the middle of implementing the virtual memory manager (a first implementation of the physical memory manager is done), but might also take up development of the keyboard driver in parallel.***

Brief explanation of the directories:
**archive/floppy:** The first implementation of the OS was for booting off a floppy image. I wrote the entire floppy bootloader from scratch -- this folder contains this bootloader and some other files relevant to this boot scenario. This is no longer used since I've moved to using grub.
**code:** The actual code of the OS. The file and sub-directory names here should be self-explanatory.
**cross-compiler:** The cross-compiler toolchain required to run this OS. The toolchain is built for an x86-64 build host and should work as-is.
**docs:** Some miscellaneous notes.

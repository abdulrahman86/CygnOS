# CygnOS
A hobby x86 OS created from scratch.

***Note: I haven't had the time to work on this of late (grad school -_-), but intend to get back to it as soon as possible. Currently, everything up to paging (thus including boot, timer and interrupt initialization, physical memory manager and paging) has been implemented. I intend to take up development of the keyboard driver next.***

Brief explanation of the directories:

- **archive/floppy:** The first implementation of the OS was for booting off a floppy image. I wrote the entire floppy bootloader from scratch -- this folder contains this bootloader and some other files relevant to this boot scenario all contained in the _boot_ subdirectory. This is no longer used since I've moved to using grub.

- **code:** The actual code of the OS. The file and sub-directory names here should be self-explanatory.

- **cross-compiler:** The cross-compiler toolchain required to run this OS. The toolchain is built for an x86-64 build host and should work as-is.

- **docs:** Some miscellaneous notes.

## Dependencies and Build
A few packages are required to build the OS and run it. Build and development is with reference to an Ubuntu 16.04 AMD64 system with gcc 5.4.0. Some points about this:

- A cross-compilation toolchain is required since we're building specifically for an x86 system on an AMD64 system. I have already compiled a version of this toolchain and it is what is contained in the _cross-compiler_ directory mentioned above. Add that directory's _bin_ folder to the _PATH_ environment variable before building.
- An environment variable called _OS_HOME_ will be required for the build. Set this to the path of the folder where you downloaded the OS code.

Following are the required dependencies:

- nasm
- grub-mkrescue
- grub i386-pc. This can be installed by using the command _**sudo apt-get install grub-pc**_. Make sure you **don't** actually install grub-pc to a partition as this will screw up your system.
- xorriso (1.2.9 or later)
- qemu (for running the OS). You can use bochs too!

To build the OS (once all the environment variables mentioned above are set and the dependencies are installed), navigate to the _code/build_ directory above and just run _make_. This will generate all object files and the final image in a directory called _images_ under the OS' source root.

The image will be named _cygnusos.iso_ and be launched in qemu using _**qemu-system-i386 -cdrom (path of the images directory)/cygnusos.iso**_.

If all goes well, this will show the grub menu of the OS and you're good to go :)

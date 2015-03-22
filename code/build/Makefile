include Makeinc

BOOT_DIR = boot/

KRNL_HOME = $(SRC_HOME)kernel
LIB_HOME = $(SRC_HOME)lib

OBJS = $(shell find $(BUILD_HOME) -type f -name *.o)

LD_DIR = linker/
LD_SCRIPT = kernel.ld
LD_FILE = $(SRC_HOME)$(LD_DIR)$(LD_SCRIPT)

GRUB_MK_RESCUE = grub-mkrescue
GRUB_IMAGES_DIR = /usr/lib/grub/i386-pc/

KRNL_BINARY = kernel.bin
ISODIR = $(IMAGES_HOME)isodir/
OS_ISO = cygnusos.iso

all: compile buildbinary buildimage
 
compile:
	$(MAKE) -C $(KRNL_HOME)
	$(MAKE) -C $(LIB_HOME)
	
buildbinary:
	$(CC) -T $(LD_FILE) -o $(BUILD_HOME)$(KRNL_BINARY) $(LD_OTHERS) $(OBJS) $(LD_LIBS)

buildimage:
	mkdir -p $(ISODIR)boot/grub
	cp $(BUILD_HOME)$(KRNL_BINARY) $(ISODIR)boot/cygnusos.bin
	cp $(SRC_HOME)$(BOOT_DIR)grub.cfg $(ISODIR)boot/grub/grub.cfg
	$(GRUB_MK_RESCUE) -d $(GRUB_IMAGES_DIR) -o $(IMAGES_HOME)$(OS_ISO) $(ISODIR)
	rm -rf $(ISODIR)

clean:
	rm -rf $(IMAGES_HOME)

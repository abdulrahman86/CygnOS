CC = i686-elf-gcc


AS = nasm
ASFLAGS = -felf


IMAGES_HOME = $(CURDIR)/../../images/
BUILD_OUTPUT_DIR = build/
BUILD_OUTPUT_HOME = $(IMAGES_HOME)$(BUILD_OUTPUT_DIR)


SRC_HOME = $(CURDIR)/../src/

KRNL_SRC_DIR = kernel/
KRNL_SRC_PATH = $(SRC_HOME)$(KRNL_SRC_DIR)
#KRNL_SRC_FILES = $(wildcard (KRNL_SRC_PATH)*.c)

KRNL_SRC_ASM_DIR = asm/
KRNL_SRC_ASM_PATH = $(KRNL_SRC_PATH)$(KRNL_SRC_ASM_DIR)
#KRNL_SRC_ASM_FILES = $(wildcard $(KRNL_SRC_ASM_PATH)*.asm)

LIB_SRC_DIR = lib/
LIB_SRC_PATH = $(SRC_HOME)$(LIB_SRC_DIR)
#LIB_SRC_FILES = $(wildcard $(LIB_SRC_PATH)*.c)


KRNL_OUT_PATH = $(BUILD_OUTPUT_HOME)$(KRNL_SRC_DIR)
#KRNL_OBJ_FILES = $(pathsubst %.c,%.o,$(KRNL_SRC_FILES))

KRNL_ASM_OUT_PATH = $(KRNL_OUT_PATH)$(KRNL_SRC_ASM_DIR)
#KRNL_ASM_OBJ_FILES = $(pathsubst %.asm,%.o,$(KRNL_SRC_ASM_FILES))

LIB_OUT_PATH = $(BUILD_OUTPUT_HOME)$(LIB_SRC_DIR)
#LIB_OBJ_FILES = $(pathsubst %.c,%.o,$(LIB_SRC_FILES))

OS_ISO_NAME = cygnusos.iso


all: clean compile buildimage
 
compile: dirs ascode ccode

dirs:
	mkdir -p $(KRNL_ASM_OUT_PATH)
	mkdir -p $(LIB_OUT_PATH)

ascode:
	$(AS) $(ASFLAGS) $(KRNL_SRC_ASM_PATH)kernel.asm -o $(KRNL_ASM_OUT_PATH)kernel_s.o
	$(AS) $(ASFLAGS) $(KRNL_SRC_ASM_PATH)isr.asm -o $(KRNL_ASM_OUT_PATH)isr_s.o
	
ccode:

buildimage:

clean:
	rm -rf $(IMAGES_HOME)

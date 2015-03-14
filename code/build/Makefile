IMAGES_HOME = $(CURDIR)/../../images/
BUILD_OUTPUT_DIR = build/
BUILD_OUTPUT_HOME = $(IMAGES_HOME)$(BUILD_OUTPUT_DIR)


SRC_HOME = $(CURDIR)/../src/

INCLUDES_DIR = include
INCLUDES_DIR_PATH = $(SRC_HOME)$(INCLUDES_DIR)

KRNL_SRC_DIR = kernel/
KRNL_SRC_PATH = $(SRC_HOME)$(KRNL_SRC_DIR)
KRNL_SRC_FILES = $(wildcard $(KRNL_SRC_PATH)*.c)

KRNL_SRC_ASM_DIR = asm/
KRNL_SRC_ASM_PATH = $(KRNL_SRC_PATH)$(KRNL_SRC_ASM_DIR)

LIB_SRC_DIR = lib/
LIB_SRC_PATH = $(SRC_HOME)$(LIB_SRC_DIR)
LIB_SRC_FILES = $(wildcard $(LIB_SRC_PATH)*.c)


AS = nasm
ASFLAGS = -felf


CC = i686-elf-gcc
CF_INCLUDE = -I$(INCLUDES_DIR_PATH)
CF_CSTD = -std=gnu99
CF_OTHERS = -Wall -Wextra -ffreestanding
CFLAGS = $(CF_INCLUDE) $(CF_CSTD) $(CF_OTHERS)


KRNL_OUT_PATH = $(BUILD_OUTPUT_HOME)$(KRNL_SRC_DIR)
#KRNL_OBJ_RELDIR_FILES = ${KRNL_SRC_FILES:.c=.o}
KRNL_OBJ_FILES = $(pathsubst $(KRNL_SRC_PATH)%.c, $(KRNL_OUT_PATH)%.o, $(KRNL_SRC_FILES))

KRNL_ASM_OUT_PATH = $(KRNL_OUT_PATH)$(KRNL_SRC_ASM_DIR)

LIB_OUT_PATH = $(BUILD_OUTPUT_HOME)$(LIB_SRC_DIR)
LIB_OBJ_FILES = ${LIB_SRC_FILES:.c=.o}


OS_ISO_NAME = cygnusos.iso


all: compile buildimage
 
compile: dirs ascode ccode

dirs:
	mkdir -p $(KRNL_ASM_OUT_PATH)
	mkdir -p $(LIB_OUT_PATH)

ascode:
	$(AS) $(ASFLAGS) $(KRNL_SRC_ASM_PATH)kernel.asm -o $(KRNL_ASM_OUT_PATH)kernel_s.o
	$(AS) $(ASFLAGS) $(KRNL_SRC_ASM_PATH)isr.asm -o $(KRNL_ASM_OUT_PATH)isr_s.o
	
ccode: kernel lib

kernel: $(KRNL_OBJ_FILES)

lib: 

buildimage:

clean:
	rm -rf $(IMAGES_HOME)

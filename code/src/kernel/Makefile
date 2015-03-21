include $(CURDIR)/../../build/Makeinc

KRNL_DIR = $(shell basename $(CURDIR))
ASM_DIR = asm/

KRNL_SRCS = $(wildcard *.c)

KRNL_OBJS = $(KRNL_SRCS:.c=.o)
KRNL_OBJ_PATH = $(BUILD_HOME)$(KRNL_DIR)/
ASM_OBJ_PATH = $(KRNL_OBJ_PATH)$(ASM_DIR)

all: dirs assembly kernel

dirs:
	mkdir -p $(ASM_OBJ_PATH)

assembly:
	$(AS) $(ASFLAGS) $(ASM_DIR)kernel.asm -o $(ASM_OBJ_PATH)kernel_s.o
	$(AS) $(ASFLAGS) $(ASM_DIR)isr.asm -o $(ASM_OBJ_PATH)isr_s.o

kernel: $(KRNL_OBJS)
	mv *.o $(KRNL_OBJ_PATH)

clean:
	rm -rf $(KRNL_OBJ_PATH)

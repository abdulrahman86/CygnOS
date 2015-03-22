include $(CURDIR)/../../build/Makeinc

KRNL_DIR = $(shell basename $(CURDIR))
ASM_DIR = asm/

KRNL_SRCS = $(wildcard *.c)
ASM_SRCS = $(wildcard $(ASM_DIR)*.asm)

KRNL_OBJ_PATH = $(BUILD_HOME)$(KRNL_DIR)/
KRNL_OBJS = $(patsubst %.c, $(KRNL_OBJ_PATH)%.o, $(KRNL_SRCS))
ASM_OBJ_PATH = $(KRNL_OBJ_PATH)$(ASM_DIR)
ASM_OBJS = $(patsubst $(ASM_DIR)%.asm, $(ASM_OBJ_PATH)%_s.o, $(ASM_SRCS))

all: dirs $(ASM_OBJS) $(KRNL_OBJS)

dirs:
	@mkdir -p $(ASM_OBJ_PATH)

$(ASM_OBJS): $(ASM_OBJ_PATH)%_s.o: $(ASM_DIR)%.asm
	$(AS) $(ASFLAGS) $< -o $@

$(KRNL_OBJS): $(KRNL_OBJ_PATH)%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(KRNL_OBJ_PATH)

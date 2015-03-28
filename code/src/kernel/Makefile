include $(CURDIR)/../../build/Makeinc

KRNL_DIR = $(shell basename $(CURDIR))
ASM_DIR = asm/
MM_DIR = mm/

KRNL_SRCS = $(wildcard *.c)
ASM_SRCS = $(wildcard $(ASM_DIR)*.asm)
MM_SRCS = $(wildcard $(MM_DIR)*.c)

KRNL_OBJ_PATH = $(BUILD_HOME)$(KRNL_DIR)/
KRNL_OBJS = $(patsubst %.c, $(KRNL_OBJ_PATH)%.o, $(KRNL_SRCS))
ASM_OBJ_PATH = $(KRNL_OBJ_PATH)$(ASM_DIR)
ASM_OBJS = $(patsubst $(ASM_DIR)%.asm, $(ASM_OBJ_PATH)%_s.o, $(ASM_SRCS))
MM_OBJ_PATH = $(KRNL_OBJ_PATH)$(MM_DIR)
MM_OBJS = $(patsubst $(MM_DIR)%.c, $(MM_OBJ_PATH)%.o, $(MM_SRCS))

all: dirs $(ASM_OBJS) $(MM_OBJS) $(KRNL_OBJS)

dirs:
	@mkdir -p $(ASM_OBJ_PATH)
	@mkdir -p $(MM_OBJ_PATH)

$(ASM_OBJS): $(ASM_OBJ_PATH)%_s.o: $(ASM_DIR)%.asm
	$(AS) $(ASFLAGS) $< -o $@

$(MM_OBJS): $(MM_OBJ_PATH)%.o: $(MM_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(KRNL_OBJS): $(KRNL_OBJ_PATH)%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(KRNL_OBJ_PATH)

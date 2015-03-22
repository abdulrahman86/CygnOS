include $(CURDIR)/../../build/Makeinc

LIB_DIR = $(shell basename $(CURDIR))

LIB_SRCS = $(wildcard *.c)

LIB_OBJ_PATH = $(BUILD_HOME)$(LIB_DIR)/
LIB_OBJS = $(patsubst %.c, $(LIB_OBJ_PATH)%.o, $(LIB_SRCS))

all: dirs $(LIB_OBJS)

dirs:
	@mkdir -p $(LIB_OBJ_PATH)

$(LIB_OBJS): $(LIB_OBJ_PATH)%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(LIB_OBJ_PATH)

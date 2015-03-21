include $(CURDIR)/../../build/Makeinc

LIB_DIR = $(shell basename $(CURDIR))

LIB_SRCS = $(wildcard *.c)

LIB_OBJS = $(LIB_SRCS:.c=.o)
LIB_OBJ_PATH = $(BUILD_HOME)$(LIB_DIR)/

all: dirs libs

dirs:
	mkdir -p $(LIB_OBJ_PATH)

libs: $(LIB_OBJS)
	mv *.o $(LIB_OBJ_PATH)

clean:
	rm -rf $(LIB_OBJ_PATH)

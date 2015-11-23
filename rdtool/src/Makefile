# Compile the race detector library, which *includes* cilkplusrts

LIB_DIR = ../lib
TOOL = rd

include ../common.mk

SRC := shadow_stack.cpp debug_util.cpp print_addr.cpp mem_access.cpp om/om.c
OBJ := $(SRC:.cpp=.o)
#-include $(OBJ:.o=.d)
INC += -I$(RUNTIME_INTERNAL)/include
CFLAGS += $(INC)
CXXFLAGS += $(INC)
TARGET = $(LIB_DIR)/lib$(TOOL).a

all: $(TARGET)
$(TARGET): $(OBJ) $(LIB_DIR)

$(LIB_DIR):
	if [ ! -e $(LIB_DIR) ]; then mkdir -p $(LIB_DIR); fi



.PHONY: all clean

clean:
	rm -rf *.o $(LIB_DIR)/*

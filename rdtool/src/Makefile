# Compile the race detector library, which *includes* cilkplusrts
include ../common.mk

SRC := shadow_stack.cpp debug_util.cpp print_addr.cpp mem_access.cpp tsan.cpp omrd.cpp
#OBJ := $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(SRC)))
OBJ1 := $(SRC:.cpp=.o)
OBJ := $(OBJ1:.c=.o)
LIB_INC = -I$(RUNTIME_INTERNAL)/include
CFLAGS += $(LIB_INC)
CXXFLAGS += $(LIB_INC)
TARGET = $(LIB_DIR)/lib$(TOOL_NAME).a

all: $(TARGET)
include $(OBJ:.o=.d)

$(TARGET): $(OBJ) $(LIB_DIR)

stat: $(LIB_DIR)/libstattool.a

$(LIB_DIR)/libstattool.a: OBJ=stattool.o
$(LIB_DIR)/libstattool.a: stattool.o $(LIB_DIR)


.PHONY: all clean

$(LIB_DIR):
	if [ ! -e $(LIB_DIR) ]; then mkdir -p $(LIB_DIR); fi

clean:
	rm -rf *.o $(LIB_DIR)/* *.d

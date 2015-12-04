TOOL_HOME:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(TOOL_HOME)/config.mk

#TOOL_HOME ?= $(shell pwd)
TOOL_NAME = rd
LIB_DIR ?= $(TOOL_HOME)/lib
INC_DIR ?= $(TOOL_HOME)/include

CC = $(COMPILER_HOME)/bin/clang #-flto
CXX = $(COMPILER_HOME)/bin/clang++ #-flto

OPT_FLAG ?= -O3
CILKFLAGS += -fcilkplus -fno-inline-detach
INC = -I$(INC_DIR) -I$(COMPILER_HOME)/include
# -fno-omit-frame-pointer required to work properly
FLAGS += $(OPT_FLAG) -Wall -g -fno-omit-frame-pointer $(CILKFLAGS) $(INC) #-DNDEBUG
CFLAGS += $(FLAGS) -std=c99
CXXFLAGS += $(FLAGS) -std=c++11
LDFLAGS += 

ARFLAGS=
# #ARFLAGS=--plugin $(COMPILE_HOME/lib/LLVMgold.so
# LDLIBS = $(TOOL_LDLIBS) $(APP_LDLIBS)

## Each C source file will have a corresponding file of prerequisites.
## Include the prerequisites for each of our C source files.

# This rule generates a file of prerequisites (i.e., a makefile)
# called name.d from a C source file name.c.
# %.d: CFLAGS += -MM -MP
# %.d: %.c
# 	@set -e; rm -f $@; \
# 	$(CC) $(CFLAGS) -MF $@.$$$$ $<; \
# 	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
# 	rm -f $@.$$$$

# This rule generates a file of prerequisites (i.e., a makefile)
# called name.d from a CPP source file name.cpp.
# %.d: CXXFLAGS += -MM -MP
# %.d: %.cpp
# 	@set -e; rm -f $@; \
# 	$(CXX) $(CXXFLAGS) -MF $@.$$$$ $<; \
# 	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
# 	rm -f $@.$$$$

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

#$(LIB_DIR)
$(LIB_DIR)/lib%.a: $(OBJ)
	ar $(ARFLAGS) -r $@ $(OBJ)

#$(LIB_DIR)
$(LIB_DIR)/lib%.so: $(OBJ)
	$(CC) $(OBJ) -shared -o $@

.PRECIOUS: %.o

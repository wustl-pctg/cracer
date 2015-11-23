-include ../config.mk
CC ?= $(COMPILER_HOME)/bin/clang #-flto
CXX ?= $(COMPILER_HOME)/bin/clang++ #-flto

OPT_FLAG ?= -O3
# -fno-omit-frame-pointer required to work properly
FLAGS += $(OPT_FLAG) -Wall -g -fno-omit-frame-pointer #-DNDEBUG
CFLAGS += $(BASIC_FLAGS) -std=c99
CXXFLAGS += $(BASIC_FLAGS) -std=c++11

CILKFLAGS += -fcilkplus -fno-inline-detach
INC += -I../include/
# #ARFLAGS=--plugin $(COMPILE_HOME/lib/LLVMgold.so
LDFLAGS += $(MALLOC)
# LDFLAGS = -L$(LIB_DIR) $(APP_LDFLAGS)
# LDLIBS = $(TOOL_LDLIBS) $(APP_LDLIBS)

## Each C source file will have a corresponding file of prerequisites.
## Include the prerequisites for each of our C source files.

# This rule generates a file of prerequisites (i.e., a makefile)
# called name.d from a C source file name.c.
%.d: CFLAGS += -MM -MP
%.d: %.c
	@set -e; rm -f $@; \
	$(CC) $(CFLAGS) -MF $@.$$$$ $<; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# This rule generates a file of prerequisites (i.e., a makefile)
# called name.d from a CPP source file name.cpp.
%.d: CXXFLAGS += -MM -MP
%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) $(CXXFLAGS) -MF $@.$$$$ $<; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

#$(LIB_DIR)
$(LIB_DIR)/lib%.a: $(OBJ)
	ar rcs $@ $(OBJ)

#$(LIB_DIR)
$(LIB_DIR)/lib%.so: $(OBJ)
	$(CC) $(OBJ) -shared -o $@

include ../test.mk
TARGETS := matmul fft cholesky
TARGETS_BENCH := $(TARGETS:=_bench)
TARGETS_BASE := $(TARGETS:=_base)
TARGETS_ICC := $(TARGETS:=_icc)
TARGETS_INSERT := $(TARGETS:=_insert)
OBJ_DIR_BASE := ./obj_base
OBJ_DIR_BENCH := ./obj_rd
OBJ_DIR_INSERT := ./obj_insert
OBJ_DIR_ICC := ./obj_icc
LIB = $(LIB_DIR)/lib$(TOOL_NAME).a

default : $(TARGETS_BENCH)
all: $(TARGETS_BENCH) $(TARGETS_BASE) $(TARGETS_INSERT) $(TARGETS_ICC)
bench: $(TARGETS_BENCH)
base: $(TARGETS_BASE)
insert: $(TARGETS_INSERT)
icc: $(TARGETS_ICC)


$(TARGETS) : % : %_bench

%_all:
	$(MAKE) $(@:_all=_base)
	$(MAKE) $(@:_all=_insert)
	$(MAKE) $(@:_all=_bench)
	$(MAKE) $(@:_all=_icc)

%_base: OBJ_DIR := $(OBJ_DIR_BASE)
%_base: LDFLAGS += $(RUNTIME_LIB)
%_base: $(OBJ_DIR)/%.o $(OBJ_DIR)/getoptions.o
	$(CXX) $(OBJ_DIR)/$< $(OBJ_DIR)/getoptions.o $(LDFLAGS) -o $@

%_bench: OBJ_DIR := $(OBJ_DIR_BENCH)
%_bench: CFLAGS += $(RDFLAGS)
%_bench: CXXFLAGS += $(RDFLAGS)
%_bench: LDFLAGS += $(RUNTIME_LIB)
%_bench: $(OBJ_DIR)/%.o $(OBJ_DIR)/getoptions.o $(LIB)
	$(CXX) $(OBJ_DIR)/$< $(OBJ_DIR)/getoptions.o $(LIB) $(LDFLAGS) -o $@

%_insert: OBJ_DIR := $(OBJ_DIR_INSERT)
%_insert: CFLAGS += $(RDFLAGS) -DINSERTSONLY
%_insert: CXXFLAGS += $(RDFLAGS) -DINSERTSONLY
%_insert: LDFLAGS += $(RUNTIME_LIB)
%_insert: $(OBJ_DIR)/%.o $(OBJ_DIR)/getoptions.o $(LIB)
	$(CXX) $(OBJ_DIR)/$< $(OBJ_DIR)/getoptions.o $(LIB) $(LDFLAGS) -o $@

%_icc: OBJ_DIR := $(OBJ_DIR_ICC)
%_icc: LDFLAGS += $(MALLOC)
%_icc: CC := $(ICC)
%_icc: CXX := $(ICC)
%_icc: LDFLAGS += -lcilkrts
%_icc: $(OBJ_DIR)/%.o $(OBJ_DIR)/getoptions.o
	$(CXX) $(OBJ_DIR)/$< $(OBJ_DIR)/getoptions.o $(LDFLAGS) -o $@

.PHONY: default clean $(TARGETS) %_bench

clean:
	rm -rf *.o $(OBJ_DIR_BENCH) $(OBJ_DIR_INSERT)
	rm -rf $(TARGETS_BENCH) $(TARGETS_INSERT)

baseclean:
	rm -rf $(OBJ_DIR_BASE) $(OBJ_DIR_ICC)
	rm -rf $(TARGETS_BASE) $(TARGETS_ICC)

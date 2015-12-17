include ../test.mk
TARGETS := matmul fft cholesky cilksort
OBJ_DIR_PREFIX := ./obj
BIN_DIR := ./bin

TARGETS_BRD := $(TARGETS:=_brd)
TARGETS_BASE := $(TARGETS:=_base)
TARGETS_ICC := $(TARGETS:=_icc)
TARGETS_INSERT := $(TARGETS:=_insert)
TARGETS_CILKSAN := $(TARGETS:=_cilksan)

OBJ_DIR_BASE := $(OBJ_DIR_PREFIX:=_base)
OBJ_DIR_BRD := $(OBJ_DIR_PREFIX:=_brd)
OBJ_DIR_INSERT := $(OBJ_DIR_PREFIX:=_insert)
OBJ_DIR_ICC := $(OBJ_DIR_PREFIX:=_icc)

LIB = $(LIB_DIR)/lib$(TOOL_NAME).a

default : $(TARGETS_BRD)
all: $(TARGETS_BRD) $(TARGETS_BASE) $(TARGETS_INSERT) $(TARGETS_ICC) $(TARGETS_CILKSAN)
brd: $(TARGETS_BRD)
base: $(TARGETS_BASE)
insert: $(TARGETS_INSERT)
icc: $(TARGETS_ICC)
cilksan: $(TARGETS_CILKSAN)

$(TARGETS) : % : %_brd
$(BIN_DIR):
	mkdir $(BIN_DIR)

%_all:
	$(MAKE) $(@:_all=_base)
	$(MAKE) $(@:_all=_insert)
	$(MAKE) $(@:_all=_bench)
	$(MAKE) $(@:_all=_icc)

%_cilksan: $(CILKSAN_HOME) $(BIN_DIR)
	$(MAKE) -C $(CILKSAN_HOME)/src
	$(MAKE) -C $(CILKSAN_HOME)/test/bench $(@:_cilksan=)
	cp $(CILKSAN_HOME)/test/bench/$(@:_cilksan=) $(BIN_DIR)/$@

%_base: OBJ_DIR := $(OBJ_DIR_BASE)
%_base: LDFLAGS += $(RUNTIME_LIB)
%_base: $(OBJ_DIR)/%.o $(OBJ_DIR)/getoptions.o $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/$< $(OBJ_DIR)/getoptions.o $(LDFLAGS) -o $(BIN_DIR)/$@

%_brd: OBJ_DIR := $(OBJ_DIR_BRD)
%_brd: CFLAGS += $(RDFLAGS)
%_brd: CXXFLAGS += $(RDFLAGS)
%_brd: LDFLAGS += $(RUNTIME_LIB)
%_brd: $(OBJ_DIR)/%.o $(OBJ_DIR)/getoptions.o $(LIB) $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/$< $(OBJ_DIR)/getoptions.o $(LIB) $(LDFLAGS) -o $(BIN_DIR)/$@

%_insert: OBJ_DIR := $(OBJ_DIR_INSERT)
%_insert: CFLAGS += $(RDFLAGS) -DINSERTSONLY
%_insert: CXXFLAGS += $(RDFLAGS) -DINSERTSONLY
%_insert: LDFLAGS += $(RUNTIME_LIB)
%_insert: $(OBJ_DIR)/%.o $(OBJ_DIR)/getoptions.o $(LIB) $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/$< $(OBJ_DIR)/getoptions.o $(LIB) $(LDFLAGS) -o $(BIN_DIR)/$@

%_icc: OBJ_DIR := $(OBJ_DIR_ICC)
%_icc: LDFLAGS += $(MALLOC)
%_icc: CC := $(ICC)
%_icc: CXX := $(ICC)
%_icc: LDFLAGS += -lcilkrts -Wl,-rpath,$(INTEL_LIB)
%_icc: $(OBJ_DIR)/%.o $(OBJ_DIR)/getoptions.o $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/$< $(OBJ_DIR)/getoptions.o $(LDFLAGS) -o $(BIN_DIR)/$@


.PHONY: default clean $(TARGETS) %_brd %_base %_insert %_icc %_clean

clean:
	rm -rf *.o $(OBJ_DIR_BRD) $(OBJ_DIR_INSERT) $(OBJ_DIR_BASE) $(OBJ_DIR_ICC) $(BIN_DIR)

%_clean:
	rm -rf $(OBJ_DIR_PREFIX:=_$(@:_clean=))
	rm -rf $(BIN_DIR)/*_$(@:_clean=)

TARGETS := matmul heat fft cholesky cilksort fib fibx strassen
OBJ_DIR_PREFIX := obj
BIN_DIR := bin
CC = $(COMPILER_HOME)/bin/clang #-flto
CXX = $(COMPILER_HOME)/bin/clang++ #-flto
MODES = brd base icc insert cilksan qstat prof

ifeq ($(mode), brd)
	OBJ_DIR := $(OBJ_DIR_PREFIX:=_brd)
	CFLAGS += $(RDFLAGS)
	CXXFLAGS += $(RDFLAGS)
	LDFLAGS += $(RUNTIME_LIB)
	LIB = $(LIB_DIR)/lib$(TOOL_NAME).a
else ifeq ($(mode), base)
	OBJ_DIR := $(OBJ_DIR_PREFIX:=_base)
	LDFLAGS += $(RUNTIME_LIB)
else ifeq ($(mode), icc)
	OBJ_DIR := $(OBJ_DIR_PREFIX:=_icc)
	LDFLAGS += $(MALLOC)
	CC = $(ICC)
	CXX = $(ICC)
	LDFLAGS += -lcilkrts -Wl,-rpath,$(INTEL_LIB)
else ifeq ($(mode), insert)
	OBJ_DIR := $(OBJ_DIR_PREFIX:=_insert)
	CFLAGS += $(RDFLAGS) -DINSERTSONLY
	CXXFLAGS += $(RDFLAGS) -DINSERTSONLY
	LDFLAGS += $(RUNTIME_LIB)
	LIB = $(LIB_DIR)/lib$(TOOL_NAME).a
else ifeq ($(mode), cilksan)
	OBJ_DIR := $(OBJ_DIR_PREFIX:=_cilksan)
else ifeq ($(mode), qstat)
	OBJ_DIR := $(OBJ_DIR_PREFIX:=_qstat)
	CFLAGS += -fcilktool -fsanitize=thread -DSTATS
	CXXFLAGS += -fcilktool -fsanitize=thread -DSTATS
	LDFLAGS += $(RUNTIME_LIB)
	LIB = $(LIB_DIR)/libstattool.a
else ifeq ($(mode), prof)
	OBJ_DIR := $(OBJ_DIR_PREFIX:=_prof)
	CFLAGS += $(RDFLAGS) -DSTATS=2
	CXXFLAGS += $(RDFLAGS) -DSTATS=2
	LDFLAGS += $(RUNTIME_LIB)
	LIB = $(LIB_DIR)/librd.prof.a
endif
include ../test.mk

most: brd base insert icc cilksan
all: most qstat prof
brd:
	$(MAKE) $(TARGETS) mode=brd
base:
	$(MAKE) $(TARGETS) mode=base
qstat:
	$(MAKE) $(TARGETS) mode=qstat
prof:
	$(MAKE) $(TARGETS) mode=prof
insert:
	$(MAKE) $(TARGETS) mode=insert
icc:
	$(MAKE) $(TARGETS) mode=icc
icc:
	$(MAKE) $(TARGETS) mode=icc
cilksan:
	$(MAKE) $(TARGETS:=_cilksan) mode=cilksan

$(BIN_DIR):
	mkdir $(BIN_DIR)

%_cilksan: $(CILKSAN_HOME) $(BIN_DIR)
	$(MAKE) -C $(CILKSAN_HOME)/src
	$(MAKE) -C $(CILKSAN_HOME)/test/bench $(@:_cilksan=)
	cp $(CILKSAN_HOME)/test/bench/$(@:_cilksan=) $(BIN_DIR)/$@

ifdef mode
matmul: $(OBJ_DIR)/matmul.o $(OBJ_DIR)/getoptions.o
heat: $(OBJ_DIR)/heat.o $(OBJ_DIR)/getoptions.o
fft: $(OBJ_DIR)/fft.o $(OBJ_DIR)/getoptions.o
cholesky: $(OBJ_DIR)/cholesky.o $(OBJ_DIR)/getoptions.o
cilksort: $(OBJ_DIR)/cilksort.o $(OBJ_DIR)/getoptions.o
fib: $(OBJ_DIR)/fib.o $(OBJ_DIR)/getoptions.o
fibx: $(OBJ_DIR)/fibx.o $(OBJ_DIR)/getoptions.o
strassen: $(OBJ_DIR)/strassen.o $(OBJ_DIR)/getoptions.o
$(TARGETS): 
	$(CXX) $< $(OBJ_DIR)/getoptions.o $(LIB) $(LDFLAGS) -o $(BIN_DIR)/$@_$(mode)
else
$(TARGETS):
	$(MAKE) $@ mode=brd
endif

%_base:
	$(MAKE) $(@:_base=) mode=base
%_brd:
	$(MAKE) $(@:_brd=) mode=brd
%_prof:
	$(MAKE) $(@:_prof=) mode=prof
%_insert:
	$(MAKE) $(@:_insert=) mode=insert
%_qstat:
	$(MAKE) $(@:_qstat=) mode=qstat
%_icc:
	$(MAKE) $(@:_icc=) mode=icc

%_all:
	$(MAKE) $(@:_all=_base)
	$(MAKE) $(@:_all=_insert)
	$(MAKE) $(@:_all=_brd)
	$(MAKE) $(@:_all=_icc)
	$(MAKE) $(@:_all=_cilksan)

.PHONY: clean modeclean %_clean $(TARGETS) %_brd %_base %_insert %_icc %_prof %_qstat %_cilksan

%_clean:
	rm -rf $(BIN_DIR)/$(@:_clean=)*
	for m in $(MODES); do \
		rm -f $(OBJ_DIR_PREFIX)_$$m/$(@:_clean=.o); \
	done

clean:
	$(MAKE) modeclean mode=base
	$(MAKE) modeclean mode=brd
	$(MAKE) modeclean mode=insert

modeclean:
	rm -rf *.o $(OBJ_DIR) $(BIN_DIR)/*_$(mode)

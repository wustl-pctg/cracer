TARGETS = matmul heat fft cholesky cilksort fib fibx strassen
TARGETS += knapsack lu nqueens qsort rectmul test
OBJ_DIR_PREFIX := obj
BIN_DIR := bin
MODES = brd base icc insert cilksan qstat prof

ifeq ($(mode), brd)
	OBJ_DIR := $(OBJ_DIR_PREFIX:=_brd)
	CFLAGS += $(RDFLAGS)
	CXXFLAGS += $(RDFLAGS)
	LIB = $(LIB_DIR)/lib$(TOOL_NAME).a
	LDFLAGS += $(RUNTIME_LIB)
else ifeq ($(mode), base)
	OBJ_DIR := $(OBJ_DIR_PREFIX:=_base)
	LDFLAGS += $(RUNTIME_LIB)
else ifeq ($(mode), icc)
	OBJ_DIR := $(OBJ_DIR_PREFIX:=_icc)
	LDFLAGS += $(MALLOC)
	LDFLAGS += -lcilkrts -Wl,-rpath,$(INTEL_LIB)
else ifeq ($(mode), insert)
	OBJ_DIR := $(OBJ_DIR_PREFIX:=_insert)
	CFLAGS += $(INSERTFLAGS)
	CXXFLAGS += $(INSERTFLAGS)
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
# common.mk (included by test.mk) resets CC,CXX
ifeq ($(mode), icc)
	CC = $(ICC)
	CXX = $(ICC)
endif

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
cilksan:
	$(MAKE) $(TARGETS:=_cilksan) mode=cilksan

$(BIN_DIR):
	mkdir $(BIN_DIR)

%_cilksan: $(CILKSAN_HOME) $(BIN_DIR)
	$(MAKE) -C $(CILKSAN_HOME)/src
	$(MAKE) -C $(CILKSAN_HOME)/test/bench $(@:_cilksan=)
	cp $(CILKSAN_HOME)/test/bench/$(@:_cilksan=) $(BIN_DIR)/$@

ifdef mode
fib: $(OBJ_DIR)/fib.o $(OBJ_DIR)/getoptions.o
fibx: $(OBJ_DIR)/fibx.o $(OBJ_DIR)/getoptions.o
qsort: $(OBJ_DIR)/qsort.o $(OBJ_DIR)/getoptions.o
nqueens: $(OBJ_DIR)/nqueens.o $(OBJ_DIR)/getoptions.o
matmul: $(OBJ_DIR)/matmul.o $(OBJ_DIR)/getoptions.o
heat: $(OBJ_DIR)/heat.o $(OBJ_DIR)/getoptions.o
fft: $(OBJ_DIR)/fft.o $(OBJ_DIR)/getoptions.o
cholesky: $(OBJ_DIR)/cholesky.o $(OBJ_DIR)/getoptions.o
cilksort: $(OBJ_DIR)/cilksort.o $(OBJ_DIR)/getoptions.o
strassen: $(OBJ_DIR)/strassen.o $(OBJ_DIR)/getoptions.o
knapsack: $(OBJ_DIR)/knapsack.o $(OBJ_DIR)/getoptions.o
lu: $(OBJ_DIR)/lu.o $(OBJ_DIR)/getoptions.o
rectmul: $(OBJ_DIR)/rectmul.o $(OBJ_DIR)/getoptions.o
test: $(OBJ_DIR)/test.o

$(TARGETS): | $(BIN_DIR)
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
	rm -rf $(BIN_DIR)/$(@:_clean=)_*
	for m in $(MODES); do \
		rm -f $(OBJ_DIR_PREFIX)_$$m/$(@:_clean=.o); \
	done

clean:
	$(MAKE) modeclean mode=base
	$(MAKE) modeclean mode=brd
	$(MAKE) modeclean mode=insert

modeclean:
	rm -rf *.o $(OBJ_DIR) $(BIN_DIR)/*_$(mode)

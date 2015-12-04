include ../test.mk
TARGETS := matmul sum
TARGETS_BENCH := $(TARGETS:=_bench)
TARGETS_BASE := $(TARGETS:=_base)
OBJ_DIR_BASE := ./baseobj
OBJ_DIR_BENCH := ./rdobj
LIB = $(LIB_DIR)/lib$(TOOL_NAME).a

#fib fib-serial fibx cholesky cilksort fft heat knapsack lu \
matmul nqueens rectmul strassen qsort # throw foo

default : $(TARGETS_BENCH)
bench: $(TARGETS_BENCH)
base: $(TARGETS_BASE)


$(TARGETS) : % : %_bench

# fib: fib.o $(LIB)
# fib-serial: fib-serial.o 
# fibx: fibx.o
# cholesky: getoptions.o cholesky.o
# cilksort: getoptions.o cilksort.o
# fft: getoptions.o fft.o
# heat: getoptions.o heat.o
# knapsack: getoptions.o knapsack.o
# lu: getoptions.o lu.o
# matmul: getoptions.o matmul.o
# nqueens: getoptions.o nqueens.o
# rectmul: getoptions.o rectmul.o
# strassen: getoptions.o strassen.o
# loop-matmul: loop-matmul.o
# qsort: qsort.o

%_base: OBJ_DIR := $(OBJ_DIR_BENCH)
%_base: LDFLAGS += $(RUNTIME_LIB)
%_base: $(OBJ_DIR)/%.o $(OBJ_DIR)/getoptions.o
	$(CXX) $(OBJ_DIR)/$< $(OBJ_DIR)/getoptions.o $(LDFLAGS) -o $@

%_bench: OBJ_DIR := $(OBJ_DIR_BENCH)
%_bench: CFLAGS += $(RDFLAGS)
%_bench: CXXFLAGS += $(RDFLAGS)
%_bench: LDFLAGS += $(RUNTIME_LIB)
%_bench: $(OBJ_DIR)/%.o $(OBJ_DIR)/getoptions.o $(LIB)
	$(CXX) $(OBJ_DIR)/$< $(OBJ_DIR)/getoptions.o $(LIB) $(LDFLAGS) -o $@

.PHONY: default clean $(TARGETS) %_bench

clean:
	rm -rf *.o $(TARGETS_BENCH)
	rm -rf $(OBJ_DIR_BENCH)

baseclean:
	rm -rf $(TARGETS_BASE) $(OBJ_DIR_BASE)

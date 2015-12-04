include ../test.mk
TARGETS := matmul sum
TARGETS_BENCH := $(TARGETS:=_bench)
TARGETS_BASE := $(TARGETS:=_base)

#fib fib-serial fibx cholesky cilksort fft heat knapsack lu \
matmul nqueens rectmul strassen qsort # throw foo

default : $(TARGETS_BENCH)
bench: $(TARGETS_BENCH)
#base: $(TARGETS_BASE)

ALL_TESTS = $(TARGETS)
LIB = $(LIB_DIR)/lib$(TOOL_NAME).a

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

%_bench: CFLAGS += $(RDFLAGS)
%_bench: CXXFLAGS += $(RDFLAGS)
%_bench: LDFLAGS += $(RUNTIME_LIB)
%_bench: %.o getoptions.o $(LIB)
	$(CXX) $< getoptions.o $(LIB) $(LDFLAGS) -o $@

%_base: %.base.o getoptions.o
	$(CXX) $< getoptions.o $(LDFLAGS) -o $@

.PHONY: default clean $(TARGETS)

clean:
	rm -rf *.o $(TARGETS_BENCH)

baseclean:
	rm -rf $(TARGETS_BASE)

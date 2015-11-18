TARGETS := matmul fib

#fib fib-serial fibx cholesky cilksort fft heat knapsack lu \
matmul nqueens rectmul strassen qsort # throw foo

default : $(TARGETS)

ALL_TESTS = $(TARGETS)
LIB = ../libomrd.a

fib: fib.o $(LIB)
# fib-serial: fib-serial.o 
# fibx: fibx.o
# cholesky: getoptions.o cholesky.o
# cilksort: getoptions.o cilksort.o
# fft: getoptions.o fft.o
# heat: getoptions.o heat.o
# knapsack: getoptions.o knapsack.o
# lu: getoptions.o lu.o
matmul: getoptions.o matmul.o $(LIB)
# nqueens: getoptions.o nqueens.o
# rectmul: getoptions.o rectmul.o
# strassen: getoptions.o strassen.o
# loop-matmul: loop-matmul.o
# qsort: qsort.o

include ../test.mk

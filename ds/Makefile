#CC = gcc
CC= ../prob/support/cilkclocal
#CFLAGS = -Wall -UCILK -std=c99
CFLAGS = -Wall -DCILK #-save-temps #-Wc,-std=c99
LD =
INC = -Iutil
ds ?= pairheap dynarray counter conchash #skiplist btree dynarray
TARGETS = $(ds)

mode ?= bench # Default is to benchmark.
ifeq ($(mode),bench)
	CFLAGS += -O3 -march=native -flto -DNDEBUG #-UDEBUG
endif
ifeq ($(mode),debug)
	CFLAGS += -O0 -g -cilk-debug -save-temps -v1
endif
ifeq ($(mode),profile)
	CFLAGS += -O3 -UDEBUG # -cilk-profile
endif

all: bench
test: $(TARGETS:=_test)
bench: $(TARGETS:=_bench)

#%_test: LD += -lcheck
# %_test: INC += -I.
# %_test: test/%_test.cilk %_bench.cilk %.cilk %.cilkh
# 	$(CC) $(CFLAGS) test/$(@:=.cilk) -o $(@) $(INC) $(LD)

%_bench: UTIL += util/benchopt.c util/timer.c
%_bench: %_bench.cilk %.cilk %.cilkh %_batch.cilk
	$(CC) $(CFLAGS) $(@:=.cilk) $(@:_bench=.cilk) $(UTIL) -o $(@) $(INC) $(LD)

.PHONY: clean check-syntax all

check-syntax:
	$(CC) $(CFLAGS) -o /dev/null -c ${CHK_SOURCES} $(INC)

clean:
	rm -f *.i *.cilki *.cilkc *.o
	rm -f	$(TARGETS)
	rm -f $(TARGETS:=_test)
	rm -f $(TARGETS:=_bench)
	rm -rf *.dSYM

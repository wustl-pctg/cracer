CC = gcc
CFLAGS = -Wall -UCILK -std=c99 -x c
LD =
INC = -Iutil

ds ?= pairheap dynarray conchash #skiplist btree dynarray

TARGETS = $(ds)

ifeq ($(mode),)
	CFLAGS += -O3 -m64 -UDEBUG -DNDEBUG
endif
ifeq ($(mode),bench)
	CFLAGS += -O3 -m64 -UDEBUG -DNDEBUG
endif
ifeq ($(mode),debug)
	CFLAGS += -O0 -g -DDEBUG # -cilk-debug -save-temps
endif
ifeq ($(mode),profile)
	CFLAGS += -O3 -UDEBUG # -cilk-profile
endif

all: test bench
test: $(TARGETS:=_test)
bench: $(TARGETS:=_bench)

%_test: LD += -lcheck
%_test: INC += -I.
%_test: test/%_test.cilk %_bench.cilk %.cilk %.cilkh
	$(CC) $(CFLAGS) test/$(@:=.cilk) -o $(@) $(INC) $(LD)

%_bench: UTIL += util/benchopt.c util/timer.c
%_bench: %_bench.cilk %.cilk %.cilkh
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

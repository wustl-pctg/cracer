CILKC = ../support/cilkclocal
#CFLAGS = -g -D_CILK_DEBUG -DDEBUG
CFLAGS =
DEPS = $(wildcard util/*.h util/*.cilkh)
SRCS = $(wildcard *.cilk)
INC = util/parfor.cilk util/benchOptions.c
DS =
LD =

all: bench

%_bench: %_bench.cilk $(DEPS)
	$(CILKC) $(CFLAGS) $(@:=.cilk) $(INC) $(DS) -o $* $(LD)

cilk:
	cd ..; make
	make clean

bench: skip seq

skip: DS = ds/skiplist.cilk
skip: skiplist_bench

.PHONY: clean cilk-clean check-syntax

clean:
	rm -f *.i *.cilki *.cilkc *.d *.P *.o
	rm -f $(SRCS:.cilk=)

cilk-clean:
	cd ..;make clean
	make clean

check-syntax:
	$(CILKC) -o /dev/null -c ${CHK_SOURCES}

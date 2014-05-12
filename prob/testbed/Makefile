CILKC = ../support/cilkclocal
CFLAGS =
MAKEDEPEND = $(CILKC) -MM
DEPS = $(wildcard util/*.h util/*.cilkh)
SRCS = $(wildcard *.cilk)
INC = util/parfor.cilk util/benchOptions.c
DS =
LD =

ifeq ($(shell uname -s),Linux)
	CFLAGS += #-D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200809L
endif
ifeq ($(shell uname -s),Darwin)
	CFLAGS +=
endif

ifeq ($(mode),)
	mode = bench
endif
ifeq ($(mode),bench)
	CFLAGS += -O3 -DNDEBUG -m64
endif
ifeq ($(mode),debug)
	CFLAGS += -O0 -g -cilk-debug -save-temps -Wall
endif
ifeq ($(profile),1)
	CFLAGS += -cilk-profile #-Wc,"-p"
endif

all: tests

# Rule for all actual source files
# May need to add -DUSE_LOCKS for ...Lock_test files
%_test: %_test.cilk $(DEPS)
	$(CILKC) $(CFLAGS) $(@:=.cilk) $(INC) $(DS) -o $* $(LD)

# Rule for rebuilding CILK (BATCHER)
cilk:
	cd ..; make
	make clean

# Building our tests
tests:  param counter btree dynArray concHash omPMA omTree stack

# Parameterized Test
param: param.cilk
	$(CILKC) $(CFLAGS) $@.cilk $(INC) -lm -o $@
race: DS = ds/stack.cilk
race: race.cilk
	$(CILKC) $(CFLAGS) $@.cilk $(INC) $(DS) -o $@
seq: DS = ds/stack.cilk
seq: seq.cilk
	$(CILKC) $(CFLAGS) $@.cilk $(INC) $(DS) -o $@

# Data Structure Tests
counter: counterLock_test counterBatch_test
btree: DS = ds/btree.cilk
btree: btree_test
dynArray: dynArrayLock_test dynArrayBatch_test
concHash: LD += -lm
concHash: concHashLock_test concHashBatch_test
omPMA: LD += -lm
omPMA: omPMALock_test omPMABatch_test
omTree: LD += -lm
omTree: omTreeLock_test omTreeBatch_test
stack: DS = ds/stack.cilk
stack: stackBatch_test
fcstack: DS = ds/stack_fc.cilk
fcstack: stackFC_test
skip: DS = ds/skiplist.cilk
skip: skiplist_test
noSpawn: stackNoSpawn_test

.PHONY: clean cilk-clean check-syntax doc information all
clean:
	rm -f *.i *.cilki *.cilkc *.d *.P *.o
	rm -f $(SRCS:.cilk=)

cilk-clean:
	cd ..;make clean
	make clean

check-syntax:
	$(CILKC) -o /dev/null -c ${CHK_SOURCES}

doc:
	@command -v doxygen >/dev/null 2>&1 || { echo "Doxygen not installed. Aborting." >&2; exit 1; }
	doxygen Doxyfile

information:
ifneq ($(mode),bench)
ifneq ($(mode),debug)
	@echo "Invalid build mode."
	@echo "Only 'mode=bench' and 'mode=debug' are currently supported."
	@exit 1
endif
endif
	@echo "Building on "$(mode)" mode"
	@echo ".........................."

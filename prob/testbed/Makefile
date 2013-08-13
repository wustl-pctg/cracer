CILKC = ../support/cilkclocal
CFLAGS =
MAKEDEPEND = $(CILKC) -MM
DEPS = $(wildcard util/*.h util/*.cilkh)
DS = $(wildcard ds/*.c ds/*.cilk)
SRCS = $(wildcard *.cilk)
LD =

ifeq ($(shell uname -s),Linux)
	CFLAGS += -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200809L
endif
ifeq ($(shell uname -s),Darwin)
	CFLAGS +=
endif

ifeq ($(mode),)
	mode = bench
endif
ifeq ($(mode),bench)
	CFLAGS += -O3 -DNDEBUG
endif
ifeq ($(mode),debug)
	CFLAGS += -O0 -g -cilk-debug -save-temps -DDEBUG -Wall
endif
ifeq ($(profile),1)
	CFLAGS += -cilk-profile -Wc,"-pg"
endif

all: cilk tests

# Rule for all actual source files
# May need to add -DUSE_LOCKS for ...Lock_test files
%_test: $(DEPS)
	$(CILKC) $(CFLAGS) $(@:=.cilk) -o $@ $(LD)


# Rule for rebuilding CILK (BATCHER)
cilk:
	cd ..; make
	make clean

# Building our tests
tests:  param counter btree dynArray concHash omPMA omTree

# Parameterized Test
param: param.cilk
	$(CILKC) $(CFLAGS) -o $@ $@.cilk

# Data Structure Tests
counter: counterLock_test counterBatch_test
btree: btreeLock_test btreeBatch_test
dynArray: dynArrayLock_test dynArrayBatch_test
concHash: concHashLock_test concHashBatch_test
omPMA: LD += -lm
omPMA: omPMALock_test omPMABatch_test
omTree: omTreeLock_test omTreeBatch_test

.PHONY: clean cilk-clean check-syntax doc information all
clean:
	rm -f *.i *.cilki *.cilkc *.d *.P *.o *.d
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
	@echo "Only 'mode=test' and 'mode=debug' are currently supported."
	@exit 1
endif
endif
	@echo "Building on "$(mode)" mode"
	@echo ".........................."

CILK = ../support/cilkclocal
CFLAGS =
MAKEDEPEND = $(CILK) -MM
SRCS = $(wildcard *.cilk)
DEPS = $(wildcard util/*)

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
	CFLAGS += -cilk-profile
endif

all: cilk tests

# Rule for all actual source files
% : %.cilk $(DEPS)
	$(CILK) $(CFLAGS) -o $@ $<

# Rule for rebuilding CILK (BATCHER)
cilk:
	cd ..; make
	make clean

# Building our tests
tests:  counter param btree dynArray concHash omPMA omTree
counter: counter_lock counter_batch

.PHONY: clean cilk-clean check-syntax doc information
clean:
	rm -f *.i *.cilki *.cilkc *.d *.P *.o
	rm -f $(SRCS:.cilk=)

cilk-clean:
	cd ..;make clean
	make clean

check-syntax:
	$(CILK) -o /dev/null -c ${CHK_SOURCES}

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

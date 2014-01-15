CILKC = ../../support/cilkclocal
CFLAGS = -c
MAKEDEPEND = $(CILKC) -MM
# DEPS = $(wildcard util/*.h util/*.cilkh)
#SRCS = $(wildcard *.cilk)

ifeq ($(shell uname -s),Linux)
	CFLAGS += #-D_XOPEN_SOURCE=600 #-D_POSIX_C_SOURCE=200809L
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

# Get rid of implicit rules for .o files
.SUFFIXES:
	MAKEFLAGS += -r

SRCS = btree.cilk concHash.cilk dynArray.cilk omPMA.cilk omTree.cilk
OBJS = $(SRCS:.cilk=.o)
all: $(OBJS)

counter: counter.o
btree: btree.o
concHash: concHash.o
dynArray: dynArray.o
omPMA: omPMA.o
omTree: omTree.o

# Need to add DEPS later
$(OBJS): $<

%.o: %.cilk %.cilkh
	$(CILKC) $(CFLAGS) $< -o $@

.PHONY: clean check-syntax all

check-syntax:
	$(CILKC) -o /dev/null -c ${CHK_SOURCES}

clean:
	rm -f *.i *.cilki *.cilkc *.o

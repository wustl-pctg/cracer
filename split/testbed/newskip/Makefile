# CILKC=../../support/cilkclocal
# CFLAGS=-g -O2 -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200809L
CILKC=gcc
CFLAGS=-g -O0

all: unit

skip: skiplist.c skiplist.h
	${CILKC} ${CFLAGS} skiplist.c -c

bench: skip skiplist_bench.c
	${CILKC} ${CFLAGS} skiplist_bench.c -o bench

unit: skip test/skiplist_unit.c
	${CILKC} ${CFLAGS} test/skiplist_unit.c -o unit

clean:

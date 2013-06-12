CILK=../cilkclocal
CFLAGS=-g -O2 -save-temps -I../ -I./ -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200809L

all: btree

btree: btree.h btree.cilk ../cilkclocal
	${CILK} ${CFLAGS} -o btree btree.cilk

ds_btree: ../ds_api.cilkh btree.h btree.cilk ../cilkclocal
	${CILK} ${CFLAGS} -o ds_btree btree.cilk

clean:
	rm -f btree

CILK=./cilkclocal
CFLAGS=-save-temps -O2 -I./ -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200809L

all: ds_hello ds_incr incr

ds_hello: ds_api.h ds_hello.cilk cilkclocal
	${CILK} ${CFLAGS} -o ds_hello ds_hello.cilk
	cp ds_hello.cilkc ds_hello.c

ds_incr: ds_api.h ds_incr.cilk cilkclocal
	${CILK} ${CFLAGS} -o ds_incr ds_incr.cilk
	cp ds_incr.cilkc ds_incr.c

incr: ds_api.h incr.cilk cilkclocal
	${CILK} ${CFLAGS} -o incr incr.cilk
	cp incr.cilkc incr.c

clean:
	rm -f ds_hello ds_hello.cilkc ds_hello.c ds_hello.i ds_hello.cilki
	rm -f ds_incr ds_incr.cilkc ds_incr.c ds_incr.i ds_incr.cilki
	rm -f incr incr.cilkc incr.c incr.i incr.cilki

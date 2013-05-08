CILK=./cilkclocal
CFLAGS=-g -O2 -save-temps -I./ -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200809L

all: ds_incr incr

ds_incr: ds_api.cilkh ds_incr.cilk cilkclocal
	${CILK} ${CFLAGS} -o ds_incr ds_incr.cilk

incr: ds_api.cilkh incr.cilk cilkclocal
	${CILK} ${CFLAGS} -o incr incr.cilk

clean:
	rm -f ds_incr ds_incr.cilkc ds_incr.i ds_incr.cilki
	rm -f incr incr.cilkc incr.i incr.cilki

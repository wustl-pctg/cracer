CILK=./cilkclocal
CFLAGS= -I./ -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200809L

all: ds_hello

ds_hello: ds_api.h ds_hello.cilk cilkclocal
	${CILK} ${CFLAGS} -o ds_hello ds_hello.cilk

clean:
	rm -f ds_hello

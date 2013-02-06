CILK=./cilkclocal
CFLAGS= -save-temps -g -I./ -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200809L

all: ds_hello

ds_hello: ds_api.h ds_hello.cilk cilkclocal
	${CILK} ${CFLAGS} -o ds_hello ds_hello.cilk
	cp ds_hello.cilkc ds_hello.c

clean:
	rm -f ds_hello

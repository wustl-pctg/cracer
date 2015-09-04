CC=clang -flto
CXX=clang++ -flto
COMPILER_HOME=$(HOME)/llvm-cilk
RUNTIME=$(COMPILER_HOME)/lib/x86_64/libcilkrts.a
MALLOC= 
#MALLOC=-ltcmalloc
#MALLOC=-ltbbmalloc_proxy
USE_GOLD=true

CFLAGS=-std=c99 -g -O0
CXXFLAGS=-std=c++11 -g -O0
CILKFLAGS=-fcilkplus -fno-inline-detach
RDFLAGS=-DSTATS -DRACEDETECT -fsanitize=thread -fcilktool
ARFLAGS=--plugin $(COMPILER_HOME)/lib/LLVMgold.so
OMINC=-I../cilkplusrts/include -I$(COMPILER_HOME)/src/projects/compiler-rt/lib
USERINC=-I$(COMPILER_HOME)/include

all: matmul fib
fib: fib_check fib_nocheck
matmul: matmul_check matmul_nocheck

fib.o: fib.cpp
	$(CXX) $(CILKFLAGS) $(CXXFLAGS) $(USERINC) -c fib.cpp -o fib.o

fibrd.o: fib.cpp
	$(CXX) $(CILKFLAGS) $(CXXFLAGS) $(USERINC) $(RDFLAGS) -c fib.cpp -o fibrd.o

getoptions.o: getoptions.cpp
	$(CXX) $(CXXFLAGS) -c getoptions.cpp

matmul.o: matmul.cpp
	$(CXX) $(CILKFLAGS) $(CXXFLAGS) $(USERINC) -c matmul.cpp -o matmul.o

matmulrd.o: matmul.cpp
	$(CXX) $(CILKFLAGS) $(CXXFLAGS) $(USERINC) $(RDFLAGS) -c matmul.cpp -o matmulrd.o

omrd.o: ../omrd.cpp ../../om/om.c
	cd .. && $(CXX) $(CILKFLAGS) $(CXXFLAGS) $(OMINC) -DSTATS -c omrd.cpp && cd -

libomrd.a: omrd.o
	cd .. && ar $(ARFLAGS) -r libomrd.a omrd.o && cd -

fib_check: fibrd.o ../libomrd.a
	$(CXX) fibrd.o ../libomrd.a $(RUNTIME) -ldl -lpthread $(MALLOC) -o fib_check

fib_nocheck: fib.o
	$(CXX) fib.o $(RUNTIME) -ldl -lpthread $(MALLOC) -o fib_nocheck

matmul_check: matmulrd.o ../libomrd.a
	$(CXX) matmulrd.o getoptions.o ../libomrd.a $(RUNTIME) -ldl -lpthread $(MALLOC) -o matmul_check

matmul_nocheck: matmul.o
	$(CXX) matmul.o getoptions.o $(RUNTIME) -ldl -lpthread $(MALLOC) -o matmul_nocheck

.PHONY: all fib matmul clean
clean:
	rm -rf ../libomrd.a ../omrd.o
	rm -rf fib.o fibrd.o fib_nocheck fib_check
	rm -rf matmul.o matmulrd.o matmul_nochec matmul_check

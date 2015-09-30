CC=clang #-flto
CXX=clang++ #-flto
#USE_GOLD=true
COMPILER_HOME=$(HOME)/llvm-cilk
RUNTIME=$(COMPILER_HOME)/lib/x86_64/libcilkrts.a
MALLOC= 
#MALLOC=-ltcmalloc
#MALLOC=-ltbbmalloc_proxy


CFLAGS=-std=c99 -O0 -g
CXXFLAGS=-std=c++11 -O0 -g #-DNDEBUG
CILKFLAGS=-fcilkplus -fno-inline-detach
RDFLAGS=-DRACEDETECT -fcilktool -fsanitize=thread 
ARFLAGS=#--plugin $(COMPILER_HOME)/lib/LLVMgold.so
LDFLAGS=
OMINC=-I../cilkplusrts/include #-I$(COMPILER_HOME)/src/projects/compiler-rt/lib
USERINC=-I$(COMPILER_HOME)/include
STAT_LEVEL=0

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

omrd: ../omrd.cpp ../../om/om.c ../stack.h
	cd ..; $(CXX) $(CILKFLAGS) $(CXXFLAGS) $(OMINC) -DSTATS=$(STAT_LEVEL) -c omrd.cpp
	cd ..; $(CXX) $(CXXFLAGS) $(OMINC) -c tsan.cpp
	cd ..; $(CXX) $(CXXFLAGS) -c debug_util.cpp
	cd ..; $(CXX) $(CXXFLAGS) -c mem_access.cpp
	cd ..; $(CXX) $(CXXFLAGS) -c print_addr.cpp

libomrd: omrd
	cd ..; ar $(ARFLAGS) -r libomrd.a omrd.o tsan.o debug_util.o mem_access.o print_addr.o

fib_check: fibrd.o libomrd
	$(CXX) $(LDFLAGS) fibrd.o ../libomrd.a $(RUNTIME) -ldl -lpthread $(MALLOC) -o fib_check

fib_nocheck: fib.o
	$(CXX) fib.o $(RUNTIME) -ldl -lpthread $(MALLOC) -o fib_nocheck

matmul_check: matmulrd.o libomrd getoptions.o
	$(CXX) $(LDFLAGS) matmulrd.o getoptions.o ../libomrd.a $(RUNTIME) -ldl -lpthread $(MALLOC) -o matmul_check

matmul_nocheck: matmul.o getoptions.o
	$(CXX) matmul.o getoptions.o $(RUNTIME) -ldl -lpthread $(MALLOC) -o matmul_nocheck

.PHONY: all fib matmul clean libomrd omrd
clean:
	rm -rf ../libomrd.a ../omrd.o
	rm -rf fib.o fibrd.o fib_nocheck fib_check
	rm -rf matmul.o matmulrd.o matmul_nocheck matmul_check

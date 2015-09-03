CXX=clang++
CC=clang
CFLAGS=-O0 -std=c99 -g
CXXFLAGS=-O0 -std=c++11 -g
CILKFLAGS=-fcilkplus -fcilktool -fno-inline-detach
RDFLAGS=-fsanitize=thread
RUNTIME=$(HOME)/llvm-cilk/lib/x86_64/libcilkrts.a
INC=-I../cilkplusrts/include
MALLOC= #-ltcmalloc #-ltbbmalloc_proxy


all: matmul fib
fib: fib_check fib_nocheck
matmul: matmul_check matmul_nocheck

fib.o: fib.cpp
	$(CXX) $(CILKFLAGS) $(CXXFLAGS) -std=c++11 -c fib.cpp -o fib.o

fibrd.o: fib.cpp
	$(CXX) $(CILKFLAGS) $(RDFLAGS) $(CXXFLAGS) -I../../cilkplusrts/include -DSTATS -DRACEDETECT -std=c++11 -c fib.cpp -o fibrd.o

getoptions.o: getoptions.cpp
	$(CXX) $(CXXFLAGS) -c getoptions.cpp

matmul.o: matmul.cpp
	$(CXX) $(CILKFLAGS) $(CXXFLAGS) -std=c++11 -c matmul.cpp -o matmul.o

matmulrd.o: matmul.cpp
	$(CXX) $(CILKFLAGS) $(RDFLAGS) $(CXXFLAGS) -DSTATS -DRACEDETECT -std=c++11 -c matmul.cpp -o matmulrd.o

# om.o: ../../om/om.c
# 	cd ../../om && $(CC) $(CFLAGS) -c om.c && mv om.o ../rdtool/ && cd -

omrd.o: ../omrd.cpp ../../om/om.c
	cd .. && $(CXX) $(CILKFLAGS) $(CXXFLAGS) $(INC) -DSTATS -c omrd.cpp && cd -

libomrd.a: omrd.o #om.o
	cd .. && ar -r libomrd.a omrd.o && cd -

#-Wl,--whole-archive 
fib_check: fibrd.o libomrd.a
	$(CXX) fibrd.o ../libomrd.a $(RUNTIME) -ldl -lpthread $(MALLOC) -o fib_check

fib_nocheck: fib.o
	$(CXX) -ldl -lpthread $(MALLOC) -o fib_nocheck fib.o $(RUNTIME)

matmul_check: matmulrd.o libomrd.a
	$(CXX) -ldl -lpthread $(MALLOC) $(RDFLAGS) -Wl,--whole-archive ../libomrd.a -Wl,--no-whole-archive -o matmul_check matmulrd.o getoptions.o $(RUNTIME)

matmul_nocheck: matmul.o
	$(CXX) -ldl -lpthread $(MALLOC) -o matmul_nocheck matmul.o getoptions.o $(RUNTIME)

clean:
	rm -rf ../libomrd.a ../omrd.o
	rm -rf fib.o fibrd.o fib_nocheck fib_check
	rm -rf matmul.o matmulrd.o matmul_nochec matmul_check

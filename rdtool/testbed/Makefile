CXX=clang++
CC=clang
CFLAGS=-g -O0 -std=c99 -DNDEBUG
CXXFLAGS=-g -O0 -std=c++11 -DNDEBUG
CILKFLAGS=-fcilkplus -fcilktool -fno-inline-detach
RUNTIME=/home/rob/cilkplus/lib/x86_64/libcilkrts.a
INC=-I/home/rob/devel/batch/cilkplusrts/include


all: fib_check fib_nocheck

fib.o: fib.cpp
	$(CXX) $(CILKFLAGS) $(CXXFLAGS) -std=c++11 -c fib.cpp -o fib.o

fibrd.o: fib.cpp
	$(CXX) $(CILKFLAGS) $(CXXFLAGS) -DRACEDETECT -std=c++11 -c fib.cpp -o fibrd.o

# om.o: ../../om/om.c
# 	cd ../../om && $(CC) $(CFLAGS) -c om.c && mv om.o ../rdtool/ && cd -

omrd.o: ../omrd.cpp ../../om/om.c
	cd .. && $(CXX) $(CILKFLAGS) $(CXXFLAGS) $(INC) -c omrd.cpp && cd -

libomrd.a: omrd.o #om.o
	cd .. && ar -r libomrd.a omrd.o && cd -

fib_check: fibrd.o libomrd.a
	$(CXX) -ldl -lpthread -Wl,--whole-archive ../libomrd.a -Wl,--no-whole-archive -o fib_check fibrd.o $(RUNTIME) 

fib_nocheck: fib.o
	$(CXX) -ldl -lpthread -o fib_nocheck fib.o $(RUNTIME)

clean:
	rm -rf fib.o fibrd.o fib_nocheck fib_check ../libomrd.a ../omrd.o

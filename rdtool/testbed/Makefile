CXX=clang++
CC=clang
CFLAGS=-O3 -g -std=c99 -DNDEBUG
CXXFLAGS=-O3 -g -std=c++11 -DNDEBUG
CILKFLAGS=-fcilkplus -fcilktool -fno-inline-detach
RUNTIME=$(HOME)/llvm-cilk/lib/x86_64/libcilkrts.a
INC=-I../cilkplusrts/include


all: fib_stats #fib_check fib_nocheck fib_stats

fib.o: fib.cpp
	$(CXX) $(CILKFLAGS) $(CXXFLAGS) -std=c++11 -c fib.cpp -o fib.o

fibrd.o: fib.cpp
	$(CXX) $(CILKFLAGS) $(CXXFLAGS) -DRACEDETECT -std=c++11 -c fib.cpp -o fibrd.o

fibrd.stats.o: fib.cpp
	$(CXX) $(CILKFLAGS) $(CXXFLAGS) -DRACEDETECT -DSTATS -std=c++11 -c fib.cpp -o fibrd.stats.o

# om.o: ../../om/om.c
# 	cd ../../om && $(CC) $(CFLAGS) -c om.c && mv om.o ../rdtool/ && cd -

omrd.o: ../omrd.cpp ../../om/om.c
	cd .. && $(CXX) $(CILKFLAGS) $(CXXFLAGS) $(INC) -c omrd.cpp && cd -

omrd.stats.o: ../omrd.cpp ../../om/om.c
	cd .. && $(CXX) $(CILKFLAGS) $(CXXFLAGS) $(INC) -DSTATS -c omrd.cpp -o omrd.stats.o && cd -

libomrd.a: omrd.o #om.o
	cd .. && ar -r libomrd.a omrd.o && cd -

libomrd.stats.a: omrd.stats.o
	cd .. && ar -r libomrd.stats.a omrd.stats.o && cd -

fib_check: fibrd.o libomrd.a
	$(CXX) -ldl -lpthread -Wl,--whole-archive ../libomrd.a -Wl,--no-whole-archive -o fib_check fibrd.o $(RUNTIME) 

fib_nocheck: fib.o
	$(CXX) -ldl -lpthread -o fib_nocheck fib.o $(RUNTIME)

fib_stats: fibrd.stats.o libomrd.stats.a
	$(CXX) -ldl -lpthread -ltcmalloc -Wl,--whole-archive ../libomrd.stats.a -Wl,--no-whole-archive -o fib_stats fibrd.stats.o $(RUNTIME) 

clean:
	rm -rf fib.o fibrd.o fib_nocheck fib_check ../libomrd.a ../omrd.o ../libomrd.stats.a ../omrd.stats.o fibrd.stats.o fib_stats

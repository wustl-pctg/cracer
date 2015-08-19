CXX=clang++
CC=clang
CILKFLAGS=-fcilkplus -fcilktool -fno-inline-detach
RUNTIME=/home/rob/cilkplus/lib/x86_64/libcilkrts.a
INC=-I/home/rob/devel/batch/cilkplusrts/include


all:
	$(CXX) $(CILKFLAGS) -c fib.cpp
	cd ../../om && $(CC) -c om.c && cd -
	cd .. && $(CC) $(CILKFLAGS) $(INC) -c omrd.c && ar -r libomrd.a omrd.o ../om/om.o && cd -
	$(CXX) -ldl -lpthread -Wl,--whole-archive ../libomrd.a -Wl,--no-whole-archive -o fib fib.o $(RUNTIME) 

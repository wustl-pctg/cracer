CC=clang #-flto
CXX=clang++ #-flto
#USE_GOLD=true
COMPILER_HOME=$(HOME)/llvm-cilk
RUNTIME=$(COMPILER_HOME)/lib/x86_64/libcilkrts.a
MALLOC= 
#MALLOC=-ltcmalloc
#MALLOC=-ltbbmalloc_proxy

FLAGS=-O0 -g -fno-omit-frame-pointer #-DNDEBUG
CFLAGS=-std=c99 $(FLAGS)
CXXFLAGS=-std=c++11 $(FLAGS)
CILKFLAGS=-fcilkplus -fno-inline-detach
RDFLAGS=-DRACEDETECT -fcilktool -fsanitize=thread 
ARFLAGS=#--plugin $(COMPILER_HOME)/lib/LLVMgold.so
LDFLAGS=
OMINC=-I../cilkplusrts/include #-I$(COMPILER_HOME)/src/projects/compiler-rt/lib
USERINC=-I$(COMPILER_HOME)/include

TARGETS := test1 test2 test3 test4 test5 test6 test7 test8 test9 \
test10 test11 missing test_bags test_stack

# test_loop test_mem test_mem_list test_stack \
# test_stack_mem test_unalign

.SUFFIXES:

default : $(TARGETS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CILKFLAGS) $(RDFLAGS) $(USERINC) -c $^

%: %.o
	$(CXX) $(LDFLAGS) $^ ../../libomrd.a $(RUNTIME) -ldl -lpthread $(MALLOC) -o $@

ALL_TESTS = $(TARGETS)

clean:
	rm -rf $(TARGETS)

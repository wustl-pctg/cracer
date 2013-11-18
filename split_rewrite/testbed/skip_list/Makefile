CPPSRCS		= main.cpp cpp_framework.cpp ITest.cpp
CSRCS			= dsmain.cilk
TARGET		= test_intel64 
DSTARGET	= test_cilk
DYNTARGET = test_dyn

CPP		= g++
CC		= ../../../prob/support/cilkclocal

CPPFLAGS	= -O0 -m64 -DNDEBUG -DINTEL64 -D_REENTRANT -pthread  
CFLAGS		= -O0 -g -Wall 
LFLAGS		= -O0 -m64 -DNDEBUG -DINTEL64 -D_REENTRANT -pthread
DYNFLAGS	= -O2 -g -Wall 


OBJS		= $(CPPSRCS:.cpp=.o)
COBJS		= $(CSRCS:.cilk=.o)
DYNOBJS = dynmain.o

all: $(TARGET) $(DSTARGET) $(DYNTARGET)

main.o:
	$(CPP) $(CPPFLAGS) -c ./test/main.cpp

dsmain.o:
	$(CC) $(CFLAGS) -c ./test/dsmain.cilk
	mv ./test/dsmain.o dsmain.o	

dynmain.o: ./test/dynmain.cilk
	$(CC) $(CFLAGS) -c ./test/dynmain.cilk
	mv ./test/dynmain.o dynmain.o

cpp_framework.o:
	$(CPP) $(CPPFLAGS) -c ./framework/cpp_framework.cpp

ITest.o:
	$(CPP) $(CPPFLAGS) -c ./data_structures/ITest.cpp

$(TARGET): $(OBJS)
	$(CPP) $(LFLAGS) $(OBJS) -o $(TARGET)

$(DSTARGET): $(COBJS)
	$(CC) $(CLFLAGS) $(COBJS) -o $(DSTARGET)

$(DYNTARGET): $(DYNOBJS)
	$(CC) $(DYNFLAGS) $(DYNOBJS) -o $(DYNTARGET)

clean:
	rm -f $(OBJS) $(COBJS) $(TARGET) $(DSTARGET) $(COBJS) $(DYNTARGET) $(DYNOBJS)

depend:
	mkdep $(SRCS)


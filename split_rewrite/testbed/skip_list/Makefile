CPPSRCS		= main.cpp cpp_framework.cpp ITest.cpp
CSRCS			= dsmain.cilk
TARGET		= test_intel64 
DSTARGET	= test_cilk
DYNTARGET = test_dyn
DYNTARGETNS = test_dyn_ns
SEQTARGET = test_seq

CPP		= g++
CC		= ../../../prob/support/cilkclocal
SEQ   = gcc

CPPFLAGS	= -O2 -m64 -DNDEBUG -DINTEL64 -D_REENTRANT -pthread  
CFLAGS		= -O2 -Wall 
LFLAGS		= -O2 -m64 -DNDEBUG -DINTEL64 -D_REENTRANT -pthread
DYNFLAGS	= -O3 -Wall 
DYNFLAGSNS= -O3 -Wall 
SEQFLAGS  = -O3 -Wall

OBJS		= $(CPPSRCS:.cpp=.o)
COBJS		= $(CSRCS:.cilk=.o)
DYNOBJS = dynmain.o
DYNOBJSNS = dynmainNS.o
SEQOBJS = seqmain.o

all: $(TARGET) $(DSTARGET) $(DYNTARGET) $(DYNTARGETNS) $(SEQTARGET)

main.o:
	$(CPP) $(CPPFLAGS) -c ./test/main.cpp

dsmain.o:
	$(CC) $(CFLAGS) -c ./test/dsmain.cilk
	mv ./test/dsmain.o dsmain.o	

dynmain.o: ./test/dynmain.cilk
	$(CC) $(DYNFLAGS) -c ./test/dynmain.cilk 
	mv ./test/dynmain.o dynmain.o

dynmainNS.o: ./test/dynmainNS.cilk
	$(CC) $(DYNFLAGSNS) -c ./test/dynmainNS.cilk
	mv ./test/dynmainNS.o dynmainNS.o
	

seqmain.o: ./test/seqmain.c
	$(SEQ) $(SEQFLAGS) -c ./test/seqmain.c

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

$(DYNTARGETNS): $(DYNOBJSNS)
	$(CC) $(DYNFLAGSNS) $(DYNOBJSNS) -o $(DYNTARGETNS) 

$(SEQTARGET): $(SEQOBJS)
	$(SEQ) $(SEQFLAGS) $(SEQOBJS) -o $(SEQTARGET)

clean:
	rm -f $(OBJS) $(COBJS) $(TARGET) $(DSTARGET) $(COBJS) $(DYNTARGET) $(DYNOBJS) $(DYNTARGETNS) $(DYNOBJSNS) $(SEQTARGET) $(SEQOBJS)

depend:
	mkdep $(SRCS)


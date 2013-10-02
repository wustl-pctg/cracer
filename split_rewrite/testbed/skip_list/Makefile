CPPSRCS		= main.cpp cpp_framework.cpp ITest.cpp
CSRCS			= dsmain.cilk
TARGET		= test_intel64 
DSTARGET	= test_cilk

CPP		= g++
CC		= ../../../prob/support/cilkclocal

CPPFLAGS	= -O0 -m64 -DNDEBUG -DINTEL64 -D_REENTRANT -lrt -pthread  
CFLAGS		= -O0 -g -Wall 
LFLAGS		= -O0 -m64 -DNDEBUG -DINTEL64 -D_REENTRANT -lrt -pthread
CLFLAGS		= -O0 -g -Wall 


OBJS		= $(CPPSRCS:.cpp=.o)
COBJS		= $(CSRCS:.cilk=.o)

all: $(TARGET) $(DSTARGET)

main.o:
	$(CPP) $(CPPFLAGS) -c ./test/main.cpp

dsmain.o:
	$(CC) $(CFLAGS) -c ./test/dsmain.cilk
	mv ./test/dsmain.o dsmain.o	

cpp_framework.o:
	$(CPP) $(CPPFLAGS) -c ./framework/cpp_framework.cpp

ITest.o:
	$(CPP) $(CPPFLAGS) -c ./data_structures/ITest.cpp

$(TARGET): $(OBJS)
	$(CPP) $(LFLAGS) $(OBJS) -o $(TARGET)

$(DSTARGET): $(COBJS)
	$(CC) $(CLFLAGS) $(COBJS) -o $(DSTARGET)

clean:
	rm -f $(OBJS) $(COBJS) $(TARGET) $(DSTARGET)

depend:
	mkdep $(SRCS)


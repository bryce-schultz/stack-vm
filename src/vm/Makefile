CC=g++
CPPFLAGS=-g -Wall -Wextra -pedantic -std=c++11

OBJS=main.o \
	 SVASMParser.o \
	 SVMException.o \
	 SimpleVirtualMachine.o

.PHONY: all clean depends

all: vm

SVASMParser.o: SVASMParser.cpp SVASMParser.h Program.h \
 SimpleVirtualMachine.h SVMException.h
SVMException.o: SVMException.cpp SVMException.h
SimpleVirtualMachine.o: SimpleVirtualMachine.cpp SVMException.h \
 SimpleVirtualMachine.h SVASMParser.h Program.h
main.o: main.cpp SimpleVirtualMachine.h SVMException.h SVASMParser.h \
 Program.h

vm: $(OBJS)
	$(CC) $(CPPFLAGS) -o vm $(OBJS)

.cpp.o:
	$(CC) $(CPPFLAGS) -c $<

depends:
	$(CC) -MM *.cpp 1>&2

clean:
	rm -f vm $(OBJS)
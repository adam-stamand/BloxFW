CC = g++
CCFLAGS = -std=c++11 -Wall -I $(PWD)

all: debug.o

debug.o : debug.cpp debug.h
	$(CC) $(CCFLAGS) -c debug.cpp


clean :
	rm debug.o

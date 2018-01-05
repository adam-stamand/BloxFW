CC = g++
CCFLAGS = -std=c++11 -Wall -I $(PWD)
DEPS = ../data_structures/labeled_box.h ../data_structures/box.h ../tools/debug.h

all: container.o component.o

container.o : container.cpp container.h  containermanager.h $(DEPS)
	$(CC) $(CCFLAGS) -c container.cpp

component.o : component.cpp component.h  containermanager.h $(DEPS)
	$(CC) $(CCFLAGS) -c component.cpp

clean :
	rm container.o component.o

CC = g++
CCFLAGS = -std=c++11 -Wall

framework : main.o entitymanager.o entity.o component.o debug.o
	$(CC) $(CCFLAGS) -g -o framework main.o entitymanager.o entity.o component.o debug.o

main.o : main.cpp debug.h
	$(CC) $(CCFLAGS) -c main.cpp

entitymanager.o : entitymanager.cpp entitymanager.h container.h debug.h
	$(CC) $(CCFLAGS) -c entitymanager.cpp

entity.o : entity.cpp entity.h container.h debug.h
	$(CC) $(CCFLAGS) -c entity.cpp

component.o : component.cpp component.h debug.h
	$(CC) $(CCFLAGS) -c component.cpp

debug.o : debug.cpp debug.h
	$(CC) $(CCFLAGS) -c debug.cpp

clean :
	rm framework main.o entitymanager.o entity.o component.o

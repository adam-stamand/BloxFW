CC = g++
CCFLAGS = -std=c++11 -Wall

framework : main.o entitymanager.o entity.o component.o
	$(CC) $(CCFLAGS) -g -o framework main.o entitymanager.o entity.o component.o

main.o : main.cpp
	$(CC) $(CCFLAGS) -c main.cpp

entitymanager.o : entitymanager.cpp entitymanager.h container.h
	$(CC) $(CCFLAGS) -c entitymanager.cpp

entity.o : entity.cpp entity.h container.h
	$(CC) $(CCFLAGS) -c entity.cpp

component.o : component.cpp component.h
	$(CC) $(CCFLAGS) -c component.cpp

clean :
	rm framework main.o entitymanager.o entity.o component.o

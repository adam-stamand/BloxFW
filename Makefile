
CC = g++
CCFLAGS = -std=c++11 -Wall -I $(PWD)
src = $(wildcard *.cpp) \
			 $(wildcard data_structures/*.cpp) \
			 $(wildcard core/*.cpp) \
			 $(wildcard tools/*.cpp)
obj = $(src:.cpp=.o)

myprog: $(obj)
	$(CC) $(CCFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) myprog

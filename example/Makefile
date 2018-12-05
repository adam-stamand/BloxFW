INC=-I../
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g $(INC)

src = $(wildcard ../BloxFW/tools/*.cpp) \
	$(wildcard *.cpp) \
	$(wildcard ../BloxFW/utils/*.cpp) \
	$(wildcard ../BloxFW/core/*.cpp)
obj = $(src:.cpp=.o)

Blox: $(obj)
	$(CXX) $(CXXFLAGS)  -o $@ $^


clean:
	rm -f $(obj) Blox

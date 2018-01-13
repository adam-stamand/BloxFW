
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g

src = $(wildcard ../tools/*.cpp) \
	$(wildcard *.cpp) \
	$(wildcard ../utils/*.cpp) \
	$(wildcard ../core/*.cpp)
obj = $(src:.cpp=.o)

Blox: $(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^


clean:
	rm -f $(obj) Blox

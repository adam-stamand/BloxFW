
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g

src = $(wildcard tools/*.cpp) \
	$(wildcard *.cpp) \
	$(wildcard utils/*.cpp) \
	$(wildcard core/*.cpp) 
obj = $(src:.cpp=.o)

myprog: $(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^


clean:
	rm -f $(obj) myprog


CXX = g++
CXXFLAGS = -Wall -std=c++11 $(DEBUG)

src = $(wildcard *.cpp) \
			 $(wildcard data_structures/*.cpp) \
			 $(wildcard core/*.cpp) \
			 $(wildcard tools/*.cpp)
obj = $(src:.cpp=.o)

myprog: $(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^


clean:
	rm -f $(obj) myprog

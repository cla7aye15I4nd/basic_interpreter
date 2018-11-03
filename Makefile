CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2 

code: basic.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)
clean:
	rm code -f

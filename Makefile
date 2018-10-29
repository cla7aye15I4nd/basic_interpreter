CXX = g++
CXXFLAGS = -Wall -O2 

code: code.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm code -f

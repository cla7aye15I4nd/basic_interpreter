CXX = g++
CXXFLAGS = -Wall -O2 

score: basic
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm code -f

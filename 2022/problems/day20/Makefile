CC = g++
CXX = g++
CPPFLAGS = -Wall -g -O2 -std=gnu++20
LDLIBS = -lm

EXE = day20 day20_2 day20s day20_2s

all: $(EXE)

%: %.o
%.o: %.cpp

day20s: day20.cpp
	$(CXX) $(CPPFLAGS) -o day20s day20.cpp -DSLOW

day20_2s: day20_2.cpp
	$(CXX) $(CPPFLAGS) -o day20_2s day20_2.cpp -DSLOW

clean:
	rm -f *.o
	rm -f $(EXE)

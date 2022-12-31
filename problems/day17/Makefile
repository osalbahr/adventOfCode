CC = g++
CXX = g++
CPPFLAGS = -Wall -g -O2 -std=gnu++20
LDLIBS = -lm

EXE = day17 day17_2 day17_2chop

all: $(EXE)

%: %.o
%.o: %.cpp

day17_2chop: day17_2.cpp
	$(CXX) $(CPPFLAGS) day17_2.cpp -o day17_2chop -DCHOP



clean:
	rm -f $(EXE)

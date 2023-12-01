CXX = g++
CPPFLAGS = -Wall -g -O2 -std=gnu++20
LDLIBS = -lm

EXE = day15 day15_2

all: $(EXE)

%: %.o
%.o: %.cpp

clean:
	rm -f $(EXE)

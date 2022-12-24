CC = g++
CXX = g++
CPPFLAGS = -Wall -g -O2 -std=gnu++20
LDLIBS = -lm

EXE = day24 day24_2

all: $(EXE)

%: %.o
%.o: %.cpp

clean:
	rm -f $(EXE)

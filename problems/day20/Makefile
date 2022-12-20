CC = g++
CXX = g++
CPPFLAGS = -Wall -g -std=gnu++20
LDLIBS = -lm

EXE = day20 day20_2

all: $(EXE)

%: %.o
%.o: %.cpp

clean:
	rm -f *.o
	rm -f $(EXE)

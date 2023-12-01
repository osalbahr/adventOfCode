CXXFLAGS = -Wall -g -O2 -std=gnu++20
LDLIBS = -lm

EXE = generatex

all: $(EXE)

%: %.o
%.o: %.cpp

clean:
	rm -f $(EXE)

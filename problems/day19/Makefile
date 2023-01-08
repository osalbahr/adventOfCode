CXXFLAGS = -Wall -g -O2 -std=gnu++20
LDLIBS = -lm

EXE = day19 day19_2

all: $(EXE)

%: %.o
%.o: %.cpp

clean:
	rm -f $(EXE)

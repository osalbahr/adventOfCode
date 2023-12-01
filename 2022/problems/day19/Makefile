CXXFLAGS = -Wall -g -O2 -std=gnu++20
LDLIBS = -lm

EXE = day19 day19d day19_2

all: $(EXE)

%: %.o
%.o: %.cpp

day19d: day19.cpp
	$(CXX) $(CXXFLAGS) -DDEBUG day19.cpp -o day19d
clean:
	rm -f $(EXE)

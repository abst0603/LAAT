CC = /usr/bin/g++
CFLAGS = -std=c++14 -Wall -O2 -fopenmp
SRC = $(wildcard src/*.cpp) $(wildcard src/datapoint/*.cpp)
OBJECTS = $(patsubst src/%.cpp, bin/%.o, $(SRC))
dir_guard=@mkdir -p $(@D)

all: clearscreen LAAT

LAAT: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

bin/%.o: src/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

clearscreen:
	reset

clean:
	rm -f LAAT -r bin

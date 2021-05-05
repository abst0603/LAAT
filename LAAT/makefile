CC = /usr/bin/g++
CFLAGS = -std=c++14 -Wall -O2
SRC = $(wildcard src/*.cpp) $(wildcard src/datapoint/*.cpp)
OBJECTS = $(patsubst src/%.cpp, bin/%.o, $(SRC))

all: clearscreen antPCA

antPCA: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

bin/%.o: src/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clearscreen:
	reset

clean:
	rm -f antPCA bin/*.o bin/datapoint/*.o

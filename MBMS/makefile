CC = /usr/bin/g++
CFLAGS = -std=c++14 -Wall -O2 -iquote ../ext/eigen/ -iquote ../ext/
SRC = $(wildcard src/*.cpp) $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp, bin/%.o, $(SRC))
dir_guard=@mkdir -p $(@D)

all: clearscreen MBMS

MBMS: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

bin/%.o: src/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

clearscreen:
	reset

clean:
	rm -f -r MBMS bin/

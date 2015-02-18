CXX=clang++ #comment this if you wish to use the default
CFLAGS=-c -g -Wall -std=c++11
LDFLAGS=
SOURCES=$(shell find src -name "*.cpp")
OBJECTS:=$(SOURCES:src/%.cpp=build/%.o)
EXECUTABLE=dragoon

all: $(SOURCES) dist/$(EXECUTABLE)

dist/$(EXECUTABLE): $(OBJECTS)
	@mkdir -p dist
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS)

rmdist:
	rm -rf dist/*

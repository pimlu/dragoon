 #comment this if you wish to use the default
CXX=clang++
OPT=
CFLAGS=$(OPT) -x c++ -std=c++11 -c -g -Wall -Wno-deprecated-register
CXXFLAGS=$(OPT) -c -g -Wall -std=c++11
LDFLAGS=

PARSE=src/parse/dragoon
PARSEO=build/parse/flex.o build/parse/bison.o

SOURCES=$(shell find src -name "*.cpp")
OBJECTS:=$(SOURCES:src/%.cpp=build/%.o)
EXECUTABLE=dragoon



all: $(SOURCES) dist/$(EXECUTABLE)

dist/$(EXECUTABLE): $(OBJECTS) $(PARSEO)
	@mkdir -p dist
	$(CXX) $(OBJECTS) $(PARSEO) $(LDFLAGS) -o $@

build:
	find src/* -type d -print0 | sed 's/src/build/g' | xargs -0 mkdir -p

build/%.o: src/%.cpp $(PARSE).tab.c $(PARSE).tab.h | build
	$(CXX) $(CXXFLAGS) $< -o $@

build/parse/bison.o: $(PARSE).tab.c | build
	$(CXX) $(CFLAGS) $< -o $@

$(PARSE).tab.c $(PARSE).tab.h: $(PARSE).y | build
	bison --debug --verbose -d $< -o $(PARSE).tab.c

build/parse/flex.o: $(PARSE).yy.c $(PARSE).tab.c $(PARSE).tab.h
	$(CXX) $(CFLAGS) $< -o $@

$(PARSE).yy.c: $(PARSE).l
	flex -o$@ $<

clean: FORCE
	rm -rf build $(PARSE).tab.c $(PARSE).tab.h $(PARSE).yy.c

rmdist: FORCE
	rm -rf dist/*

FORCE:

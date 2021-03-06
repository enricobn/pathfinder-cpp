VPATH=src bin

#SAVE-TEMPS = -save-temps
SAVE-TEMPS =

#PROFILER = -pg
PROFILER =

#DEBUG = -g
DEBUG =

CC = g++

CFLAGS = -Wall -std=c++0x -c $(DEBUG) -O3 $(SAVE-TEMPS) $(PROFILER)
LFLAGS = -Wall $(DEBUG) -o bin/$@ $(PROFILER)
GL = -lglut -lGLU -lGL

all : configure field_test.bin move_example.bin List_test.bin

Shape.cpp : Shape.hpp

Field.cpp : Field.hpp

List_test.bin : obj/List_test.o
	@echo 'Building $@'
	$(CC) $(LFLAGS) $^

move_example.bin : obj/move_example.o obj/astar_pathfinder.o obj/Field.o obj/PathNode.o obj/NodesMap.o
	@echo 'Building $@'
	$(CC) $(LFLAGS) $^ $(GL)

field_test.bin : obj/field_test.o obj/Field.o obj/PathNode.o obj/NodesMap.o
	@echo 'Building $@ with $^'
	$(CC) $(LFLAGS) $^ $(GL)

obj/%.o : %.cpp
	@echo 'Compiling $<'
	$(CC) $(CFLAGS) $< -o $@

.PHONY : clean configure

clean :
	@rm -f *.bin *.o *.s *.ii *.out bin/* obj/*

configure :
	@mkdir -p bin
	@mkdir -p obj
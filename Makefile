VPATH=src bin

#SAVE-TEMPS = -save-temps
SAVE-TEMPS =
#PROFILER = -pg
PROFILER =
CC = g++
#DEBUG = -g
DEBUG =

CFLAGS = -Wall -c $(DEBUG) -O3 -pedantic $(SAVE-TEMPS) $(PROFILER)
LFLAGS = -Wall $(DEBUG) -o bin/$@ $(PROFILER)
GL = -lglut -lGLU

all : configure field_test.bin move_example.bin container_test.bin List_test.bin

List_test.bin : obj/List_test.o
	@echo 'Building $@'
	$(CC) $(LFLAGS) $^

container_test.bin : obj/container_test.o
	@echo 'Building $@'
	$(CC) $(LFLAGS) $^

move_example.bin : obj/move_example.o obj/astar_pathfinder.o obj/Field.o obj/PathNode.o
	@echo 'Building $@'
	$(CC) $(LFLAGS) $(GL) $^

field_test.bin : obj/field_test.o obj/Field.o obj/PathNode.o
	@echo 'Building $@ with $^'
	$(CC) $(LFLAGS) $^

obj/%.o : %.cpp
	@echo 'Compiling $<'
	$(CC) $(CFLAGS) $< -o $@

.PHONY : clean configure

clean :
	@rm -f *.bin *.o *.s *.ii *.out bin/* obj/*

configure :
	@mkdir -p bin
	@mkdir -p obj
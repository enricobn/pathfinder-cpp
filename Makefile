VPATH=src bin

#SAVE-TEMPS = -save-temps
SAVE-TEMPS =
#PROFILER = -pg
PROFILER =
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -O2 -pedantic $(SAVE-TEMPS) $(PROFILER)
LFLAGS = -Wall $(DEBUG) -o bin/$@ $(PROFILER)
GL = -lglut -lGLU

all : configure field_test.bin move_example.bin container_test.bin

container_test.bin : obj/container_test.o
	@echo 'Building $@'
	$(CC) $(LFLAGS) $^

move_example.bin : obj/move_example.o obj/astar_pathfinder.o obj/Field.o obj/List.o
	@echo 'Building $@'
	$(CC) $(LFLAGS) $(GL) $^

field_test.bin : obj/field_test.o obj/Field.o 
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
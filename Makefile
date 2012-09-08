#CPPFLAGS = -lglut -lGLU
all : field_test.bin move_example.bin container_test.bin

container_test.bin : container_test.o
	@echo 'Building $@'
	g++ -g -lglut -lGLU $^ -o $@

move_example.bin : move_example.o astar_pathfinder.o field.o
	@echo 'Building $@'
	g++ -g -lglut -lGLU $^ -o $@

field_test.bin : field_test.o field.o 
	@echo 'Building $@'
	g++ -g $^ -o $@

%.o : %.cc
	@echo 'Compiling $<'
	g++ -g -O2 -pedantic -save-temps -Wall -c $<

.PHONY : clean

clean :
	rm -f *.bin; rm -f *.o; rm -f *.i; rm -f *.s;

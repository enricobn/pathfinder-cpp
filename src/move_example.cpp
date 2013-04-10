/*
 * move_example.c
 * This is a path finding example using ASearch
 */
#include <unistd.h>
#include <GL/glut.h>
#include "globals.hpp"
#include "Field.hpp"
#include "astar_pathfinder.hpp"

const int WIDTH = 100;
const int HEIGHT = 100;
static const int moving_shapes_count = 50;

dimension_t dimension = dimension_t(WIDTH, HEIGHT);

CField field(dimension);

void rectangle_draw(const shape_t& rectangle) {
    glRectf((float)rectangle.point.x, (float)rectangle.point.y, (float)(rectangle.point.x + rectangle.dimension.width), 
        (float)(rectangle.point.y + rectangle.dimension.height));
}

typedef struct {
    shape_t *shape;
    point_t end;
} moving_shape_t;

moving_shape_t *moving_shapes = NULL;

shape_t *rectangle_new(int x, int y, int width, int height, void (*draw)(const shape_t& shape)) {
    shape_t *r = (shape_t *) malloc(sizeof(shape_t));
    r->point.x = x;
    r->point.y = y;
    r->dimension.width = width;
    r->dimension.height = height;
    r->draw = draw;
    return r;
}

void white_draw(const shape_t& rectangle) {
    glColor3f(1.0, 1.0, 1.0);
    rectangle_draw(rectangle);
}

void blue_draw(const shape_t& rectangle) {
    glColor3f(0.0, 0.0, 1.0);
    rectangle_draw(rectangle);
}

void red_draw(const shape_t& rectangle) {
    glColor3f(1.0, 0.0, 0.0);
    rectangle_draw(rectangle);
}

void green_draw(const shape_t& rectangle) {
    glColor3f(0.0, 1.0, 0.0);
    rectangle_draw(rectangle);
}

void field_init() {
//	dimension_t d;
//	d = {WIDTH, HEIGHT};
//    field = CField(d);

    field.add(rectangle_new(10, 10, 10, 10, white_draw));

    field.add(rectangle_new(40, 20, 20, 20, white_draw));
    
    field.add(rectangle_new(40, 60, 20, 20, white_draw));

    field.add(rectangle_new(75, 75, 10, 10, white_draw));
    
    moving_shapes = (moving_shape_t *) malloc(sizeof(moving_shape_t) * moving_shapes_count * 2);    
    
    int i;
    for (i = 0; i < moving_shapes_count; i++) {
        shape_t *p1 = rectangle_new(0, moving_shapes_count -i, 1, 1, red_draw);
        moving_shapes[2 * i].shape = p1;
        moving_shapes[2 * i].end.x = 90;
        moving_shapes[2 * i].end.y = 99 -i;
        field.add(p1);

        shape_t *p2 = rectangle_new(90, 99 -i, 1, 1, blue_draw);
        moving_shapes[2 * i +1].shape = p2;
        moving_shapes[2 * i + 1].end.x = 0;
        moving_shapes[2 * i + 1].end.y = moving_shapes_count -i;
        field.add(p2);
    }
}

void display(void)
{
//    printf("display start\n");
/* clear all pixels  */
    glClear (GL_COLOR_BUFFER_BIT);

    vector<const shape_t *> *shapes = field.get_shapes();

    for (unsigned int i = 0; i < shapes->size(); i++) {
        const shape_t *s = (*shapes)[i];
        glColor3f (1.0, 1.0, 1.0);
        s->draw(*s);
    }

/*   
    {
        int i;
        for (i = 0; i < moving_shapes_count * 2; i++) {
            glColor3f(0.0, 1.0, 0.0);
            moving_shapes[i].shape->draw(*moving_shapes[i].shape);
        }
    }
*/

/*    list_free(shapes, FALSE);*/
    
/* don't wait!  
 * start processing buffered OpenGL routines 
 */
   glFlush ();
/*   usleep(100);*/
   glutSwapBuffers();
//   printf("display end\n");
}

void animate() {
//    printf("animate start\n");
    for (int i = 0; i < moving_shapes_count * 2; i++) {
//        printf("moving_shape %d ", i);
//        moving_shapes[i].shape->print();
        if (moving_shapes[i].shape->point == moving_shapes[i].end) {
            continue;
        }
        point_t *point = get_next_to_path(field, moving_shapes[i].shape->point, moving_shapes[i].end);
        if (point == NULL) {
/*            printf("empty path\n");*/
            continue;
/*            ERROR("No path found!\n");
            exit(1);
*/
        }
        
        moving_shapes[i].shape->point.x = point->x;
        moving_shapes[i].shape->point.y = point->y;
        free(point);
    }


/* 
    Normally openGL doesn't continuously draw frames. It puts one in place and waits for you to tell him what to do next.
     Calling glutPostRedisplay() forces a redraw with the new angle
 */
//    printf("before redisplay\n");
    glutPostRedisplay();
//    printf("animate end\n");
}

void init (void) 
{
/* select clearing color 	*/
    glClearColor (0.0, 0.0, 0.0, 0.0);

/* initialize viewing values  */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, WIDTH, HEIGHT, 0, 0, 1.0);
}

/* 
 * Declare initial window size, position, and display mode
 * (single buffer and RGBA).  Open window with "hello"
 * in its title bar.  Call initialization routines.
 * Register callback function to display graphics.
 * Enter main loop and process events.
 */
int main(int argc, char** argv)
{
    field_init();
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (800, 800); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Path example");
    init ();
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutMainLoop();
    return 0;   /* ANSI C requires main to return int. */
}


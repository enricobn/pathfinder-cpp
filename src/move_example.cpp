/*
 * move_example.c
 * This is a path finding example using ASearch
 * 
 * Intel(R) Core(TM) i7 CPU       M 620  @ 2.67GHz
 * Completed in 6.027212 secs.
 */
#include <unistd.h>
#include <GL/glut.h>
#include "globals.hpp"
#include "Field.hpp"
#include "astar_pathfinder.hpp"
using namespace std;

const int WIDTH = 100;
const int HEIGHT = 100;
static const int moving_shapes_count = 50;

dimension_t dimension = dimension_t(WIDTH, HEIGHT);

//ComposedField field(dimension, 4);
StandardField field(dimension);
//void rectangle_draw(const shape_t& rectangle) {
//    glRectf((float)rectangle.getPoint()->x, (float)rectangle.getPoint()->y, (float)(rectangle.getPoint()->x + rectangle.dimension.width),
//        (float)(rectangle.getPoint()->y + rectangle.dimension.height));
//}

typedef struct {
    Shape *shape;
    point_t end;
} moving_shape_t;

moving_shape_t *moving_shapes = NULL;
bool ended = false;
clock_t *start_time = NULL;

//shape_t *rectangle_new(int x, int y, int width, int height, void (*draw)(const shape_t& shape)) {
////    shape_t *r = (shape_t *) malloc(sizeof(shape_t));
//	shape_t* r = new shape_t({x,y}, {width, height});
////    r->getPoint().x = x;
////    r->getPoint().y = y;
////    r->dimension.width = width;
////    r->dimension.height = height;
//    r->draw = draw;
//    return r;
//}
//
//void white_draw(const shape_t& rectangle) {
//    glColor3f(1.0, 1.0, 1.0);
//    rectangle_draw(rectangle);
//}
//
//void blue_draw(const shape_t& rectangle) {
//    glColor3f(0.0, 0.0, 1.0);
//    rectangle_draw(rectangle);
//}
//
//void red_draw(const shape_t& rectangle) {
//    glColor3f(1.0, 0.0, 0.0);
//    rectangle_draw(rectangle);
//}
//
//void green_draw(const shape_t& rectangle) {
//    glColor3f(0.0, 1.0, 0.0);
//    rectangle_draw(rectangle);
//}

void field_init() {
//	dimension_t d;
//	d = {WIDTH, HEIGHT};
//    field = CField(d);

    field.add(new Rectangle({10, 10}, {10, 10}, 1.0, 1.0, 1.0));

    field.add(new Rectangle({40, 20}, {20, 20}, 1.0, 1.0, 1.0));
    
    field.add(new Rectangle({40, 60}, {20, 20}, 1.0, 1.0, 1.0));

    field.add(new Rectangle({75, 75}, {10, 10}, 1.0, 1.0, 1.0));
    
    moving_shapes = (moving_shape_t *) malloc(sizeof(moving_shape_t) * moving_shapes_count * 2);    
    
    int i;
    for (i = 0; i < moving_shapes_count; i++) {
        Shape *p1 = new Rectangle({0, moving_shapes_count -i}, {1, 1}, 1.0, 0.0, 0.0);
        moving_shapes[2 * i].shape = p1;
        moving_shapes[2 * i].end.x = 90;
        moving_shapes[2 * i].end.y = 99 -i;
        field.add(p1);

        Shape *p2 = new Rectangle({90, 99 -i}, {1, 1}, 0.0, 0.0, 1.0);
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

    vector<Shape *> shapes = field.get_shapes();

    for(std::vector<Shape *>::iterator it = shapes.begin(); it != shapes.end(); ++it) {
//
//    for (unsigned int i = 0; i < shapes.size(); i++) {
//        Shape *s = shapes[i];
        glColor3f (1.0, 1.0, 1.0);
        (*it)->draw();
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

    bool completed = true;

    for (int i = 0; i < moving_shapes_count * 2; i++) {
//        printf("moving_shape %d ", i);
//        moving_shapes[i].shape->print();
        auto from = moving_shapes[i].shape->getPoint();
        auto to = moving_shapes[i].end;

        if (from != to) {
            completed = false;
        }
        
        if (moving_shapes[i].shape->getPoint() == to) {
            continue;
        }

        point_t *point = get_next_to_path(field, from, to);

        if (point == NULL) {
            printf("empty path\n");
            exit(1);
            continue;
        }
        
//        moving_shapes[i].shape->point.x = point->x;
//        moving_shapes[i].shape->point.y = point->y;

        moving_shapes[i].shape->setPoint(*point);
        free(point);
    }

    if (completed && !ended) {
        auto time = clock();
        double elapsed_secs = double(time - *start_time) / CLOCKS_PER_SEC;
        printf("Completed in %f secs.\n", elapsed_secs);
        ended = true;
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
    auto time = clock();
    start_time = &time;

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


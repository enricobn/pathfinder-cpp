#include <assert.h>
#include "Field.hpp"

int main(int argc, char** argv) {
    dimension_t dimension = dimension_t(100, 100);
    CField field = CField::CField(dimension, 4);

    /* test contains */ 

    point_t p1 = {50, 50};
    assert(field.contains(p1)); 

    point_t p2 = {150, 150};
    assert(!field.contains(p2)); 

    point_t p3 = {-1, -1};
    assert(!field.contains(p3));

    point_t p4 = {100, 100};
    assert(!field.contains(p4));

    /* test is_occupied */ 
    
    shape_t r1 = {{0, 0}, {10, 10}};
    field.add(&r1);

    shape_t r2 = {{20, 20}, {10, 10}};
    field.add(&r2);
    
    point_t p5 = {5, 5};
    assert(field.is_occupied(p5));
    
    point_t p6 = {15, 15};
    assert(!field.is_occupied(p6));
    
    return 0;
}

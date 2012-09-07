#include <vector>
using namespace std;

#ifndef FIELD_H_
#define FIELD_H_

typedef struct {
    int x;
    int y;
} point_t;

typedef struct {
    int width;
    int height;
} dimension_t;

struct shape_s {
    point_t point;
    dimension_t dimension;
    void (*draw)(struct shape_s shape);
};

typedef struct shape_s shape_t;

int point_equals(point_t a, point_t b);

class CField {
    private:
        vector<shape_t> _shapes;
        dimension_t _dimension;
    public:
        CField(dimension_t dimension);
        
        int is_occupied(point_t point);
        
        int contains(point_t point);
        
        void add(shape_t shape);
        
        vector<shape_t> get_shapes();
};

#endif
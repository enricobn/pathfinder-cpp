#include <stdio.h>
#include <vector>
using namespace std;

#ifndef FIELD_H_
#define FIELD_H_

struct point_t_s {
    int x;
    int y;
    inline
    bool operator<(const point_t_s &other) const {
    	if (x == other.x) {
    		return y < other.y;
    	} else {
    		return x < other.x;
    	}
    }
};

typedef struct point_t_s point_t;

typedef struct {
    int width;
    int height;
} dimension_t;

struct shape_s {
    point_t point;
    dimension_t dimension;
    void (*draw)(struct shape_s shape);
    void print() {
        printf("shape(%d, %d)\n", point.x, point.y); 
    };
};

typedef struct shape_s shape_t;

int point_equals(const point_t& a, const point_t& b);

class CField {
    private:
        vector<shape_t *> _shapes;
        dimension_t _dimension;
    public:
        CField(dimension_t dimension);
        
        int is_occupied(point_t& point);
        
        int contains(point_t& point);
        
        void add(shape_t *shape);
        
        vector<shape_t *> *get_shapes();
};

#endif

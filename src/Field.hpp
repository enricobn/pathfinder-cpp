#include <stdio.h>
#include <vector>
using namespace std;

#ifndef FIELD_H_
#define FIELD_H_

struct point_t_s {
    int x;
    int y;

    inline bool operator<(const point_t_s &other) const {
    	if (x == other.x) {
    		return y < other.y;
    	} else {
    		return x < other.x;
    	}
    }

    bool operator==(const point_t_s &other) const {
    	return x == other.x && y == other.y;
    }

    void print() {
        printf("(%d, %d)", x, y);
    }
};

typedef struct point_t_s point_t;

typedef struct {
    int width;
    int height;
    void print() {
        printf("(%d, %d)", width, height);
    }
} dimension_t;

struct shape_t;

struct shape_t {
    point_t point;
    dimension_t dimension;
    void (*draw)(struct shape_t shape);
    void print() {
        printf("shape p");
        point.print();
        printf(" d");
        dimension.print();
    };
};

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

#include <stdio.h>
#include <vector>
using namespace std;

#ifndef FIELD_H_
#define FIELD_H_

struct point_t;

struct point_t {
    int x;
    int y;

    inline bool operator<(const point_t &other) const {
    	if (x == other.x) {
    		return y < other.y;
    	} else {
    		return x < other.x;
    	}
    }

    inline bool operator==(const point_t &other) const {
    	return x == other.x && y == other.y;
    }

    void print() {
        printf("(%d, %d)", x, y);
    }
};

struct dimension_t {
    int width;
    int height;

    dimension_t(const int width, const int height) {
    	this->width = width;
    	this->height = height;
    }

    void operator=(dimension_t& d) {
    	this->width = d.width;
    	this->height = d.height;
    }

    void print() {
        printf("(%d, %d)", width, height);
    }
};

struct shape_t;

struct shape_t {
    point_t point;
    dimension_t dimension;

    void (*draw)(const shape_t&);

    void print() {
        printf("shape p");
        point.print();
        printf(" d");
        dimension.print();
    };
};

class CField {
    private:
        vector<const shape_t *> _shapes;
        const dimension_t _dimension;
    public:
        CField(const dimension_t&);
        
        int is_occupied(const point_t&);
        
        int contains(const point_t&);
        
        void add(const shape_t *);
        
        vector<const shape_t *> *get_shapes();
};

inline int CField::contains(const point_t& point) {
    return (point.x >= 0 && point.y >= 0
        && point.x < _dimension.width
        && point.y < _dimension.height);
}


#endif

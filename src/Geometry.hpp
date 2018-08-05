/*
 * Geometry.hpp
 *
 *  Created on: Jul 27, 2013
 *      Author: enrico
 */

#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

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

    inline bool operator!=(const point_t &other) const {
    	return !(*this == other);
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

    void print() const {
        printf("(%d, %d)", width, height);
    }
};

#endif /* GEOMETRY_HPP_ */

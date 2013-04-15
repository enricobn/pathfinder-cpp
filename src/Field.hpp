#include <stdio.h>
#include <vector>
#include <unordered_map>
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

struct point_t_hash {
	inline size_t operator()(const point_t& point) const {
		return hash<int>()(point.x * 1000 + point.y);
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

struct shape_t;

class ShapeContainer {

public:
    virtual int is_occupied(const point_t&) const = 0;

    virtual int contains(const point_t&) const = 0;

    virtual void add(shape_t *) = 0;

    virtual vector<shape_t *> *get_shapes() = 0;

    virtual void shapeBeforeMove(shape_t&, const point_t&) = 0;

    virtual ~ShapeContainer() {}

};

struct shape_t {
private:
    point_t _point;
public:
    const dimension_t dimension;

	ShapeContainer* _shapeContainer;

    shape_t(const point_t& point, const dimension_t& dimension) :
    	_point(point), dimension(dimension)
    {
    	_shapeContainer = 0;
    	draw = 0;
    }

    void (*draw)(const shape_t&);

    const point_t* getPoint() const {
    	return &_point;
    }

    void setPoint(const point_t& point) {
    	if (_shapeContainer != 0) {
    		_shapeContainer->shapeBeforeMove(*this, point);
    	}
    	_point = point;
    }

    void print() {
        printf("shape p");
        _point.print();
        printf(" d");
        dimension.print();
    };
};

class SubField : public ShapeContainer {
    private:
        vector<shape_t *> _shapes;
        const point_t _point;
        const dimension_t _dimension;
    public:
        SubField(const point_t&, const dimension_t&);

        int is_occupied(const point_t&) const;

        int contains(const point_t&) const;

        int contains(const int, const int) const;

        int contains(const point_t& point, const dimension_t& dimension) const;

        int containsEntirely(const point_t& point, const dimension_t& dimension) const;

        void remove(const shape_t*);

        void add(shape_t *);

        vector<shape_t *> *get_shapes();

        void shapeBeforeMove(shape_t&, const point_t&);

        virtual ~SubField();
};

inline int SubField::contains(const int x, const int y) const {
	return x >= _point.x
			&& y >= _point.y
			&& x < (_point.x + _dimension.width)
			&& y < (_point.y + _dimension.height);
}

inline int SubField::contains(const point_t& point) const {
	return point.x >= _point.x
			&& point.y >= _point.y
			&& point.x < (_point.x + _dimension.width)
			&& point.y < (_point.y + _dimension.height);
}

// StandardField

class StandardField : public ShapeContainer {
    private:
        vector<shape_t *> _shapes;
        const dimension_t _dimension;
    public:
        StandardField(const dimension_t&);

        int is_occupied(const point_t&) const;

        int contains(const point_t&) const;

        void add(shape_t *);

        vector<shape_t *> *get_shapes();

        void shapeBeforeMove(shape_t&, const point_t&);

        virtual ~StandardField();
};

inline int StandardField::contains(const point_t& point) const {
    return point.x >= 0
    		&& point.y >= 0
    		&& point.x < _dimension.width
    		&& point.y < _dimension.height;
}

// ComposedField

class ComposedField : public ShapeContainer {
    private:
        vector<shape_t *> _shapes;
        const dimension_t _dimension;
        vector<SubField*> _subFields;
    public:
        ComposedField(const dimension_t&, const int);

        int is_occupied(const point_t&) const;

        int contains(const point_t&) const;

        void add(shape_t *);

        vector<shape_t *> *get_shapes();

        void shapeBeforeMove(shape_t&, const point_t&);

        virtual ~ComposedField();
};

inline int ComposedField::contains(const point_t& point) const {
    return point.x >= 0
    		&& point.y >= 0
    		&& point.x < _dimension.width
    		&& point.y < _dimension.height;
}

#endif

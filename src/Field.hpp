#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <GL/gl.h>
#include <GL/glut.h>
#include "Geometry.hpp"
#include "globals.hpp"
using namespace std;

#ifndef FIELD_H_
#define FIELD_H_

struct point_t_hash {
	inline size_t operator()(const point_t& point) const {
		return hash<int>()(point.x * 1000 + point.y);
	}
};

class ShapeContainer;

class Shape {
protected:
	ShapeContainer *_container;
	point_t _point;
public:
	Shape(point_t point);

	virtual ~Shape();

	void setContainer(ShapeContainer *container);

	point_t getPoint() const;

	void setPoint(point_t point);

	virtual dimension_t getDimension() const = 0;

	virtual void draw() const = 0;
};

class Rectangle : public Shape {
private:
	const dimension_t _dimension;
	const GLfloat _red;
	const GLfloat _green;
	const GLfloat _blue;
public:
	Rectangle(point_t point, const dimension_t dimension,	const GLfloat red,	const GLfloat green,
			const GLfloat blue);

	dimension_t getDimension() const;

	void draw() const;

};

class ShapeContainer {
protected:
    vector<Shape*> _shapes;
public:
    vector<Shape*> get_shapes() {
    	return _shapes;
    }

    virtual int contains(const point_t&) const = 0;

    virtual void add(Shape* shape) = 0;

    virtual void shapeBeforeMove(Shape*, point_t&) = 0;

    virtual int is_occupied(const point_t& point) const;

    virtual ~ShapeContainer() {}

};

class SubField : public ShapeContainer {
    private:
        const point_t _point;
        const dimension_t _dimension;
    public:
        SubField(const point_t&, const dimension_t&);

        int contains(const point_t&) const;

        int contains(const int, const int) const;

        int contains(const point_t& point, const dimension_t& dimension) const;

        int containsEntirely(const point_t& point, const dimension_t& dimension) const;

        void shapeBeforeMove(Shape *shape, point_t& point);

        void remove(Shape*);

        void add(Shape* shape);

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
        const dimension_t _dimension;
    public:
        StandardField(const dimension_t&);

        int contains(const point_t&) const;

        void shapeBeforeMove(Shape*, point_t&);

        void add(Shape* shape);

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
        const dimension_t _dimension;
        vector<SubField*> _subFields;
    public:
        ComposedField(const dimension_t&, const int);

        int contains(const point_t&) const;

        void shapeBeforeMove(Shape*, point_t&);

        void add(Shape* shape);

        int is_occupied(const point_t& point) const;

        virtual ~ComposedField();
};

inline int ComposedField::contains(const point_t& point) const {
    return point.x >= 0
    		&& point.y >= 0
    		&& point.x < _dimension.width
    		&& point.y < _dimension.height;
}

#endif

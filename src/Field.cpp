#include "globals.hpp"
#include "Field.hpp"
#include <algorithm>

Shape::Shape(point_t point) : _container(0), _point(point) {
}

Shape::~Shape() {
}

void Shape::setContainer(ShapeContainer *container) {
	_container = container;
}

point_t Shape::getPoint() const {
 	return _point;
}

void Shape::setPoint(point_t point) {
 	if (_container != 0) {
 		_container->shapeBeforeMove(this, point);
 	}
 	_point = point;
}

Rectangle::Rectangle(point_t point, const dimension_t dimension, const GLfloat red,
		const GLfloat green, const GLfloat blue) : Shape(point) , _dimension(dimension),
	_red(red), _green(green), _blue(blue) {
}


void Rectangle::draw() const {
	glColor3f(_red, _green, _blue);
    glRectf((float)_point.x, (float)_point.y, (float)(_point.x + _dimension.width),
        (float)(_point.y + _dimension.height));

}

dimension_t Rectangle::getDimension() const {
	return _dimension;
}

int ShapeContainer::is_occupied(const point_t& point) const {
    for (unsigned int i = 0; i < _shapes.size(); i++) {
        Shape *element = _shapes[i];
        if (point.x >= element->getPoint().x && point.y >= element->getPoint().y
                && point.x <= (element->getPoint().x + element->getDimension().width - 1)
                && point.y <= (element->getPoint().y + element->getDimension().height - 1)) {
             return TRUE;
         }
    }
    return FALSE;
}


ComposedField::ComposedField(const dimension_t& dimension, const int subFields) : _dimension(dimension) {
	if (dimension.width % subFields != 0) {
		throw "invalid subFields";
	}

	if (dimension.height % subFields != 0) {
		throw "invalid subFields";
	}

	int dx = dimension.width / subFields;
	int dy = dimension.height / subFields;

	dimension_t subFieldDim{dx, dy};

	for (int ix = 0; ix < subFields; ix++) {
		for (int iy = 0; iy < subFields; iy++) {
			point_t point{ix * dx, iy * dy};
			SubField* subField = new SubField(point, subFieldDim);

			SubField s(point, subFieldDim);

			_subFields.push_back(subField);
		}
	}
}

int ComposedField::is_occupied(const point_t& point) const {
	for (unsigned int i = 0; i < _subFields.size(); i++) {
		SubField* subField = _subFields[i];
		if (subField->contains(point)) {
			return subField->is_occupied(point);
		}
	}
	return FALSE;
}

void ComposedField::add(Shape *shape) {
	for (unsigned int i = 0; i < _subFields.size(); i++) {
		SubField* subField = _subFields[i];
		if (subField->contains(shape->getPoint(), shape->getDimension())) {
			subField->add(shape);
			shape->setContainer(this);
		}
	}
    _shapes.push_back(shape);
}

ComposedField::~ComposedField() {
	for (vector<SubField*>::iterator i = _subFields.begin(); i != _subFields.end(); ++i) {
		delete (*i);
	}

/*	for (vector<shape_t*>::iterator i = _shapes.begin(); i != _shapes.end(); ++i) {
		delete (*i);
	}
*/
}

void ComposedField::shapeBeforeMove(Shape *shape, point_t& point) {
	for (vector<SubField*>::iterator i = _subFields.begin(); i != _subFields.end(); ++i) {
		SubField* subField = *i;

		/*
		 * it seems to be slower!
		if (subField->contains(*shape.getPoint(), shape.dimension)) {
			if (subField->containsEntirely(*shape.getPoint(), shape.dimension) &&
					subField->containsEntirely(point, shape.dimension)) {
				break;
			} else {
				subField->remove(&shape);
			}
		}
*/
		if (subField->contains(shape->getPoint(), shape->getDimension())) {
			subField->remove(shape);
		}
		if (subField->contains(point, shape->getDimension())) {
			subField->add(shape);
		}
	}

}

// StandardField

StandardField::StandardField(const dimension_t& dimension) :
		_dimension(dimension)
{
}

void StandardField::add(Shape *shape) {
    _shapes.push_back(shape);
}

StandardField::~StandardField() {

}

void StandardField::shapeBeforeMove(Shape* shape, point_t& point) {
}

// SubField

SubField::SubField(const point_t& point, const dimension_t& dimension) :
		_point(point),
		_dimension(dimension)
{
}

void SubField::add(Shape *shape) {
    _shapes.push_back(shape);
}

int SubField::containsEntirely(const point_t& point, const dimension_t& dimension) const {
	return contains(point) &&
			contains(point.x + dimension.width -1, point.y + dimension.height -1) &&
			contains(point.x + dimension.width -1, point.y) &&
			contains(point.x, point.y + dimension.height -1);
}

int SubField::contains(const point_t& point, const dimension_t& dimension) const {
	return contains(point) ||
			contains(point.x + dimension.width -1, point.y + dimension.height -1) ||
			contains(point.x + dimension.width -1, point.y) ||
			contains(point.x, point.y + dimension.height -1);
}

SubField::~SubField() {

}

void SubField::shapeBeforeMove(Shape *shape, point_t& point) {

}

void SubField::remove(Shape *shape) {
	vector<Shape*>::iterator i = find(_shapes.begin(), _shapes.end(), shape);
	if (i != _shapes.end()) {
		_shapes.erase(i);
	}
}

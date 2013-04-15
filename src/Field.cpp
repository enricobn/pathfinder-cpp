#include "globals.hpp"
#include "Field.hpp"
#include <algorithm>

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

void ComposedField::add(shape_t *shape) {
	for (unsigned int i = 0; i < _subFields.size(); i++) {
		SubField* subField = _subFields[i];
		if (subField->accepts(shape)) {
			subField->add(shape);
			shape->_shapeContainer = this;
		}
	}
    _shapes.push_back(shape);
}

ComposedField::~ComposedField() {

}

vector<shape_t *> *ComposedField::get_shapes() {
    return &_shapes;
}

void ComposedField::shapeMoved(shape_t& shape, const point_t& point) {
	for (unsigned int i = 0; i < _subFields.size(); i++) {
		SubField* subField = _subFields[i];
		subField->remove(&shape);
		if (subField->accepts(&shape)) {
			subField->add(&shape);
		}
	}

}

// StandardField

StandardField::StandardField(const dimension_t& dimension) :
		_dimension(dimension)
{
}

int StandardField::is_occupied(const point_t& point) const {
    for (unsigned int i = 0; i < _shapes.size(); i++) {
        const shape_t *element = _shapes[i];
        if (point.x >= element->getPoint()->x && point.y >= element->getPoint()->y
                && point.x <= (element->getPoint()->x + element->dimension.width - 1)
                && point.y <= (element->getPoint()->y + element->dimension.height - 1)) {
             return TRUE;
         }
    }
    return FALSE;
}

void StandardField::add(shape_t *shape) {
    _shapes.push_back(shape);
}

StandardField::~StandardField() {

}

vector<shape_t *> *StandardField::get_shapes() {
    return &_shapes;
}

void StandardField::shapeMoved(shape_t& shape, const point_t& point) {
}

// SubField

SubField::SubField(const point_t& point, const dimension_t& dimension) :
		_point(point),
		_dimension(dimension)
{
}

int SubField::is_occupied(const point_t& point) const {
    for (unsigned int i = 0; i < _shapes.size(); i++) {
        const shape_t *element = _shapes[i];
        if (point.x >= element->getPoint()->x && point.y >= element->getPoint()->y
                && point.x <= (element->getPoint()->x + element->dimension.width - 1)
                && point.y <= (element->getPoint()->y + element->dimension.height - 1)) {
             return TRUE;
         }
    }
    return FALSE;
}

void SubField::add(shape_t *shape) {
    _shapes.push_back(shape);
}

void SubField::remove(const shape_t* shape) {
	vector<shape_t*>::iterator i = find(_shapes.begin(), _shapes.end(), shape);
	if (i != _shapes.end()) // == vector.end() means the element was not found
		_shapes.erase(i);
}

SubField::~SubField() {

}

vector<shape_t *> *SubField::get_shapes() {
    return &_shapes;
}

int SubField::accepts(const shape_t* shape) const {
	return contains(*shape->getPoint()) ||
			contains(shape->getPoint()->x + shape->dimension.width -1, shape->getPoint()->y) ||
			contains(shape->getPoint()->x + shape->dimension.width -1, shape->getPoint()->y + shape->dimension.height -1) ||
			contains(shape->getPoint()->x, shape->getPoint()->y + shape->dimension.height);
}

void SubField::shapeMoved(shape_t& shape, const point_t& point) {

}


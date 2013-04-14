#include "globals.hpp"
#include "Field.hpp"

CField::CField(const dimension_t& dimension, const int subFields) : _dimension(dimension) {
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

int CField::is_occupied(const point_t& point) const {
	for (unsigned int i = 0; i < _subFields.size(); i++) {
		SubField* subField = _subFields[i];
		if (subField->contains(point)) {
			return subField->is_occupied(point);
		}
	}
	return FALSE;
}

void CField::add(const shape_t *shape) {
	for (unsigned int i = 0; i < _subFields.size(); i++) {
		SubField* subField = _subFields[i];
		if (subField->contains(shape)) {
			subField->add(shape);
		}
	}
    _shapes.push_back(shape);
}

CField::~CField() {

}

vector<const shape_t *> *CField::get_shapes() {
    return &_shapes;
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

void StandardField::add(const shape_t *shape) {
    _shapes.push_back(shape);
}

StandardField::~StandardField() {

}

vector<const shape_t *> *StandardField::get_shapes() {
    return &_shapes;
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

void SubField::add(const shape_t *shape) {
    _shapes.push_back(shape);
}

SubField::~SubField() {

}

vector<const shape_t *> *SubField::get_shapes() {
    return &_shapes;
}

int SubField::contains(const shape_t* shape) const {
	return contains(*shape->getPoint());
}


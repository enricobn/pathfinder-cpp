#include "globals.hpp"
#include "Field.hpp"

CField::CField(const dimension_t& dimension) : _dimension(dimension) {
}

int CField::is_occupied(const point_t& point) {
    for (unsigned int i = 0; i < _shapes.size(); i++) {
        const shape_t *element = _shapes[i];
        if (point.x >= element->point.x && point.y >= element->point.y
                && point.x <= (element->point.x + element->dimension.width - 1) 
                && point.y <= (element->point.y + element->dimension.height - 1)) {
             return TRUE;
         }
    }
    return FALSE;
}

void CField::add(const shape_t *shape) {
    _shapes.push_back(shape);
}

CField::~CField() {

}

vector<const shape_t *> *CField::get_shapes() {
    return &_shapes;
}

// SubField

SubField::SubField(const point_t& point, const dimension_t& dimension) :
		_point(point),
		_dimension(dimension)
{
}

int SubField::is_occupied(const point_t& point) {
    for (unsigned int i = 0; i < _shapes.size(); i++) {
        const shape_t *element = _shapes[i];
        if (point.x >= element->point.x && point.y >= element->point.y
                && point.x <= (element->point.x + element->dimension.width - 1)
                && point.y <= (element->point.y + element->dimension.height - 1)) {
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

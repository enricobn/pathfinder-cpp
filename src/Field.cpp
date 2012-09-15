#include "globals.hpp"
#include "Field.hpp"

CField::CField(dimension_t dimension) {
    _dimension = dimension;
}

int CField::is_occupied(point_t& point) {
    for (unsigned int i =0; i < _shapes.size(); i++) {
        shape_t *element = _shapes[i];
        if (point.x >= element->point.x && point.y >= element->point.y
                && point.x <= (element->point.x + element->dimension.width - 1) 
                && point.y <= (element->point.y + element->dimension.height - 1)) {
             return TRUE;
         }
    }
    return FALSE;
}

int CField::contains(point_t& point) {
    return (point.x >= 0 && point.y >= 0
        && point.x < _dimension.width
        && point.y < _dimension.height);
}

void CField::add(shape_t *shape) {
    _shapes.push_back(shape);
}

vector<shape_t *> *CField::get_shapes() {
    return &_shapes;
}

int point_equals(const point_t& a, const point_t& b) {
    return a.x == b.x && a.y == b.y;
}


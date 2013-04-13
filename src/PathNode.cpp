/*
 * PathNode.cpp
 *
 *  Created on: Apr 13, 2013
 *      Author: enrico
 */

#include "globals.hpp"
#include "PathNode.hpp"

PathNode::PathNode(PathNode *parent, point_t& point, point_t& to) {
    _parent = parent;
    _point = point;
    _to = to;
    _H = (abs(to.x - point.x) + abs(to.y - point.y)) * 10;
    _G = G_vs(parent);
    _F = _H + _G;
}

void PathNode::set_parent(PathNode *parent) {
	_parent = parent;
    _G = G_vs(parent);
    _F = _H + _G;
}

void PathNode::print() {
	printf("(%d,%d)=%d\n", _point.x, _point.y, _F);
}

void PathNode::fprint(FILE *file) {
	fprintf(file, "(%d,%d)=%d\n", _point.x, _point.y, _F);
}

int PathNode::G_vs(const PathNode *vs) {
    if (vs == NULL) {
        return 0;
    }
    int g = vs->_G;
    if (_point.x == vs->_point.x || _point.y == vs->_point.y) {
        g += 10;
    } else {
        g += 14;
    }
    return g;
}

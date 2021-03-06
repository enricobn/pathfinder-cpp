/*
 * PathNode.hpp
 *
 *  Created on: Apr 13, 2013
 *      Author: enrico
 */
#include "Field.hpp"

#ifndef PATHNODE_HPP_
#define PATHNODE_HPP_

class PathNode {
    private :
        PathNode *_parent;
        point_t _point;
        point_t _to;
        int _F;
        int _G;
        int _H;
    public :
        PathNode(PathNode *parent, point_t& point, point_t& to);

        PathNode *get_parent();

        void set_parent(PathNode *parent);

        point_t& get_point();

        int get_F();

        int get_G();

        int G_vs(const PathNode *vs);

        bool operator ==(const PathNode& other);

        void print();

        void fprint(FILE *file);
};

inline bool PathNode::operator ==(const PathNode& other) {
    return _point == other._point;
}

inline PathNode *PathNode::get_parent() {
    return _parent;
}

inline int PathNode::get_F() {
    return _F;
}

inline int PathNode::get_G() {
    return _G;
}

inline point_t& PathNode::get_point() {
    return _point;
}

#endif /* PATHNODE_HPP_ */

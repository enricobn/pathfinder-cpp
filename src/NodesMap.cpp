/*
 * NodesMap.cpp
 *
 *  Created on: Apr 14, 2013
 *      Author: enrico
 */

#include "NodesMap.hpp"
#include "globals.hpp"

NodesMap::NodesMap() {
}

void NodesMap::add(PathNode *node) {
	map<int, map<int, PathNode*> >::iterator iPoint_x = _nodes.find(node->get_point().x);
	if (iPoint_x == _nodes.end()) {
		map<int, PathNode*> xMap;
		xMap.insert(pair<int, PathNode *>(node->get_point().y, node));
		_nodes.insert(pair<int, map<int, PathNode* > >(node->get_point().x, xMap));
	} else {
		iPoint_x->second.insert(pair<int, PathNode *>(node->get_point().y, node));
	}
}

void NodesMap::remove(PathNode *node) {
	map<int, map<int, PathNode*> >::iterator iPoint_x = _nodes.find(node->get_point().x);
	iPoint_x->second.erase(node->get_point().y);
}

PathNode* NodesMap::find(const point_t& point) {
	map<int, map<int, PathNode*> >::iterator iPoint_x = _nodes.find(point.x);
	if (iPoint_x == _nodes.end()) {
		return NULL;
	} else {
		map<int, PathNode*>::iterator iPoint_y = iPoint_x->second.find(point.y);
		if (iPoint_y == iPoint_x->second.end()) {
			return NULL;
		}
		return iPoint_y->second;
	}
}


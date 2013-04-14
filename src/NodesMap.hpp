/*
 * NodesMap.hpp
 *
 *  Created on: Apr 14, 2013
 *      Author: enrico
 */

#include <map>
#include "PathNode.hpp"
using namespace std;

#ifndef NODESMAP_HPP_
#define NODESMAP_HPP_

class NodesMap {
	private:
		map<int, map<int, PathNode*> > _nodes;
public:
	NodesMap();

	void add(PathNode *node);

	void remove(PathNode *node);

	PathNode* find(const point_t&);

};

#endif /* NODESMAP_HPP_ */

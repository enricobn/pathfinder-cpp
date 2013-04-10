/* thanks to http://www.policyalmanac.org/games/aStarTutorial.htm */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <algorithm>
#include <map>
#include "globals.hpp"
#include "astar_pathfinder.hpp"
#include "Field.hpp"
#include "List.hpp"

#define null 0
//#define PRINT
//#define FPRINT

class CPathNode {
    private :
        CPathNode *_parent;
        point_t _point;
        point_t _to;
        int _F;
        int _G;
        int _H;
    public :
        CPathNode(CPathNode *parent, point_t& point, point_t& to);
        CPathNode *get_parent();
        void set_parent(CPathNode *parent);
        point_t& get_point();
        int get_F();
        int get_G();
        int G_vs(const CPathNode *vs);
        bool operator ==(const CPathNode& other);
        void print();
        void fprint(FILE *file);
};

CPathNode::CPathNode(CPathNode *parent, point_t& point, point_t& to) {
    _parent = parent;
    _point = point;
    _to = to;
    _H = (abs(to.x - point.x) + abs(to.y - point.y)) * 10;    
    _G = G_vs(parent);
    _F = _H + _G;
}

void CPathNode::set_parent(CPathNode *parent) {
    _H = (abs(_to.x - _point.x) + abs(_to.y - _point.y)) * 10;
    _G = G_vs(parent);
    _F = _H + _G;
}

void CPathNode::print() {
	printf("(%d,%d)=%d\n", _point.x, _point.y, _F);
}

void CPathNode::fprint(FILE *file) {
	fprintf(file, "(%d,%d)=%d\n", _point.x, _point.y, _F);
}

int CPathNode::G_vs(const CPathNode *vs) {
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

bool CPathNode::operator ==(const CPathNode& other) {
    return _point == other._point;
}

CPathNode *CPathNode::get_parent() {
    return _parent;
}

int CPathNode::get_F() {
    return _F;
}

int CPathNode::get_G() {
    return _G;
}

point_t& CPathNode::get_point() {
    return _point;
}

static point_t *adjacents_tmp = NULL;

void fprint_point(FILE *file, point_t point) {
	fprintf(file, "(%d,%d)\n", point.x, point.y);
}

void print_point(point_t point) {
	printf("(%d,%d)", point.x, point.y);
}

static point_t *get_adjacents(point_t point) {
    if (adjacents_tmp == NULL) {
        adjacents_tmp = (point_t *)malloc(sizeof(point_t) * 8);
        if (adjacents_tmp == NULL) {
            ERROR("Cannot allocate adjacents_tmp.");
            exit(TRUE);
        }
    }
    adjacents_tmp[0].x = point.x + 1;
    adjacents_tmp[0].y = point.y;

    adjacents_tmp[1].x = point.x + 1;
    adjacents_tmp[1].y = point.y + 1;

    adjacents_tmp[2].x = point.x;
    adjacents_tmp[2].y = point.y + 1;

    adjacents_tmp[3].x = point.x - 1;
    adjacents_tmp[3].y = point.y + 1;

    adjacents_tmp[4].x = point.x - 1;
    adjacents_tmp[4].y = point.y;

    adjacents_tmp[5].x = point.x - 1;
    adjacents_tmp[5].y = point.y - 1;

    adjacents_tmp[6].x = point.x;
    adjacents_tmp[6].y = point.y - 1;

    adjacents_tmp[7].x = point.x + 1;
    adjacents_tmp[7].y = point.y - 1;
    
    return adjacents_tmp;
}

/*
int find_node(vector<CPathNode *> nodes, CPathNode *node) {
    for (unsigned int i = 0; i < nodes.size(); i++ ) {
        CPathNode *iNode = nodes[i];
        if (*iNode == *node) {
            return TRUE;
        }
    }
    return FALSE;
}
*/

int CPathNode_equals(CPathNode *e1, CPathNode *e2) {
	return e1->get_point() == e2->get_point();
}

static map<point_t, CPathNode*> *open = NULL;
static map<point_t, CPathNode*> *closed = NULL;

FILE *file = NULL;

CPathNode *get_path_internal(CField& field, point_t from, point_t to) {
	#ifdef PRINT
		printf("get_path_internal ");
		print_point(from);
		printf(" ");
		print_point(to);
		printf("\n");
	#endif
	int debug1 = from.x == 61 && from.y == 73 && to.x == 0 && to.y == 1;
	#ifdef FPRINT
		if (file == NULL) {
			file = fopen("out.txt","w");
		}
		fprintf(file, "from");
		fprint_point(file, from);
		fprintf(file, "to");
		fprint_point(file, to);
	#endif
//    open.reserve(10000);
//    closed.reserve(10000);
    open = new map<point_t, CPathNode *>();
    closed = new map<point_t, CPathNode *>();
    
    CPathNode *from_node = new CPathNode(NULL, from, to);
    if (from_node == NULL) {
    	ERROR("Cannot create new CPathNode");
    }
    open->insert(pair<point_t,CPathNode *>(from, from_node));

    CPathNode *target_node = NULL;

    while (TRUE) {
		#ifdef FPRINT
        	fprintf(file, "%u ", open->size());
		#endif
//        if (debug1) {
//        	printf("%u\n", open->size());
//        }
        if (open->size() == 0) {
            return NULL;
        }
        int min = INT_MAX;
        CPathNode *min_node = NULL;

        for( map<point_t,CPathNode *>::iterator ii=open->begin(); ii!= open->end(); ++ii) {
        	CPathNode *node = (*ii).second;
            if (min_node == NULL || node->get_F() < min) {
                min = node->get_F();
                min_node = node;
            }
        }
        
		#ifdef FPRINT
        	min_node->fprint(file);
		#endif

        if (min_node->get_point() == to) {
//            target_node = (CPathNode *)malloc(sizeof(CPathNode));
//            memcpy(target_node, min_node, sizeof(CPathNode));
        	target_node = min_node;
            break;
        }

        point_t *adjacents = get_adjacents(min_node->get_point());

        for (int i = 0; i < 8; i++) {
            point_t point = adjacents[i];
			#ifdef FPRINT
//            	fprint_point(file, point);
			#endif
			#ifdef PRINT
            	printf("adjacent");
            	print_point(point);
            	printf("\n");
			#endif
            // I do not consider the end point to be occupied, so I can move towards it
            if (field.contains(point) && (point == to || !field.is_occupied(point))) {
                map<point_t,CPathNode *>::iterator iClosed = closed->find(point);
                if (iClosed == closed->end()) {
                    CPathNode *node = new CPathNode(min_node, point, to);
                    if (node == NULL) {
                        ERROR("Error allocating new node");
                    }
                	map<point_t,CPathNode *>::iterator iOpen = open->find(point);
                    if (iOpen == open->end()) {
                    	open->insert(pair<point_t,CPathNode *>(point, node));
                    } else {
						#ifdef PRINT
                    		printf("Found (%d,%d)\n", iOpen->first.x, iOpen->first.y);
						#endif
                    	CPathNode *got = iOpen->second;

						int gToMin = min_node->G_vs(got);
						if (gToMin < node->get_G()) {
							got->set_parent(min_node);
						}
                    	delete node;
                    }
                }
            }
        }

        open->erase(min_node->get_point());
        closed->insert(pair<point_t,CPathNode *>(min_node->get_point(), min_node));
    }
    return target_node;
}

/*
void clear(vector<CPathNode*> nodes) {
    vector<CPathNode*>::iterator it;
    for (it = nodes.begin(); it != nodes.end(); it++ ) {
        CPathNode *node = *it;
        try {
            free(node);
        } catch(...) {
            ERROR("Error freeing vector elements");
        }
    }
    nodes.clear(); 
}
*/

void clear(map<point_t, CPathNode *> *m) {
	for( map<point_t,CPathNode *>::iterator ii=m->begin(); ii!= m->end(); ++ii) {
//		delete &((*ii).first);
		delete (*ii).second;
	}
	delete m;
}

point_t *get_next_to_path(CField& field, point_t from, point_t to) {
    CPathNode *target_node = get_path_internal(field, from, to);
    if (target_node == NULL) {
        clear(open);
        clear(closed);
        return NULL;
    }

    point_t *point = NULL;   
    
    while (target_node != NULL) {
        /* the path can contains occupied points. Typically it can be only the end point */
        if (!field.is_occupied(target_node->get_point())) {
        	// I cannot do: point = &target_node->get_point();
        	// since it's an address of a temporary, then I get a warning
        	point_t node_point = target_node->get_point();
            point = &node_point;
        }
        target_node = target_node->get_parent();
    }

    /* I copy it so I can completely free open and closed vectors */
    point_t *result = NULL;
    if (point != NULL) {
        result = (point_t *)malloc(sizeof(point_t));
        memcpy(result, point, sizeof(point_t));
		#ifdef PRINT
        	printf("(%d,%d)\n", result->x, result->y);
		#endif
    }
    
    clear(open);
    clear(closed);

    return result;
}

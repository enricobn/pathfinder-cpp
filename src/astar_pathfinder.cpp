/* thanks to http://www.policyalmanac.org/games/aStarTutorial.htm */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <algorithm>
#include <vector>
#include "globals.hpp"
#include "astar_pathfinder.hpp"
#include "Field.hpp"
#include "List.hpp"

#define null 0

class CPathNode {
    private :
        CPathNode *_parent;
        point_t _point;
        int _F;
        int _G;
        int _H;
        int G_vs(CPathNode *vs);
    public :
        CPathNode(CPathNode *parent, point_t point, point_t to);
        CPathNode *get_parent();
        point_t get_point();
        int get_F();
        bool operator ==(const CPathNode other);
        void print();
};

CPathNode::CPathNode(CPathNode *parent, point_t point, point_t to) {
    _parent = parent;
    _point = point;
    _H = (abs(to.x - point.x) + abs(to.y - point.y)) * 10;    
    _G = G_vs(parent);
    _F = _H + _G;
}

void CPathNode::print() {
    printf("this = %p parent = %p (%d, %d) = %d)\n", this, _parent, _point.x, _point.y, _F); 
}

int CPathNode::G_vs(CPathNode *vs) {
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

bool CPathNode::operator ==(const CPathNode other) {
    return point_equals(_point, other._point);
}

CPathNode *CPathNode::get_parent() {
    return _parent;
}

int CPathNode::get_F() {
    return _F;
}

point_t CPathNode::get_point() {
    return _point;
}

static point_t *adjacents_tmp = NULL;

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

int find_node(vector<CPathNode *> nodes, CPathNode *node) {
    for (unsigned int i = 0; i < nodes.size(); i++ ) {
        CPathNode *iNode = nodes[i];
        if (*iNode == *node) {
            return TRUE;
        }
    }
    return FALSE;
}

int CPathNode_equals(const void *e1, const void *e2) {
	return point_equals(((CPathNode *)e1)->get_point(), ((CPathNode *)e2)->get_point());
}

static List *open = NULL;
static List *closed = NULL;

CPathNode *get_path_internal(CField field, point_t from, point_t to) {
//    open.reserve(10000);
//    closed.reserve(10000);
    open = new List(CPathNode_equals);
    closed = new List(CPathNode_equals);
    
    CPathNode *from_node = new CPathNode(NULL, from, to); 
    open->add(from_node);

    CPathNode *target_node = NULL;

    while (TRUE) {
//        printf("%u \n", open.size());
        if (open->size() == 0) {
            return NULL;
        }
        int min = INT_MAX;
        CPathNode *min_node = NULL;

        
        CPathNode *node = NULL;
        LIST_FOREACH_START(open, node)
//            printf("iterating nodes ");
//            node->print();

       	    if (node == node->get_parent()) {
                printf("node == node.parent!!!\n");
//                printf("node %d = %d, %d\n", &node, node.point.x, node.point.y);
//                printf("node parent %d = %d, %d\n", node.parent, node.parent->point.x, node.parent->point.y);
//                exit(1);
            }

//                printf("point=%d,%d=%d\n", node.point.x, node.point.y, node.F);
            if (min_node == NULL || node->get_F() < min) {
                min = node->get_F();
                min_node = node;
            }
        LIST_FOREACH_END(open)
        
//        printf("min_node = %p\n", min_node);
        
        if (point_equals(min_node->get_point(), to)) {
            target_node = (CPathNode *)malloc(sizeof(CPathNode));
            memcpy(target_node, min_node, sizeof(CPathNode));
            break;
        }

        point_t *adjacents = get_adjacents(min_node->get_point());

        for (int i = 0; i < 8; i++) {
            point_t point = adjacents[i];
/*            printf("adjacent=%d,%d\n", point.x, point.y);*/
            // I do not consider the end point to be occupied, so I can move towards it
            if (field.contains(point) && (point_equals(point, to) || !field.is_occupied(point))) {
                CPathNode *node = new CPathNode(min_node, point, to);
                if (node == NULL) {
                    ERROR("Error allocating new node");
                }
//                printf("node created: ");
//               (*node).print(); 
/*
          	    if (&node == &node->parent) {
                    printf("created: are equals!!!\n");
                }
*/
//                vector<CPathNode*>::iterator iClosed = std::find(closed.begin(), closed.end(), node);
                if (!closed->contains(node)) {
//                    vector<CPathNode*>::iterator iOpen = std::find(open.begin(), open.end(), node);
                    if (!open->contains(node)) {
//                        printf("not found\n");
                        open->add(node);
//                        printf("pushed %d\n", node);
//	               	    if (node == node.get_parent()) {
//                            printf("added: are equals!!!\n");
//                        }
                    } else {
//                        printf("found\n");
/* TODO
                        int g_to_min = path_node_G_vs(min_node, node);
                        if (g_to_min < node->G) {
                            printf("optimized path\n");
                            node->parent = min_node;
                            node->G = path_node_G(node);
                            node->F = path_node_F(node);
                        }
*/
                        free(node);
                    }
                } else {
                    free(node);
                }
//                free(node);
/*
            } else {
                printf("Excluded %d, %d, fc=%d, oc=%d\n", point.x, point.y, field_contains(field, point), field_is_occupied(field, point));
*/
            }
        }

//        printf("open before %d\n", open.size());
        open->remove(min_node);
//        printf("open after %d\n", open.size());
        closed->add(min_node);
    }

//    printf("found %d, %d\n", target_node->point.x, target_node->point.y);     
    
    return target_node;
}

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

point_t *get_next_to_path(CField field, point_t from, point_t to) {
    CPathNode *target_node = get_path_internal(field, from, to);
    if (target_node == NULL) {
        open->clear(TRUE);
        free(open);
        closed->clear(TRUE);
        free(closed);
        return NULL;
    }

    point_t *point = NULL;   
    
    while (target_node->get_parent() != NULL) {
//        printf("cucu\n");
//	    if (*target_node == *target_node.get_parent()) {
//            printf("are equals!!!\n");
//        } else {
//            printf("target_node %d, %d, %d\n", target_node, target_node->point.x, target_node->point.y);
//            printf("target_node parent %d, %d, %d\n", target_node->parent, target_node->parent->point.x, target_node->parent->point.y);
//        }
        /* the path can contains occupied points. Tipically it can be only the end point */ 
        if (!field.is_occupied(target_node->get_point())) {
            point = &target_node->get_point(); 
        }
        target_node = target_node->get_parent();
    }

    /* I copy it so I can completely free open and closed vectors */
    point_t *result = NULL;
    if (point != NULL) {
        result = (point_t *)malloc(sizeof(point_t));
        memcpy(result, point, sizeof(point_t));
    }
    
    open->clear(TRUE);
    free(open);
    closed->clear(TRUE);
    free(closed);
    return result;
}
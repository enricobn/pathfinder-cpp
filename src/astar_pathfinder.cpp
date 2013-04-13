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
#include "PathNode.hpp"

#define null 0
//#define PRINT
//#define FPRINT

void fprint_point(FILE *file, point_t point) {
	fprintf(file, "(%d,%d)\n", point.x, point.y);
}

void print_point(point_t point) {
	printf("(%d,%d)", point.x, point.y);
}

int PathNode_equals(PathNode *e1, PathNode *e2) {
	return e1->get_point() == e2->get_point();
}

static map<point_t, PathNode*> *open = NULL;
static map<point_t, PathNode*> *closed = NULL;

FILE *file = NULL;

point_t adjacent = point_t();

PathNode *get_path_internal(CField& field, point_t from, point_t to) {
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

    open = new map<point_t, PathNode *>();
    closed = new map<point_t, PathNode *>();
    
    PathNode *from_node = new PathNode(NULL, from, to);
    if (from_node == NULL) {
    	ERROR("Cannot create new PathNode");
    }
    open->insert(pair<point_t,PathNode *>(from, from_node));

    PathNode *target_node = NULL;

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
        PathNode *min_node = NULL;

        for( map<point_t,PathNode *>::iterator ii=open->begin(); ii!= open->end(); ++ii) {
        	PathNode *node = (*ii).second;
            if (min_node == NULL || node->get_F() < min) {
                min = node->get_F();
                min_node = node;
            }
        }
        
		#ifdef FPRINT
        	min_node->fprint(file);
		#endif

        if (min_node->get_point() == to) {
        	target_node = min_node;
            break;
        }

        point_t min_node_point = min_node->get_point();
        for (int x = min_node_point.x -1; x < min_node_point.x + 2; x++) {
    		adjacent.x = x;
        	for (int y = min_node_point.y -1; y < min_node_point.y + 2 ; y++) {
        		if (x == min_node_point.x && y == min_node_point.y) {
        			continue;
        		}
        		adjacent.y = y;
				#ifdef FPRINT
	//            	fprint_point(file, point);
				#endif
				#ifdef PRINT
					printf("adjacent");
					print_point(adjacent);
					printf("\n");
				#endif
				// I do not consider the end point to be occupied, so I can move towards it
				if (field.contains(adjacent) && (adjacent == to || !field.is_occupied(adjacent))) {
					map<point_t,PathNode *>::iterator iClosed = closed->find(adjacent);
					if (iClosed == closed->end()) {
						PathNode *node = new PathNode(min_node, adjacent, to);
						if (node == NULL) {
							ERROR("Error allocating new node");
						}
						map<point_t,PathNode *>::iterator iOpen = open->find(adjacent);
						if (iOpen == open->end()) {
							open->insert(pair<point_t,PathNode *>(adjacent, node));
						} else {
							#ifdef PRINT
								printf("Found (%d,%d)\n", iOpen->first.x, iOpen->first.y);
							#endif
							PathNode *got = iOpen->second;

							int gToMin = min_node->G_vs(got);
							if (gToMin < node->get_G()) {
								got->set_parent(min_node);
							}
							delete node;
						}
					}
                }

        	}
        }
        open->erase(min_node->get_point());
        closed->insert(pair<point_t,PathNode *>(min_node->get_point(), min_node));
    }
    return target_node;
}

void clear(map<point_t, PathNode *> *m) {
	for( map<point_t,PathNode *>::iterator ii=m->begin(); ii!= m->end(); ++ii) {
		delete (*ii).second;
	}
	delete m;
}

point_t *get_next_to_path(CField& field, point_t from, point_t to) {
    PathNode *target_node = get_path_internal(field, from, to);
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

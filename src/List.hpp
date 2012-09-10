#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef LIST_H_
#define LIST_H_

struct cursor {
    void *current;
    struct cursor *next;
};

typedef int (*list_equals) (const void *e1, const void *e2);

class List {
private:
public:
    struct cursor *first;
    struct cursor *last;
    struct cursor **actual_cursor;
    int actual_cursor_number;
    list_equals equals;

    List(list_equals equals);
    void add(void *element);
    int contains(const void *element);
    void clear(int deep);
    int remove(void *element);
    int size();

};


#define LIST_FOREACH_START(LIST, ELEMENT) \
    LIST->actual_cursor_number++;\
/*    printf("List %s -> actual cursor number %d, file=%s, line=%d, \n", #LIST, LIST->actual_cursor_number, __FILE__, __LINE__);*/\
    for (LIST->actual_cursor[LIST->actual_cursor_number] = LIST->first; \
                LIST->actual_cursor[ LIST->actual_cursor_number] != NULL; \
                LIST->actual_cursor[ LIST->actual_cursor_number] = LIST->actual_cursor[ LIST->actual_cursor_number]->next) {\
        ELEMENT = (__typeof__(ELEMENT)) LIST->actual_cursor[ LIST->actual_cursor_number]->current;

#define LIST_FOREACH_END(LIST) \
    }\
    LIST->actual_cursor[ LIST->actual_cursor_number--] = NULL; 
#endif

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "globals.hpp"

#ifndef LIST_H_
#define LIST_H_

template <class T>
struct cursor {
    T current;
    struct cursor<T> *next;
};

template <class T>
class List {
private:
public:
	typedef int (*list_equals) (T e1, T e2);
    struct cursor<T> *first;
    struct cursor<T> *last;
    struct cursor<T> **actual_cursor;
    int actual_cursor_number;
    list_equals equals;

    List(list_equals equals);
    ~List();
    void add(T element);
    int contains(const T element);
    void clear(int deep);
    int remove(T element);
    int size();

};

template <class T>
List<T>::List(list_equals equals) {
	this->equals = equals;
    actual_cursor = (struct cursor<T> **)malloc(10 * sizeof(struct cursor<T> *));
    if (actual_cursor == NULL)
    {
        ERROR("Error allocating array for foreach.");
    }
    actual_cursor_number = -1;
    first = NULL;
    last = NULL;
}

template <class T>
List<T>::~List() {
	free(actual_cursor);
}

template <class T>
void List<T>::add(T element) {
    struct cursor<T> *c = NEW_POINTER(struct cursor<T>);
    if (c == NULL) {
        ERROR("Cannot allocate new cursor.");
        exit(TRUE);
    }
    c->current = element;
    c->next = NULL;
    if (first == NULL) {
        first = c;
        last = c;
    } else {
       last->next = c;
       last = c;
    }
}

template <class T>
int List<T>::contains(const T element) {
    struct cursor<T> *cur = first;
    for (cur = first; cur != NULL; cur = cur->next) {
		if (equals(element, cur->current)) {
			return TRUE;
		}
    }
    return FALSE;
}

template <class T>
void List<T>::clear(int deep) {
    struct cursor<T> *cur = first;
    first = NULL;
    last = NULL;
    while (cur != NULL) {
        if (deep && cur->current != NULL) {
            delete cur->current;
        }
        struct cursor<T> *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

template <class T>
int List<T>::remove(T element) {
    struct cursor<T> *cur = first;
    struct cursor<T> *prev = first;
    for (cur = first; cur != NULL; cur = cur->next) {
        if (equals(element, cur->current)) {
        	int is_last = (cur == last);
            if (cur == first) {
                struct cursor<T> *tmp = first;
                first = first->next;
                free(tmp);
                tmp = NULL;
                if (is_last) {
                	last = first;
                }
            } else {
                struct cursor<T> *tmp = cur;
                prev->next = cur->next;
                free(tmp);
                tmp = NULL;
                if (is_last) {
                	last = prev;
                }
            }
            return TRUE;
        }
        prev = cur;
    }
    return FALSE;
}

template <class T>
int List<T>::size() {
    int count = 0;
    struct cursor<T> *cur = NULL;
    for (cur = first; cur != NULL; cur = cur->next) {
    	count++;
    }

    return count;
}

#define LIST_FOREACH_START(LIST, ELEMENT) \
    LIST->actual_cursor_number++;\
/*    printf("List %s -> actual cursor number %d, file=%s, line=%d, \n", #LIST, LIST->actual_cursor_number, __FILE__, __LINE__);*/\
    for (LIST->actual_cursor[LIST->actual_cursor_number] = LIST->first; \
                LIST->actual_cursor[ LIST->actual_cursor_number] != NULL; \
                LIST->actual_cursor[ LIST->actual_cursor_number] = LIST->actual_cursor[ LIST->actual_cursor_number]->next) {\
        ELEMENT = LIST->actual_cursor[ LIST->actual_cursor_number]->current;

#define LIST_FOREACH_END(LIST) \
    }\
    LIST->actual_cursor[ LIST->actual_cursor_number--] = NULL;

#endif

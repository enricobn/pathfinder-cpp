#include <stdlib.h>
#include "globals.hpp"
#include "List.hpp"

List::List(list_equals equals) {
	this->equals = equals;
    actual_cursor = (struct cursor **)malloc(10 * sizeof(struct cursor *));
    if (actual_cursor == NULL)
    {
        ERROR("Error allocating array for foreach.");
    }
    actual_cursor_number = -1;
    first = NULL;
    last = NULL;
}

void List::add(void *element) {
    struct cursor *c = NEW_POINTER(struct cursor);
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

int List::contains(const void *element) {
    struct cursor *cur = first;
    for (cur = first; cur != NULL; cur = cur->next) {
		if (equals(element, cur->current)) {
			return TRUE;
		}
    }
    return FALSE;
}

void List::clear(int deep) {
    struct cursor *cur = first;
    first = NULL;
    last = NULL;
    while (cur != NULL) {
        if (deep && cur->current != NULL) {
            free(cur->current);
        }
        struct cursor *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

int List::remove(void *element) {
    struct cursor *cur = first;
    struct cursor *prev = first;
    for (cur = first; cur != NULL; cur = cur->next) {
        if (equals(element, cur->current)) {
            if (cur == first) {
                struct cursor *tmp = first;
                first = first->next;
                free(tmp);
                tmp = NULL;
            } else {
                struct cursor *tmp = cur;
                prev->next = cur->next;
                free(tmp);
                tmp = NULL;
            }
            return TRUE;
        }
        prev = cur;
    }
    return FALSE;
}

int List::size() {
    int count = 0;
    void *element;
    LIST_FOREACH_START(this, element)
        count++;
    LIST_FOREACH_END(this)
    return count;
}
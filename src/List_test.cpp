#include <assert.h>
#include "globals.hpp"
#include "List.hpp"

int equals(int *e1, int *e2) {
    return *e1 == *e2;
}

int main(int argc, char** argv) {
    List<int*> *l = new List<int*>(equals);
    assert(l->first == NULL);
    assert(l->last == NULL);

    int a = 10;
    l->add(&a);
    assert(l->size() == 1);
    assert(l->first->current == &a);
    assert(l->last->current == &a);

    int b = 20;
    l->add(&b);
    assert(l->size() == 2);
    assert(l->first->current == &a);
    assert(l->last->current == &b);

    int c = 30;
    l->add(&c);
    assert(l->size() == 3);
    assert(l->first->current == &a);
    assert(l->last->current == &c);

    struct cursor<int*> *cur = l->first;
    
	assert(*cur->current == 10);

    cur = cur->next;
	assert(*cur->current == 20);
	
    cur = cur->next;    
    assert(*cur->current == 30);

    int count = 0;
    int *element;
    LIST_FOREACH_START(l, element)
        count++;
    LIST_FOREACH_END(l)

    assert(count == 3);

    int d = 40;

    assert(l->contains(&a));
    assert(l->contains(&b));
    assert(l->contains(&c));
    assert(!l->contains(&d));

    assert(l->remove(&a));
    assert(!l->contains(&a));
    assert(l->size() == 2);
    assert(l->first->current == &b);
    assert(l->last->current == &c);

    assert(l->remove(&c));
    assert(!l->contains(&c));
    assert(l->size() == 1);
    assert(l->first->current == &b);
    assert(l->last->current == &b);

    assert(l->remove(&b));
    assert(!l->contains(&b));
    assert(l->size() == 0);

    assert(l->first == NULL);
    assert(l->last == NULL);
    
    l->clear(FALSE);
    
    assert(!l->contains(&a));

    return 0;
}


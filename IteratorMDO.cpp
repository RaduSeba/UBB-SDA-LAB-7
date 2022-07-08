#include "IteratorMDO.h"
#include "MDO.h"

IteratorMDO::IteratorMDO(const MDO& d) : dict(d) {
    /* de adaugat */
    if (d.parent != NULL)
        current = d.parent->minim;
    else
        current = NULL;
}

void IteratorMDO::prim() {
    /* de adaugat */
    if (dict.parent != NULL)
        current = dict.parent->minim;
    else
        current = NULL;
}

void IteratorMDO::urmator() {
    /* de adaugat */
    current = dict.urmator(current);
}

bool IteratorMDO::valid() const {
    /* de adaugat */

    return current != NULL;
}

TElem IteratorMDO::element() const {
    /* de adaugat */
    return current->values;
}


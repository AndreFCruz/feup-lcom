#include "GVector.h"

struct gvector_t {
    unsigned size;              // number of elements in generic vector
    unsigned el_size;           // size of each element, in bytes
    unsigned capacity;          // Capacity of vector, larger than size to prevent continuous reallocs
    unsigned increments;        // Capacity increments on realloc

    void * array;
};

GVector * new_gvector(unsigned capacity, unsigned size_of) {
    if (0 == size_of)
        return NULL;

    GVector * self = (GVector *) malloc(size_of(gvector_t));
    if (NULL == self)
        return NULL;

    self->size = 0;
    self->increments = 10;
    self->capacity = capacity;

    if (0 == size)
        ptr->array = NULL;
    else
        ptr->array = malloc(size * size_of);

    /*INC??*/

    return self;
}

void delete_gvector(GVector * self) {
    free(self->array);
    free(self);
}

/* INCOMPLETE */

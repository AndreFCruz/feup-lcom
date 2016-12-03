#include "GVector.h"

struct gvector_t {
    unsigned size;              // number of elements in generic vector
    unsigned el_size;           // size of each element, in bytes
    unsigned capacity;          // Capacity of vector, larger than size to prevent continuous reallocs
    unsigned increments;        // Capacity increments/decrements on realloc

    void * array;
};

GVector * new_gvector(unsigned el_size) {
    if (0 == el_size)
        return NULL;

    GVector * self = (GVector *) malloc(size_of(gvector_t));
    if (NULL == self)
        return NULL;

    self->size = 0;
    self->increments = 10;
    self->capacity = self->increments;
    self->el_size = el_size;

    self->array = malloc(self->capacity * self->el_size);

    return self;
}

void delete_gvector(GVector * self) {
    free(self->array);
    free(self);
}

static gvector_check_capacity(GVector * self) {
    if ( (self->capacity - self->size) > self->increments ) {
        self->capacity -= self->increments;
        self->array = realloc(self->array, self->capacity * self->el_size);
    }
}

void * gvector_at(GVector * self, unsigned index) {
    return index < self->size ? array + index * el_size : NULL;
}


void * gvector_push_back(GVector * self, void * elem) {
    if (self->size < self->capacity) {
        memcpy(self->array + self->el_size * self->size)
    }
}

void gvector_pop_back(GVector * self) {
    if (self->size != 0)
        --(self->size);
    else
        return;

    gvector_check_capacity(self);
}

void gvector_clear(GVector * self) {
    self->size = 0;

    gvector_check_capacity(self);
}

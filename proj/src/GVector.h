#ifndef __GVECTOR_H
#define __GVECTOR_H

/**
 * Struct for Generic Vector Container
 */

struct gvector_t;
typedef struct gvector_t GVector;

/* Methods */
GVector * new_gvector(unsigned el_size);
void delete_gvector(GVector * self);

unsigned gvector_get_size(GVector * self);

void * gvector_at(GVector * self, unsigned index);
void * gvector_push_back(GVector * self, void * elem);  // realloc may happen

void gvector_pop_back(GVector * self);                  // realloc may happen
void gvector_clear(GVector * self);


#endif

#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct {
    void* data;
    size_t length;
    size_t capacity;
    size_t elementSize;
} Vector;

void VectorPush(void* value, Vector* vector);

#endif
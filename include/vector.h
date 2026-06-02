#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct {
    void* data;
    size_t length;
    size_t capacity;
    size_t elementSize;
} Vector;

void VectorInit(Vector* vector, size_t elementSize);
void VectorPush(void* value, Vector* vector);
void* VectorPop(Vector* vector);
void VectorDestroy(Vector* vector);

#endif
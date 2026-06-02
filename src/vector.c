#include "include/vector.h"
#include <string.h>
#include <stdint.h>

void VectorInit(Vector* vector, size_t elementSize) {
    vector->capacity = 8;
    vector->length = 0;
    vector->elementSize = elementSize;
    vector->data = malloc(vector->capacity * elementSize);
    
    if(vector->data == NULL) {
        vector->capacity = 0;
    }
}

void VectorPush(void* value, Vector* vector) {
    if(vector->length >= vector->capacity) {
        size_t newCapacity = (vector->capacity == 0) ? 8 : vector->capacity * 2;

        void* newData = realloc(vector->data, newCapacity * vector->elementSize);
        if(!newData) return;

        vector->data = newData;
        vector->capacity = newCapacity;
    }

    void* dest = (char*)vector->data + (vector->length * vector->elementSize);
    memcpy(dest, value, vector->elementSize);

    vector->length++;
}

void VectorDestroy(Vector* vector) {
    if(!vector) return;

    free(vector->data);
    vector->data = NULL;
    vector->length = 0;
    vector->capacity = 0;
}
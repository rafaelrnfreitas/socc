#include "include/vector.h"
#include <string.h>
#include <stdint.h>

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
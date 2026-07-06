#include <stddef.h>
#include <stdlib.h>

Vector newVector() {
    char** data = malloc(sizeof(char*) * 10);

    Vector v;
    v.ptr = 0

    if (data == NULL) {
        v.data = NULL;
        v.cap = 0;

        return v;
    }

    v.data = data;
    v.cap = 10;

    return v;
}

void pushToVector(Vector* v, char* value) {
    if (v->ptr == v->cap) {
        size_t newCap = v->cap * 2;
        char** newAlloc = realloc(v->data, sizeof(char*) * newCap);

        if (newAlloc == NULL) {
            fprintf(stderr, "Error: Could not reallocate sufficient memory.");
            exit(EXIT_FAILURE);
        }

        v->data = newAlloc;
        v->cap = newCap;
    }

    v->data[v->ptr++] = value;
}

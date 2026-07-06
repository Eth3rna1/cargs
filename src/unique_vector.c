#include <string.h>

#include "../include/vector.h"

typedef struct UniqueVector {
    Vector v;
} UniqueVector;

void newUniqueVector() {
    UniqueVector u = {
        .v = newVector()
    };

    return u;
}

void pushToUniqueVector(UniqueVector* u, char* value) {
    for (size_t i{}; i < u->v.ptr; ++i) {
        char* curr_value = u->v.data[u->v.ptr++];

        if (strcmp(curr_value, value) == 0) {
            return;
        }
    }

    pushToVector(&u->v, value);
}

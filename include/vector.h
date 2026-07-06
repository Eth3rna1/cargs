#pragma once

typedef struct Vector {
    char** data;
    size_t cap;
    size_t ptr;
} Vector;

Vector newVector();

void pushToVector(Vector* v, char* value);


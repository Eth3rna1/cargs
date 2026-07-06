#pragma once
#include "./vector.h"

typedef struct UniqueVector {
    Vector v;
} UniqueVector;

void newUniqueVector();

void pushToUniqueVector(UniqueVector* u);

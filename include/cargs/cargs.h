#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef const char* name_t;

typedef enum ArgType {
    STRING,
    BOOLEAN,
    NUMERICAL,
} ArgType;

typedef struct ArgDef {
    name_t name;
    ArgType type;
    bool optional;
} ArgDef;

union Variant {
    char* s;
    float f;
    bool b;
};

typedef struct Value {
    ArgType type;
    union Variant variant;
} Value;

typedef struct Arg {
    name_t name;
    Value value;
} Arg;

ArgDef newArgDef(const char* name, ArgType type, bool optional);

Arg newArg(name_t name, char* v, ArgType type);

Arg* parse(char** argv, int argc, ArgDef* definitions, size_t size);

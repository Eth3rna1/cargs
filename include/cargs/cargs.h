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
    const char* name;
    ArgType type;
    bool optional;
} ArgDef;

typedef struct CArgs {
    ArgDef* args;
    size_t size;
} CArgs;

typedef struct StringArg {
    name_t name;
    char* value;
} StringArg;

typedef struct BooleanArg {
    name_t name;
    bool value;
} BooleanArg;

typedef struct NumericalArg {
    name_t name;
    float value;
} NumericalArg;

typedef struct StringArgCollection {
    StringArg* args;
    size_t size;
} StringArgCollection;

typedef struct BooleanArgCollection {
    BooleanArg* args;
    size_t size;
} BooleanArgCollection;

typedef struct NumericalArgCollection {
    NumericalArg* args;
    size_t size;
} NumericalArgCollection;

typedef struct CArgsResult {
    StringArg* stringArgs;
    BooleanArg* booleanArgs;
    NumericalArg* numericalArgs;
} CArgsResult;


ArgDef newArg(const char* name, ArgType type, bool optional);

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

int getArgDefWithName(const char* arg, const ArgDef* definitions, size_t size);

bool argAlreadyExists(Arg* args, size_t argsSize, const Arg* arg);

bool isArgDefUsed(const ArgDef* def, const Arg* args, size_t argsSize);

Arg parseStringArgDef(char** argv, int argc, const ArgDef* def, size_t pos);

Arg parseBooleanArgDef(char** argv, int argc, const ArgDef* def, size_t pos);

Arg parseNumericalArgDef(char** argv, int argc, const ArgDef* def, size_t pos);

Arg parseDispatch(char** argv, int argc, size_t pos, const ArgDef* def);

Arg* parse(char** argv, int argc, ArgDef* definitions, size_t size);

Arg* getArg(Arg* args, size_t argsSize, name_t argument);

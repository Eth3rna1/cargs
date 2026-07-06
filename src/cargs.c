#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cargs/cargs.h>

ArgDef newArgDef(const char* name, ArgType type, bool optional) {
    ArgDef arg = {
        .name = name,
        .type = type,
        .optional = optional
    };

    return arg;
}


Arg newArg(name_t name, char* v, ArgType type) {
    Value value;
    value.type = type;

    switch (value.type) {
        case STRING:
            value.variant.s = v;
            break;
        case BOOLEAN:
            value.variant.b = true;
            break;
        case NUMERICAL:
            char* end;
            float floatValue = strtof(v, &end);

            if (v == end) {
                printf("There was an error parsing the float number\n");
                break;
            }
            value.variant.f = floatValue;
            break;
    }

    Arg arg = {
        .name = name,
        .value = value
    };

    return arg;
}

size_t getArgDefWithName(const char* arg, const ArgDef* definitions, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        ArgDef def = definitions[i];
        
        if (strcmp(def.name, arg) == 0) return i;
    }

    return -1;
}

bool intInInts(int n, int* ints, size_t size) {
    for (int i = 0; i < size; ++i) {
        if (n == ints[i]) return true;
    }
    return false;
}

Arg* parse(char** argv, int argc, ArgDef* definitions, size_t size) {
    size_t i = 0;

    while (i < argc) {
        char* arg = argv[i];

        // looking for its definition
        size_t idx = findArgDef(arg, definitions, size);

        if (idx == -1) {
            fprintf(stderr, "Unexpected argument given: `%s`\n", argv[idx]);
            exit(EXIT_FAILURE);
        }
        
        switch (definitions[i].type) {
            case STRING:
            if (i + 1 >= argc) {
                fprintf(stderr, "Please provide a string value to: `%s`\n", arg);
                exit(EXIT_FAILURE);
            }
            char* argValue = argv[i + 1];

            Arg a = {
                .name = arg,
                .value = {
                    .type = STRING,
                    .variant.s = argValue
                }
            };
            // do something with `a` here

            i += 2;
            break;
            case BOOLEAN:
            Arg a = {
                .name = arg,
                .value = {
                    .type = BOOLEAN,
                    .variant.b = true;
                }
            }
            // do something with `a` here

            i += 1;
            break;
            case NUMERICAL:
            if (i + 1 >= argc) {
                fprintf(stderr, "Please provide a numerical value to: `%s`\n", arg);
                exit(EXIT_FAILURE);
            }
            char* argValue = argv[i + 1];
            char* end;

            float floatArgValue = strtof(argValue, &end);
            if (end == argValue) {
                fprintf(stderr, "Error parsing numerical value: `%f`\n", argValue);
                exit(EXIT_FAILURE);
            }

            Arg a = {
                .name = arg,
                .value = {
                    .type = BOOLEAN,
                    .variant.s = argValue
                }
            };
            // do something with `a` here

            i += 2;
            break;
        }
    }
}

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

bool isArgsExist(Arg* args, size_t argsS, const Arg* arg) {
    size_t left = 0;
    size_t right = argsS - 1;

    while (left <= right) {
        if (strcmp(args[left].name, arg->name) == 0) return true;
        if (strcmp(args[right].name, arg->name) == 0) return true;
        left++;
        right--;
    }

    return false;
}

// bool valueIsArgDef(ArgDef* definitions, size_t defsSize, const char* value) {
//     size_t left = 0;
//     size_t right = defsSize - 1;
//
//     while (left <= right) {
//         if (strcmp(definitions[left].name, value) == 0) return true;
//         if (strcmp(definitions[right].name, value) == 0) return true;
//         left++;
//         right--;
//     }
//
//     return false;
// }

Arg* parse(char** argv, int argc, ArgDef* definitions, size_t defsSize) {
    // skipping the first argument since its always the program name
    size_t i = 1;

    Arg* args = malloc(sizeof(Arg) * defsSize);
    size_t argsP = 0;

    if (args == NULL) {
        fprintf(stderr, "Could not allocate memory for arguments\n");
        return NULL;
    }

    while (i < argc) {
        char* arg = argv[i];

        if (strcmp(arg, "--") == 0) {
            i += 1;
            continue;
        }

        // looking for its definition
        size_t idx = getArgDefWithName(arg, definitions, defsSize);

        if (idx == -1) {
            fprintf(stderr, "Unexpected argument given: `%s`\n", arg);
            return NULL;
        }
        
        switch (definitions[i].type) {
            case STRING: {
                printf("Entering the STRING branch\n");

                if (i + 1 >= argc) {
                    fprintf(stderr, "Please provide a string value to: `%s`\n", arg);
                    return NULL;
                }
                char* argValue = argv[i + 1];

                printf("Acquired the arguments value: %s\n", argValue);

                bool isNotAnArgument = getArgDefWithName(argValue, definitions, defsSize) != -1;
                bool hasPrecedingDecouplingToken = i != 0 && strcmp(argv[i - 1], "--") != 0;

                if (isNotAnArgument && hasPrecedingDecouplingToken) {
                    // means that the taken value is actually a flag
                    fprintf(
                        stderr,
                        "Please provide a valid value for `%s`, not `%s`. If this is what you wanted, preceed the token with `--` as a token\n",
                        arg,
                        argValue
                    );
                    return NULL;
                }

                Arg a = {
                    .name = arg,
                    .value = {
                        .type = STRING,
                        .variant.s = argValue
                    }
                };

                printf("Constructed the argument object");

                if (!isArgsExist(args, argsP, &a)) {
                    args[argsP++] = a;
                }

                i += 2;
                printf("Skipped the i pointer two times. Should be continuing");
                break;
            }
            case BOOLEAN: {
                Arg a = {
                    .name = arg,
                    .value = {
                        .type = BOOLEAN,
                        .variant.b = true
                    }
                };

                if (!isArgsExist(args, argsP, &a)) {
                    args[argsP++] = a;
                }

                i += 1;
                break;
            }
            case NUMERICAL: {
                if (i + 1 >= argc) {
                    fprintf(stderr, "Please provide a numerical value to: `%s`\n", arg);
                    return NULL;
                }
                char* argValue = argv[i + 1];
                char* end;

                float floatArgValue = strtof(argValue, &end);
                if (end == argValue) {
                    fprintf(stderr, "Error parsing numerical value: `%f`\n", argValue);
                    return NULL;
                }

                if ((getArgDefWithName(argValue, definitions, defsSize) != -1) && (i != 0 && strcmp(argv[i - 1], "--") != 0)) {
                    // means that the taken value is actually a flag
                    fprintf(
                        stderr,
                        "Please provide a valid value for `%s`, not `%d`. If this is what you wanted, preceed the token with `--` as a token\n",
                        arg,
                        floatArgValue
                    );
                    return NULL;
                }

                Arg a = {
                    .name = arg,
                    .value = {
                        .type = BOOLEAN,
                        .variant.s = argValue
                    }
                };

                if (!isArgsExist(args, argsP, &a)) {
                    args[argsP++] = a;
                }

                i += 2;
                break;
            }
        }
    }

    // looking for argument definitions that were deemed as not optional
    for (size_t i = 0; i < defsSize; ++i) {
        // if (!definitions[i].optional && getArgDefWithName())
    }
}

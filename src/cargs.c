#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cargs.h>


#define TERMINATE exit(EXIT_FAILURE)


ArgDef newArgDef(const char* name, ArgType type, bool optional) {
    ArgDef arg = {
        .name = name,
        .type = type,
        .optional = optional
    };

    return arg;
}

int getArgDefWithName(const char* arg, const ArgDef* definitions, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        ArgDef def = definitions[i];
        
        if (strcmp(def.name, arg) == 0) return i;
    }

    return -1;
}

bool argAlreadyExists(Arg* args, size_t argsSize, const Arg* arg) {
    for (size_t i = 0; i < argsSize; ++i) {
        if (strcmp(args[i].name, arg->name) == 0) return true;
    }

    return false;
}

bool isArgDefUsed(const ArgDef* def, const Arg* args, size_t argsSize) {
    for (size_t i = 0; i < argsSize; ++i) {
        if (strcmp(args[i].name, def->name) == 0) return true;
    }
    
    return false;
}

Arg parseStringArgDef(char** argv, int argc, const ArgDef* def, size_t pos) {
    if (pos + 1 >= argc) {
        fprintf(stderr, "Please provide a string value to: `%s`\n", def->name);
        TERMINATE;
    }

    char* argValue = argv[pos + 1];

    Arg a = {
        .name = def->name,
        .value = {
            .type = STRING,
            .variant.s = argValue
        }
    };

    return a;
}

Arg parseBooleanArgDef(char** argv, int argc, const ArgDef* def, size_t pos) {
    Arg a = {
        .name = def->name,
        .value = {
            .type = BOOLEAN,
            .variant.b = true
        }
    };

    return a;
}

Arg parseIntegerArgDef(char** argv, int argc, const ArgDef* def, size_t pos) {
    if (pos + 1 >= (size_t)argc) {
        fprintf(stderr, "Please provide an integer value to: `%s`\n", def->name);
        TERMINATE;
    }
    char* argValue = argv[pos + 1];
    char* end;

    errno = 0;

    long integerArgValue = strtol(argValue, &end, 10);

    bool isUnderflowed = integerArgValue < INT_MIN;
    bool isOverflowed = integerArgValue > INT_MAX;
    bool isOutOfIntRange = isUnderflowed || isOverflowed;
    bool isRangeError = errno == ERANGE;

    if (isRangeError) {
        fprintf(stderr, "Failed to parse argument value: `%s`\n\nValue is either too big or too small\n", argValue);
        TERMINATE;
    }

    if (argValue == end) {
        fprintf(stderr, "No valid digits found for parsing: %s\n", argValue);
        TERMINATE;
    }

    if (*end != '\0') {
        fprintf(stderr, "Error parsing integer value: `%s`\n", argValue);
        TERMINATE;
    }

    if (isOutOfIntRange) {
        fprintf(stderr, "Failed to parse argument value: `%s`\n\nValue is either too big or too small\n", argValue);
        TERMINATE;
    }

    int intResult = (int)integerArgValue;

    Arg a = {
        .name = def->name,
        .value = {
            .type = INTEGER,
            .variant.i = intResult 
        }
    };

    return a;
}

Arg parseFloatArgDef(char** argv, int argc, const ArgDef* def, size_t pos) {
    if (pos + 1 >= argc) {
        fprintf(stderr, "Please provide a decimal value to: `%s`\n", def->name);
        TERMINATE;
    }
    char* argValue = argv[pos + 1];
    char* end;

    errno = 0;
    float floatArgValue = strtof(argValue, &end);

    if (argValue == end) {
        fprintf(
            stderr,
            "No valid decimal number found for %s: `%s`",
            argv[pos],
            argValue
        );
        TERMINATE;
    }

    if (*end != '\0') {
        fprintf(stderr, "Error parsing float value: `%s`\n", argValue);
        TERMINATE;
    }

    if (errno == ERANGE) {
        fprintf(stderr, "Decimal value out of range: `%s`\n", argValue);
        TERMINATE;
    }

    Arg a = {
        .name = def->name,
        .value = {
            .type = FLOAT,
            .variant.f = floatArgValue
        }
    };

    return a;
}

Arg parseDispatch(char** argv, int argc, size_t pos, const ArgDef* def) {
    Arg a;

    switch (def->type) {
        case STRING: a = parseStringArgDef(argv, argc, def, pos); break;
        case FLOAT: a = parseFloatArgDef(argv, argc, def, pos); break;
        case INTEGER: a = parseIntegerArgDef(argv, argc, def, pos); break;
        case BOOLEAN: a = parseBooleanArgDef(argv, argc, def, pos); break;
    }

    return a;
}

Arg* parse(char** argv, int argc, ArgDef* definitions, size_t defsSize) {
    // skipping the first argument since its always the program name
    size_t i = 1;

    Arg* args = malloc(sizeof(Arg) * defsSize);
    size_t argsP = 0;

    if (args == NULL) {
        fprintf(stderr, "Could not allocate memory for arguments\n");
        return NULL;
    }

    while (i < (size_t)argc) {
        char* arg = argv[i];

        // looking for its definition
        int idx = getArgDefWithName(arg, definitions, defsSize);

        if (idx == -1) {
            fprintf(stderr, "Unexpected argument given: `%s`\n", arg);
            goto cleanup;
        }

        const ArgDef* def = &definitions[idx];

        Arg a = parseDispatch(argv, argc, i, def);

        // checking for duplicates
        if (argAlreadyExists(args, argsP, &a)) {
            fprintf(stderr, "Found duplicate argument: `%s`", a.name);
            goto cleanup;
        }

        args[argsP++] = a;

        i += (def->type == BOOLEAN) ? 1 : 2;
    }


    // looking for argument definitions that were deemed as not optional
    for (size_t i = 0; i < defsSize; ++i) {
        ArgDef def = definitions[i];
        
        bool mandatory = !def.optional;
        bool wasNotUsed = !isArgDefUsed(&def, args, argsP);

        if (mandatory && wasNotUsed) {
            fprintf(stderr, "No argument provided for: `%s`\n", def.name);
            goto cleanup;
        }
    }

    return args;

cleanup:
    free(args);
    return NULL;
}


// post parsing, given the result
// pointer `Arg*`, retrieves the desired
// `Arg` given the name of such argument
Arg* getArg(Arg* args, size_t argsSize, name_t argument) {
    for (int i = 0; i < argsSize; ++i) {
        if (strcmp(args[i].name, argument) == 0) return &args[i];
    }

    return NULL;
}

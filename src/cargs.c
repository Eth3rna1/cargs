#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cargs/cargs.h>


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

bool intInInts(int n, int* ints, size_t size) {
    for (int i = 0; i < size; ++i) {
        if (n == ints[i]) return true;
    }
    return false;
}

bool argAlreadyExists(Arg* args, size_t argsS, const Arg* arg) {
    size_t left = 0;
    
    if (argsS == 0) return false;

    size_t right = argsS - 1;

    while (left <= right) {
        if (strcmp(args[left].name, arg->name) == 0) return true;
        if (strcmp(args[right].name, arg->name) == 0) return true;
        left++;
        right--;
    }

    return false;
}

bool isArgDefUsed(const ArgDef* def, const Arg* args, size_t argsSize) {
    size_t left = 0;

    if (argsSize == 0) return false;

    size_t right = argsSize - 1;

    while (left <= right) {
        if (strcmp(args[left++].name, def->name) == 0) return true;
        if (strcmp(args[right--].name, def->name) == 0) return true;
    }

    return false;
}

Arg parseStringArgDef(char** argv, int argc, const ArgDef* def, size_t pos) {
    if (pos + 1 >= argc) {
        fprintf(stderr, "Please provide a string value to: `%s`\n", arg);
        TERMINATE;
    }

    char* argValue = argv[pos + 1];

    bool valueIsAnArgument = getArgDefWithName(argValue, definitions, defsSize) != -1;

    if (valueIsAnArgument) {
        fprintf(
            stderr,
            "Please provide a valid value for `%s`, not `%s`. If this is what you wanted, precede the token with `--` as a token\n",
            argName,
            argValue
        );
        TERMINATE;
    }

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
        .name = argName,
        .value = {
            .type = BOOLEAN,
            .variant.b = true
        }
    };

    return a;
}

Arg parseNumericalArgDef(char** argv, int argc, const ArgDef* def, size_t pos) {
    if (pos + 1 >= argc) {
        fprintf(stderr, "Please provide a numerical value to: `%s`\n", arg);
        TERMINATE;
    }
    char* argValue = argv[pos + 1];
    char* end;

    float floatArgValue = strtof(argValue, &end);

    /*if (end == argValue) {*/
    if (*end != '\0') {
        fprintf(stderr, "Error parsing numerical value: `%s`\n", argValue);
        TERMINATE;
    }

    Arg a = {
        .name = def->name,
        .value = {
            .type = NUMERICAL,
            .variant.f = floatArgValue
        }
    };

    return a;
}

Arg parseDispatch(char** argv, int argc, const ArgDef* definitions, size_t defsSize, size_t pos) {
    Arg a;
    ArgDef def = definitions[i];

    switch (def.type) {
        case STRING: a = parseStringArgDef(argv, argc, definitions, defsSize, def->name, pos); break;
        case BOOLEAN: a = parseBooleanArgDef(argv, argc, definitions, defsSize, def->name, pos); break;
        case NUMERICAL: a = parseNumericalArgDef(argv, argc, definitions, defsSize, def->name, pos); break;
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

    while (i < argc) {
        char* arg = argv[i];

        printf("%s\n", arg);

        // looking for its definition
        int idx = getArgDefWithName(arg, definitions, defsSize);

        if (idx == -1) {
            fprintf(stderr, "Unexpected argument given: `%s`\n", arg);
            return NULL;
        }

        Arg a = parseDispatch(argv, argc, definitions, defsSize, i);

        if (!argAlreadyExists(args, argsP, &a)) args[argsP++] = a;

        i += (def.type == BOOLEAN) ? 1 : 2;
    }


    // looking for argument definitions that were deemed as not optional
    for (size_t i = 0; i < defsSize; ++i) {
        ArgDef def = definitions[i];
        
        bool mandatory = !def.optional;
        bool wasNotUsed = !isArgDefUsed(&def, args, argsP);

        if (mandatory && wasNotUsed) {
            fprintf(stderr, "No argument provided for: `%s`\n", def.name);
            return NULL;
        }
    }

    return args;
}

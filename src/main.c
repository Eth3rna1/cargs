#include <stdio.h>
#include <stdlib.h>
#include <cargs.h>

int main(int argc, char** argv) {
    const size_t size = 4;
    ArgDef definitions[4] = {
        newArgDef("--name", STRING, false),
        newArgDef("--okay", BOOLEAN, false),
        newArgDef("--age", INTEGER, false),
        newArgDef("--pi", FLOAT, false)
    };

    Arg* args = parse(argv, argc, definitions, size);

    Arg* arg = getArg(args, size, "--name");
    Arg* b_arg = getArg(args, size, "--okay");
    Arg* i_arg = getArg(args, size, "--age");
    Arg* f_arg = getArg(args, size, "--pi");

    if (arg != NULL) {
        printf("Argument value for %s: \"%s\"\n", arg->name, arg->value.variant.s);
    }
    if (b_arg != NULL) {
        char* value = b_arg->value.variant.b ? "true" : "false";
        printf("Argument value for %s: \"%s\"\n", b_arg->name, value);
    }
    if (i_arg != NULL) {
        printf("Argument value for %s: \"%d\"\n", i_arg->name, i_arg->value.variant.i);
    }
    if (f_arg != NULL) {
        printf("Argument value for %s: \"%f\"\n", f_arg->name, f_arg->value.variant.f);
    }

    free(args);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <cargs.h>

int main(int argc, char** argv) {
    const size_t size = 2;
    ArgDef definitions[2] = {
        newArgDef("--name", STRING, false),
        newArgDef("--okay", BOOLEAN, true)
    };

    Arg* args = parse(argv, argc, definitions, size);

    Arg* arg = getArg(args, size, "--name");
    Arg* b_arg = getArg(args, size, "--okay");

    if (arg != NULL) {
        printf("Argument value for %s: \"%s\"\n", arg->name, arg->value.variant.s);
    }

    // const char* booleanValue = b_arg->value.variant.b == 1 ? "true" : "false";
    const char* booleanFlagName = "--okay";
    const char* booleanValue = b_arg == NULL ? "false" : "true";
    // printf("argument value for %s: \"%s\"\n", b_arg->name, booleanValue);
    printf("argument value for %s: \"%s\"\n", booleanFlagName, booleanValue);

    free(args);
    free(arg);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <cargs/cargs.h>

int main(int argc, char** argv) {
    const size_t size = 2;
    ArgDef definitions[2] = {
        newArgDef("--name", STRING, false),
        newArgDef("--okay", BOOLEAN, false)
    };

    Arg* args = parse(argv, argc, definitions, size);

    Arg arg = args[0];

    printf("Argument value for %s: %s", arg.name, arg.value.variant.s);

    free(args);

    return 0;
}

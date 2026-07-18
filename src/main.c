#include <stdio.h>
#include <cargs/cargs.h>

int main(int argc, char** argv) {
    ArgDef definitions[1] = {
        newArgDef("--name", STRING, false)
    };

    Arg* args = parse(argv, argc, definitions, 1);

    Arg arg = args[0];

    printf("Argument value for %s: %s", arg.name, arg.value.variant.s);
    // free(args);
    return 0;
}

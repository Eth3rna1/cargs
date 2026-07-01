#include <stddef.h>
#include <cargs/cargs.h>

ArgDef newArg(const char* name, ArgType type, bool optional) {
    ArgDef arg = {
        .name = name,
        .type = type,
        .optional = optional
    };

    return arg;
}

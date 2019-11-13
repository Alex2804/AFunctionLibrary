#ifndef AFUNCTIONLIBRARY_MACROS_H
#define AFUNCTIONLIBRARY_MACROS_H

#include "definitions.h"
#include "cstringtoken.h"

#define __AFL_REGISTER_CREATE_TOKEN_FEATURE__(feature_group, feature_name, parameter_name)                             \
    A_PLUGIN_REGISTER_FEATURE(afl::CStringToken*, feature_group, feature_name, const char* parameter_name)


#define __AFL_REGISTER_CREATE_TOKEN_ALIASES_FEATURE__(feature_group, feature_name, value_name, aliases_type_name)      \
    A_PLUGIN_REGISTER_FEATURE(afl::CStringTokenAliases*, feature_group, feature_name,                                  \
                              const char* value_name, afl::TokenAliasType aliases_type_name)

#endif //AFUNCTIONLIBRARY_MACROS_H

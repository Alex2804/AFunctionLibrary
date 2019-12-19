#ifndef AFUNCTIONLIBRARY_CREATETOKENAPI_PRIVATE_HPP
#define AFUNCTIONLIBRARY_CREATETOKENAPI_PRIVATE_HPP

#include "../../APluginSDK/pluginapi.h"

#include "createtokenapi_definitions.hpp"
#include "../cstringtoken.hpp"

#define __AFL_C_API_REGISTER_CREATE_TOKEN_FEATURE__(feature_group, feature_name, value_name)                           \
    A_PLUGIN_REGISTER_FEATURE(afl::CStringToken*, feature_group, feature_name, const char* value_name)
#define __AFL_CPP_API_REGISTER_CREATE_TOKEN_FEATURE__(feature_group, feature_name, value_name)                         \
    A_PLUGIN_REGISTER_FEATURE(std::shared_ptr<const afl::Token<std::string>>, feature_group, feature_name,             \
                              const std::string& value_name)

#define __AFL_C_API_REGISTER_CREATE_TOKEN_ALIASES_FEATURE__(feature_group, feature_name, value_name, aliases_type_name)\
    A_PLUGIN_REGISTER_FEATURE(afl::CStringTokenAliases*, feature_group, feature_name,                                  \
                              const char* value_name, afl::TokenAliasType aliases_type_name)
#define __AFL_CPP_API_REGISTER_CREATE_TOKEN_ALIASES_FEATURE__(feature_group, feature_name, value_name)                 \
    A_PLUGIN_REGISTER_FEATURE(std::vector<afl::TokenAliases<std::string>>, feature_group, feature_name,                \
                              const std::string& value_name)

#endif //AFUNCTIONLIBRARY_CREATETOKENAPI_PRIVATE_HPP

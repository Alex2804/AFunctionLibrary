#include "APluginSDK/pluginapi.h"

#include "implementation/macros.h"

#define AFL_REGISTER_CREATE_TOKEN_FEATURE(feature_name, token_value_parameter_name)                                    \
    __AFL_REGISTER_CREATE_TOKEN_FEATURE__(AFL_CREATE_TOKEN_FEATURE_GROUP_NAME, feature_name,                           \
                                          token_value_parameter_name)

#define AFL_REGISTER_CREATE_TOKEN_ALIASES_FEATURE(feature_name, value_name, aliases_type_name)                         \
    __AFL_REGISTER_CREATE_TOKEN_ALIASES_FEATURE__(AFL_CREATE_TOKEN_ALIASES_FEATURE_GROUP_NAME,                         \
                                                  feature_name, value_name, aliases_type_name)

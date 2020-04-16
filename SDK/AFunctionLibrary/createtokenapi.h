#ifndef AFUNCTIONLIBRARYSDK_CREATETOKENAPI_H
#define AFUNCTIONLIBRARYSDK_CREATETOKENAPI_H

#include "AFunctionLibrary/implementation/createtokenapi/createtokenapi_private.hpp"

#define AFL_REGISTER_CREATE_TOKEN_FEATURE(feature_name, token_value_parameter_name)                                    \
    AFL_C_API_REGISTER_CREATE_TOKEN_FEATURE(feature_name, token_value_parameter_name)
#define AFL_C_API_REGISTER_CREATE_TOKEN_FEATURE(feature_name, token_value_parameter_name)                              \
    __AFL_C_API_REGISTER_CREATE_TOKEN_FEATURE__(__AFL_C_API_CREATE_TOKEN_FEATURE_GROUP_NAME__, feature_name,           \
                                                token_value_parameter_name)
#define AFL_CPP_API_REGISTER_CREATE_TOKEN_FEATURE(feature_name, token_value_parameter_name)                            \
    __AFL_CPP_API_REGISTER_CREATE_TOKEN_FEATURE__(__AFL_CPP_API_CREATE_TOKEN_FEATURE_GROUP_NAME__, feature_name,       \
                                                  token_value_parameter_name)


#define AFL_REGISTER_CREATE_TOKEN_ALIASES_FEATURE(feature_name, value_name, aliases_type_name)                         \
    AFL_C_API_REGISTER_CREATE_TOKEN_ALIASES_FEATURE(feature_name, value_name, aliases_type_name)
#define AFL_C_API_REGISTER_CREATE_TOKEN_ALIASES_FEATURE(feature_name, value_name, aliases_type_name)                   \
    __AFL_C_API_REGISTER_CREATE_TOKEN_ALIASES_FEATURE__(__AFL_C_API_CREATE_TOKEN_ALIASES_FEATURE_GROUP_NAME_,          \
                                                        feature_name, value_name, aliases_type_name)
#define AFL_CPP_API_REGISTER_CREATE_TOKEN_ALIASES_FEATURE(feature_name, value_name)                                    \
    __AFL_CPP_API_REGISTER_CREATE_TOKEN_ALIASES_FEATURE__(__AFL_CPP_API_CREATE_TOKEN_ALIASES_FEATURE_GROUP_NAME_,      \
                                                          feature_name, value_name)

#endif //AFUNCTIONLIBRARYSDK_CREATETOKENAPI_H

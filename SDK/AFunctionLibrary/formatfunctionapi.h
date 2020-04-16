#ifndef AFUNCTIONLIBRARYSDK_FORMATFUNCTIONAPI_H
#define AFUNCTIONLIBRARYSDK_FORMATFUNCTIONAPI_H

#include "AFunctionLibrary/implementation/formatfunctionapi/formatfunctionapi_private.hpp"

#define AFL_REGISTER_FORMAT_FUNCTION_STRING_FEATURE(feature_name, function_string_parameter_name)                      \
    AFL_C_REGISTER_FORMAT_FUNCTION_STRING_FEATURE(feature_name, function_string_parameter_name)
#define AFL_C_REGISTER_FORMAT_FUNCTION_STRING_FEATURE(feature_name, function_string_parameter_name)                    \
    __AFL_C_API_REGISTER_FORMAT_FUNCTION_STRING_FEATURE__(__AFL_C_API_FORMAT_FUNCTION_STRING_FEATURE_GROUP_NAME__,     \
                                                          feature_name, function_string_parameter_name)
#define AFL_CPP_REGISTER_FORMAT_FUNCTION_STRING_FEATURE(feature_name, function_string_parameter_name)                  \
    __AFL_CPP_API_REGISTER_FORMAT_FUNCTION_STRING_FEATURE__(__AFL_CPP_API_FORMAT_FUNCTION_STRING_FEATURE_GROUP_NAME__, \
                                                            feature_name, function_string_parameter_name)

#endif //AFUNCTIONLIBRARYSDK_FORMATFUNCTIONAPI_H

#ifndef AFUNCTIONLIBRARYSDK_OPTIMIZEFUNCTIONAPI_H
#define AFUNCTIONLIBRARYSDK_OPTIMIZEFUNCTIONAPI_H

#include "AFunctionLibrary/implementation/optimizefunctionapi/optimizefunctionapi_private.hpp"

#define AFL_REGISTER_OPTIMIZE_FUNCTION_STRING_FEATURE(feature_name, function_string_parameter_name)                    \
    AFL_C_API_REGISTER_OPTIMIZE_FUNCTION_STRING_FEATURE(feature_name, function_string_parameter_name)
#define AFL_C_API_REGISTER_OPTIMIZE_FUNCTION_STRING_FEATURE(feature_name, function_string_parameter_name)                  \
    __AFL_C_API_REGISTER_OPTIMIZE_FUNCTION_STRING_FEATURE__(__AFL_C_API_OPTIMIZE_FUNCTION_STRING_FEATURE_GROUP_NAME__, \
                                                          feature_name, function_string_parameter_name)
#define AFL_CPP_API_REGISTER_OPTIMIZE_FUNCTION_STRING_FEATURE(feature_name, function_string_parameter_name)                    \
    __AFL_CPP_API_REGISTER_OPTIMIZE_FUNCTION_STRING_FEATURE__(__AFL_CPP_API_OPTIMIZE_FUNCTION_STRING_FEATURE_GROUP_NAME__, \
                                                            feature_name, function_string_parameter_name)

#define AFL_REGISTER_OPTIMIZE_FUNCTION_SYNTAX_TREE_FEATURE(feature_name, function_syntax_tree_parameter_name)          \
    AFL_C_API_REGISTER_OPTIMIZE_FUNCTION_SYNTAX_TREE_FEATURE(feature_name, function_syntax_tree_parameter_name)
#define AFL_C_API_REGISTER_OPTIMIZE_FUNCTION_SYNTAX_TREE_FEATURE(feature_name, function_syntax_tree_parameter_name)                  \
    __AFL_C_API_REGISTER_OPTIMIZE_FUNCTION_SYNTAX_TREE_FEATURE__(__AFL_C_API_OPTIMIZE_FUNCTION_SYNTAX_TREE_FEATURE_GROUP_NAME__, \
                                                          feature_name, function_syntax_tree_parameter_name)
#define AFL_CPP_API_REGISTER_OPTIMIZE_FUNCTION_SYNTAX_TREE_FEATURE(feature_name, function_syntax_tree_parameter_name)                    \
    __AFL_CPP_API_REGISTER_OPTIMIZE_FUNCTION_SYNTAX_TREE_FEATURE__(__AFL_CPP_API_OPTIMIZE_FUNCTION_SYNTAX_TREE_FEATURE_GROUP_NAME__, \
                                                            feature_name, function_syntax_tree_parameter_name)

#endif //AFUNCTIONLIBRARYSDK_OPTIMIZEFUNCTIONAPI_H

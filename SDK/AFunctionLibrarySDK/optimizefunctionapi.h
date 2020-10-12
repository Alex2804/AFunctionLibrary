#ifndef AFUNCTIONLIBRARYSDK_OPTIMIZEFUNCTIONAPI_H
#define AFUNCTIONLIBRARYSDK_OPTIMIZEFUNCTIONAPI_H

#include "libs/APluginSDK/pluginapi.h"

#include "include/AFunctionLibrarySDK/atokennode.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE
    const char* k_OptimizeFunctionStringFeatureGroupName = "_afl_optimize_function_string_";
    const char* k_OptimizeFunctionSyntaxTreeFeatureGroupName = "_afl_optimize_function_syntax_tree_";
PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#define AFL_REGISTER_OPTIMIZE_FUNCTION_STRING_FEATURE(feature_name, function_string_parameter_name)                    \
    A_PLUGIN_REGISTER_FEATURE(struct AString*, _afl_optimize_function_string_, feature_name,                           \
                              const struct AString* function_string_parameter_name)

#define AFL_REGISTER_OPTIMIZE_FUNCTION_SYNTAX_TREE_FEATURE(feature_name, token_node_parameter_name)                    \
    A_PLUGIN_REGISTER_FEATURE(struct ATokenNode*, _afl_optimize_function_syntax_tree_, feature_name,                   \
                              const struct ATokenNode* token_node_parameter_name)

#endif /* AFUNCTIONLIBRARYSDK_OPTIMIZEFUNCTIONAPI_H */

#ifndef AFUNCTIONLIBRARYSDK_FORMATFUNCTIONAPI_H
#define AFUNCTIONLIBRARYSDK_FORMATFUNCTIONAPI_H

#include "libs/APluginSDK/pluginapi.h"
#include "libs/ACUtils/include/ACUtils/astring.h"

#include "private/macros.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE
    const char* k_FormatFunctionStringFeatureGroupName = "_afl_format_function_string_";
PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#define AFL_REGISTER_FORMAT_FUNCTION_STRING_FEATURE(feature_name, function_string_parameter_name)                      \
    A_PLUGIN_REGISTER_FEATURE(struct AString*, _afl_format_function_string_, feature_name,                             \
                              const struct AString* function_string_parameter_name)

#endif /* AFUNCTIONLIBRARYSDK_FORMATFUNCTIONAPI_H */

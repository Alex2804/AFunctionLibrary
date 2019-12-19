#ifndef AFUNCTIONLIBRARY_FORMATFUNCTIONAPI_PRIVATE_HPP
#define AFUNCTIONLIBRARY_FORMATFUNCTIONAPI_PRIVATE_HPP

#include "../../APluginSDK/pluginapi.h"

#include "formatfunctionapi_definitions.hpp"
#include "../cstring.hpp"

#define __AFL_C_API_REGISTER_FORMAT_FUNCTION_STRING_FEATURE__(feature_group, feature_name, value_name)                 \
    A_PLUGIN_REGISTER_FEATURE(afl::CString*, feature_group, feature_name, const char* value_name)
#define __AFL_CPP_API_REGISTER_FORMAT_FUNCTION_STRING_FEATURE__(feature_group, feature_name, value_name)               \
    A_PLUGIN_REGISTER_FEATURE(std::string, feature_group, feature_name, std::string value_name)

#endif //AFUNCTIONLIBRARY_FORMATFUNCTIONAPI_PRIVATE_HPP

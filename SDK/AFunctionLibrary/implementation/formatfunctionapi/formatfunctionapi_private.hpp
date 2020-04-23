#ifndef AFUNCTIONLIBRARYSDK_FORMATFUNCTIONAPI_PRIVATE_HPP
#define AFUNCTIONLIBRARYSDK_FORMATFUNCTIONAPI_PRIVATE_HPP

#include "../../libs/APluginSDK/pluginapi.h"

#include "formatfunctionapi_definitions.hpp"
#include "../cstring.hpp"

#define __AFL_C_API_REGISTER_FORMAT_FUNCTION_STRING_FEATURE__(feature_group, feature_name, parameter_name)             \
    A_PLUGIN_REGISTER_FEATURE(afl::CString*, feature_group, feature_name, const char* parameter_name)
#define __AFL_CPP_API_REGISTER_FORMAT_FUNCTION_STRING_FEATURE__(feature_group, feature_name, parameter_name)           \
    A_PLUGIN_REGISTER_FEATURE(std::string, feature_group, feature_name, std::string parameter_name)

#endif //AFUNCTIONLIBRARYSDK_FORMATFUNCTIONAPI_PRIVATE_HPP

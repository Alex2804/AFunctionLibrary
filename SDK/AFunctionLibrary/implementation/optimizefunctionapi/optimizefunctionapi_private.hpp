#ifndef AFUNCTIONLIBRARYSDK_OPTIMIZEFUNCTIONAPI_PRIVATE_HPP
#define AFUNCTIONLIBRARYSDK_OPTIMIZEFUNCTIONAPI_PRIVATE_HPP

#include "../../libs/APluginSDK/pluginapi.h"

#include "optimizefunctionapi_definitions.hpp"
#include "../cstring.hpp"
#include "../cstringtokensyntaxtree.hpp"

#define __AFL_C_API_REGISTER_OPTIMIZE_FUNCTION_STRING_FEATURE__(feature_group, feature_name, parameter_name)           \
    A_PLUGIN_REGISTER_FEATURE(afl::CString*, feature_group, feature_name, const char* parameter_name)
#define __AFL_CPP_API_REGISTER_OPTIMIZE_FUNCTION_STRING_FEATURE__(feature_group, feature_name, parameter_name)         \
    A_PLUGIN_REGISTER_FEATURE(std::string, feature_group, feature_name, std::string parameter_name)

#define __AFL_C_API_REGISTER_OPTIMIZE_FUNCTION_SYNTAX_TREE_FEATURE__(feature_group, feature_name, parameter_name)      \
    A_PLUGIN_REGISTER_FEATURE(afl::CStringTokenGroupNode*, feature_group, feature_name, const afl::CStringTokenGroupNode* parameter_name)
#define __AFL_CPP_API_REGISTER_OPTIMIZE_FUNCTION_SYNTAX_TREE_FEATURE__(feature_group, feature_name, parameter_name)    \
    A_PLUGIN_REGISTER_FEATURE(afl::SyntaxTree<afl::TokenGroup<std::string>>, feature_group, feature_name, afl::SyntaxTree<afl::TokenGroup<std::string>> parameter_name)

#endif //AFUNCTIONLIBRARYSDK_OPTIMIZEFUNCTIONAPI_PRIVATE_HPP

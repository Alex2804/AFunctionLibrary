#ifndef AFUNCTIONLIBRARY_FORMATFUNCTIONAPI_DEFINITIONS_HPP
#define AFUNCTIONLIBRARY_FORMATFUNCTIONAPI_DEFINITIONS_HPP

namespace afl
{
    constexpr const char* k_C_API_FormatFunctionStringFeatureGroupName = "__afl_c_api_format_function_string__";
#define __AFL_C_API_FORMAT_FUNCTION_STRING_FEATURE_GROUP_NAME__ __afl_c_api_format_function_string__
    constexpr const char* k_C_API_FormatFunctionStringDefaultFeatureName = "__afl_c_api_format_function_string_default_feature__";
#define __AFL_C_API_FORMAT_FUNCTION_STRING_DEFAULT_FEATURE_NAME__ __afl_c_api_format_function_string_default_feature__
    constexpr const char* k_CPP_API_FormatFunctionStringFeatureGroupName = "__afl_cpp_api_format_function_string__";
#define __AFL_CPP_API_FORMAT_FUNCTION_STRING_FEATURE_GROUP_NAME__ __afl_cpp_api_format_function_string__
    constexpr const char* k_CPP_API_FormatFunctionStringDefaultFeatureName = "__afl_cpp_api_format_function_string_default_feature__";
#define __AFL_CPP_API_FORMAT_FUNCTION_STRING_DEFAULT_FEATURE_NAME__ __afl_cpp_api_format_function_string_default_feature__
}

#endif //AFUNCTIONLIBRARY_FORMATFUNCTIONAPI_DEFINITIONS_HPP
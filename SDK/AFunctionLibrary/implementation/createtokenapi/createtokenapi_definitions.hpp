#ifndef AFUNCTIONLIBRARY_CREATETOKENAPI_DEFINITIONS_HPP
#define AFUNCTIONLIBRARY_CREATETOKENAPI_DEFINITIONS_HPP

namespace afl
{
    constexpr const char* k_C_API_CreateTokenFeatureGroupName = "__afl_c_api_create_token__";
#define __AFL_C_API_CREATE_TOKEN_FEATURE_GROUP_NAME__ __afl_c_api_create_token__
    constexpr const char* k_C_API_CreateTokenDefaultFeatureName = "__afl_c_api_create_token_default_feature__";
#define __AFL_C_API_CREATE_TOKEN_DEFAULT_FEATURE_NAME__ __afl_c_api_create_token_default_feature__
    constexpr const char* k_CPP_API_CreateTokenFeatureGroupName = "__afl_cpp_api_create_token__";
#define __AFL_CPP_API_CREATE_TOKEN_FEATURE_GROUP_NAME__ __afl_cpp_api_create_token__
    constexpr const char* k_CPP_API_CreateTokenDefaultFeatureName = "__afl_cpp_api_create_token_default_feature__";
#define __AFL_CPP_API_CREATE_TOKEN_DEFAULT_FEATURE_NAME__ __afl_cpp_api_create_token_default_feature__

    constexpr const char* k_C_API_CreateTokenAliasesFeatureGroupName = "__afl_c_api_create_token_aliases__";
#define __AFL_C_API_CREATE_TOKEN_ALIASES_FEATURE_GROUP_NAME_ __afl_c_api_create_token_aliases__
    constexpr const char* k_C_API_CreateTokenAliasesDefaultFeatureName = "__afl_c_api_create_token_aliases_default_feature__";
#define __AFL_C_API_CREATE_TOKEN_ALIASES_DEFAULT_FEATURE_NAME__ __afl_c_api_create_token_aliases_default_feature__
    constexpr const char* k_CPP_API_CreateTokenAliasesFeatureGroupName = "__afl_cpp_api_create_token_aliases__";
#define __AFL_CPP_API_CREATE_TOKEN_ALIASES_FEATURE_GROUP_NAME_ __afl_cpp_api_create_token_aliases__
    constexpr const char* k_CPP_API_CreateTokenAliasesDefaultFeatureName = "__afl_cpp_api_create_token_aliases_default_feature__";
#define __AFL_CPP_API_CREATE_TOKEN_ALIASES_DEFAULT_FEATURE_NAME__ __afl_cpp_api_create_token_aliases_default_feature__
}

#endif //AFUNCTIONLIBRARY_CREATETOKENAPI_DEFINITIONS_HPP

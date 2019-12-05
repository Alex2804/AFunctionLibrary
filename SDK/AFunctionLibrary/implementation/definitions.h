#ifndef AFUNCTIONLIBRARY_DEFINITIONS_H
#define AFUNCTIONLIBRARY_DEFINITIONS_H

namespace afl
{
    // create token feature
    constexpr const char *kCreateTokenFeatureGroupName = "afl_create_token";
#define AFL_CREATE_TOKEN_FEATURE_GROUP_NAME afl_create_token
    constexpr const char *kCreateTokenDefaultFeatureName = "afl_create_token_default_feature";
#define AFL_CREATE_TOKEN_DEFAULT_FEATURE_NAME afl_create_token_default_feature

    // create token aliases feature
    constexpr const char *kCreateTokenAliasesFeatureGroupName = "afl_create_token_aliases";
#define AFL_CREATE_TOKEN_ALIASES_FEATURE_GROUP_NAME afl_create_token_aliases
    constexpr const char *kCreateTokenAliasesDefaultFeatureName = "afl_create_token_aliases_default_feature";
#define AFL_CREATE_TOKEN_ALIASES_DEFAULT_FEATURE_NAME afl_create_token_aliases_default_feature

    // format function feature
    constexpr const char *kFormatFunctionFeatureGroupName = "afl_format_function";
#define AFL_FORMAT_FUNCTION_FEATURE_GROUP_NAME afl_format_function
    constexpr const char *kFormatFunctionDefaultFeatureName = "afl_format_function_default_feature";
#define AFL_FORMAT_FUNCTION_DEFAULT_FEATURE_NAME afl_format_function_default_feature
}

#endif //AFUNCTIONLIBRARY_DEFINITIONS_H

#ifndef AFUNCTIONLIBRARY_DEFINITIONS_H
#define AFUNCTIONLIBRARY_DEFINITIONS_H

namespace afl
{
#define AFL_CREATE_TOKEN_FEATURE_GROUP_NAME afl_create_token
    constexpr const char *kCreateTokenFeatureGroupName = "afl_create_token";

#define AFL_CREATE_TOKEN_ALIASES_FEATURE_GROUP_NAME afl_create_token_aliases
    constexpr const char *kCreateTokenAliasesFeatureGroupName = "afl_create_token_aliases";

#define AFL_CREATE_TOKEN_DEFAULT_FEATURE_NAME afl_create_token_default_feature
#define AFL_CREATE_TOKEN_ALIASES_DEFAULT_FEATURE_NAME afl_create_token_aliases_default_feature
}

#endif //AFUNCTIONLIBRARY_DEFINITIONS_H

#ifndef AFUNCTIONLIBRARYSDK_CREATEALIASESAPI_H
#define AFUNCTIONLIBRARYSDK_CREATEALIASESAPI_H

#include "libs/APluginSDK/pluginapi.h"

#include "include/AFunctionLibrarySDK/astringaliases.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE
    const char* k_CreateTokenAliasesFeatureGroupName = "_afl_create_token_aliases_";
PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#define AFL_REGISTER_CREATE_TOKEN_ALIASES_FEATURE(feature_name, token_value_param_name, aliases_type_param_name)       \
    A_PLUGIN_REGISTER_FEATURE(struct AStringAliases*, _afl_create_token_aliases_, feature_name,                        \
                              const char* token_value_param_name, enum AStringAliasesType aliases_type_param_name)

#endif /* AFUNCTIONLIBRARYSDK_CREATEALIASESAPI_H */

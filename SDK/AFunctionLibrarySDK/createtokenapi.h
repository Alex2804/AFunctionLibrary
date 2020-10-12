#ifndef AFUNCTIONLIBRARYSDK_CREATETOKENAPI_H
#define AFUNCTIONLIBRARYSDK_CREATETOKENAPI_H

#include "libs/APluginSDK/pluginapi.h"

#include "include/AFunctionLibrarySDK/atoken.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE
    const char* k_CreateTokenFeatureGroupName = "_afl_create_token_";
PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#define AFL_REGISTER_CREATE_TOKEN_FEATURE(feature_name, token_value_param_name)                                        \
    A_PLUGIN_REGISTER_FEATURE(struct AToken*, _afl_create_token_, feature_name, const char *token_value_param_name)

#endif /* AFUNCTIONLIBRARYSDK_CREATETOKENAPI_H */

#include "AFunctionLibrary/createtokenapi.h"

#include <cstring>

AFL_REGISTER_CREATE_TOKEN_FEATURE(just_for_testing, tokenValue)
{
    if(std::strlen(tokenValue) == 1 && tokenValue[0] >= '0' && tokenValue[0] <= '9') {
        auto token = static_cast<afl::CStringToken*>(apl::allocateMemory(sizeof(afl::CStringToken)));
        token->string = tokenValue;
        token->type = afl::TokenType::Number;
        token->precedence = 0;
        token->parameterCount = 0;
        token->associativity = afl::TokenAssociativity::None;
        return token;
    }
    return nullptr;
}

AFL_REGISTER_CREATE_TOKEN_ALIASES_FEATURE(just_for_testing, value, type)
{
    if(type == afl::TokenAliasType::String) {
        if(std::strcmp(value, "1") == 0) {
            auto alias = static_cast<afl::CStringTokenAliases*>(apl::allocateMemory(sizeof(afl::CStringTokenAliases)));
            alias->type = type;
            alias->aliasesCount = 1;
            alias->aliases = static_cast<char**>(apl::allocateMemory(sizeof(char*)));
            alias->aliases[0] = static_cast<char*>(apl::allocateMemory(sizeof(char) * 4));
            alias->aliases[0][0] = 'o';
            alias->aliases[0][1] = 'n';
            alias->aliases[0][2] = 'e';
            alias->aliases[0][3] = '\0';
            return alias;
        }
    }
    return nullptr;
}

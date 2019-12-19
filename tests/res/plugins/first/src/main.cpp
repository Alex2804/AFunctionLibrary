#include "AFunctionLibrary/createtokenapi.h"

#include <cstring>


AFL_REGISTER_CREATE_TOKEN_FEATURE(just_for_testing, tokenValue)
{
    if(std::strlen(tokenValue) == 1 && std::isdigit(tokenValue[0])) {
        return afl::convert(afl::Token<std::string>(tokenValue, afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None));
    }
    return nullptr;
}

AFL_REGISTER_CREATE_TOKEN_FEATURE(just_for_another_test, tokenValue)
{
    if(std::strlen(tokenValue) == 1 && std::isalpha(tokenValue[0])) {
        return afl::convert(afl::Token<std::string>(tokenValue, afl::TokenType::Constant, 0, 0, afl::TokenAssociativity::None));
    }
    return nullptr;
}

AFL_REGISTER_CREATE_TOKEN_ALIASES_FEATURE(just_for_testing, value, type)
{
    if(type == afl::TokenAliasType::String && std::strcmp(value, "1") == 0) {
        afl::TokenAliases<std::string> alias;
        alias.type = type;
        alias.append("one");
        return afl::convert(alias);
    }
    return nullptr;
}
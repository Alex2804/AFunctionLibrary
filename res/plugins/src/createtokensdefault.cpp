#include "AFunctionLibrary/createtokenapi.h"

#include <string>
#include <regex>

const std::regex constantRegex(R"([a-zA-Z_][\w_]*)");
const std::regex numberRegex(R"(\d+(\.\d+)?)");


afl::CStringToken* createConstantTokens(afl::CStringToken* token, const std::string& tokenString)
{
    if(std::regex_match(tokenString, constantRegex)) {
        token->type = afl::TokenType::Constant;
        return token;
    }
    return nullptr;
}
afl::CStringToken* createNumberTokens(afl::CStringToken* token, const std::string& tokenString)
{
    if(std::regex_match(tokenString, numberRegex)) {
        token->type = afl::TokenType::Number;
        return token;
    }
    return nullptr;
}

typedef afl::CStringToken*(*createTokenFunction)(afl::CStringToken*, const std::string&);

AFL_REGISTER_CREATE_TOKEN_FEATURE(AFL_CREATE_TOKEN_DEFAULT_FEATURE_NAME, tokenValue)
{
    auto token = static_cast<afl::CStringToken*>(apl::allocateMemory(sizeof(afl::CStringToken)));
    token->string = tokenValue;
    std::string tokenString = token->string;
    createTokenFunction functions[] = {createConstantTokens, createNumberTokens};
    for(createTokenFunction function : functions) {
        token->precedence = 0;
        token->parameterCount = 0;
        token->associativity = afl::TokenAssociativity::None;
        if(function(token, tokenString) != nullptr)
            return token;
    }
    apl::freeMemory(token);
    return nullptr;
}
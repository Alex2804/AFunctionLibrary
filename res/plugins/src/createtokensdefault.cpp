#include "AFunctionLibrary/createtokenapi.h"

#include <string>
#include <regex>

const std::regex constantRegex(R"([a-zA-Z_][\w_]*)");
const std::regex numberRegex(R"(\d+(\.\d+)?)");

AFL_REGISTER_CREATE_TOKEN_FEATURE(__AFL_C_API_CREATE_TOKEN_DEFAULT_FEATURE_NAME__, tokenValue)
{
    std::string tokenString = tokenValue;
    if(std::regex_match(tokenString, numberRegex)) {
        return afl::convert(afl::Token<std::string>(tokenValue, afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None));
    } else if(std::regex_match(tokenString, constantRegex)) {
        return afl::convert(afl::Token<std::string>(tokenValue, afl::TokenType::Constant, 0, 0, afl::TokenAssociativity::None));
    }
    return nullptr;
}
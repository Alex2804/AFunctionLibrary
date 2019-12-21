#include "AFunctionLibrary/createtokenapi.h"
#include "AFunctionLibrary/formatfunctionapi.h"

AFL_CPP_API_REGISTER_CREATE_TOKEN_FEATURE(just_for_testing, tokenValue)
{
    if(std::all_of(tokenValue.begin(), tokenValue.end(), ::isalpha)) {
        return std::make_shared<afl::Token<std::string>>(tokenValue, afl::TokenType::Constant, 0, 0, afl::TokenAssociativity::None);
    }
    return std::shared_ptr<afl::Token<std::string>>();
}

AFL_CPP_API_REGISTER_CREATE_TOKEN_ALIASES_FEATURE(just_for_testing, value)
{
    std::vector<afl::TokenAliases<std::string>> aliases;
    if(value == "hello") {
        aliases.emplace_back(afl::TokenAliasType::String, std::vector<std::string>({"hello_world"}));
    }
    return aliases;
}

AFL_CPP_REGISTER_FORMAT_FUNCTION_STRING_FEATURE(just_for_testing, function)
{
    std::string toReplace = "hello";
    if(function.find(toReplace) != std::string::npos)
        function.replace(function.find(toReplace), toReplace.size(), "hi");
    return function;
}

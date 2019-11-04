#include "stringtokenmanager.h"

#include <regex>

namespace
{
    const std::regex numberRegex(R"(\d+(\.\d+)?)");
    const std::regex constantRegex(R"(_*[a-zA-Z][\w_]*)");
}

std::shared_ptr<afl::Token<std::string>> afl::detail::StringTokenManager::getToken(std::string value, bool createIfNotExist)
{
    auto iterator = tokens.find(value);
    if (iterator != tokens.end()) {
        return iterator->second;
    } else if (createIfNotExist) {
        auto token = std::shared_ptr<Token<std::string>>(createToken(std::move(value)).release());
        if(token != nullptr)
            tokens.emplace(token->value, token);
        return token;
    } else {
        return std::shared_ptr<afl::Token<std::string>>();
    }
}
std::unique_ptr<afl::Token<std::string>> afl::detail::StringTokenManager::createToken(std::string value)
{
    afl::Token<std::string>* token = nullptr;
    if(std::regex_match(value, numberRegex)) {
        token = new afl::Token<std::string>(std::move(value), afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None);
    } else if(std::regex_match(value, constantRegex)) {
        token = new afl::Token<std::string>(std::move(value), afl::TokenType::Constant, 0, 0, afl::TokenAssociativity::None);
    }
    return std::unique_ptr<afl::Token<std::string>>(token);
}

void afl::detail::StringTokenManager::addToken(std::unique_ptr<Token<std::string>> token)

{
    if(tokens.find(token->value) == tokens.end()) {
        afl::Token<std::string>* tmpToken = token.release();
        tokens.emplace(tmpToken->value, std::shared_ptr<afl::Token<std::string>>(tmpToken));
    }
}
void afl::detail::StringTokenManager::removeToken(const std::string &value)
{
    tokens.erase(value);
}

std::unordered_map<std::string, std::shared_ptr<afl::Token<std::string>>> afl::detail::StringTokenManager::getTokens() const
{
    return tokens;
}

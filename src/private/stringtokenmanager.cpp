#include "stringtokenmanager.h"

#include <regex>

std::unordered_map<std::string, std::shared_ptr<afl::Token<std::string>>> afl::detail::StringTokenManager::tokens;
std::mutex afl::detail::StringTokenManager::mutex;

const std::regex numberRegex(R"(\d+(\.\d+)?)");
const std::regex constantRegex(R"(_*[a-zA-Z][\w_]*)");

afl::Token<std::string>* createToken(std::string value)
{
    if(std::regex_match(value, numberRegex)) {
        return new afl::Token<std::string>(std::move(value), afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None);
    } else if(std::regex_match(value, constantRegex)) {
        return new afl::Token<std::string>(std::move(value), afl::TokenType::Constant, 0, 0, afl::TokenAssociativity::None);
    } else {
        return nullptr; // TODO: Add Plugin integration
    }
}

std::shared_ptr<afl::Token<std::string>> afl::detail::StringTokenManager::getToken(std::string value, bool createIfNotExist)
{
    std::lock_guard<std::mutex> guard(mutex);
    auto iterator = tokens.find(value);
    if (iterator != tokens.end()) {
        return iterator->second;
    } else if (createIfNotExist) {
        auto token = std::shared_ptr<Token<std::string>>(createToken(std::move(value)));
        if(token != nullptr)
            tokens.emplace(token->value, token);
        return token;
    } else {
        return std::shared_ptr<afl::Token<std::string>>();
    }
}
void afl::detail::StringTokenManager::removeToken(const std::string &value)
{
    mutex.lock();
    tokens.erase(value);
    mutex.unlock();
}
void afl::detail::StringTokenManager::addToken(std::unique_ptr<Token<std::string>> token)

{
    mutex.lock();
    if(tokens.find(token->value) == tokens.end()) {
        afl::Token<std::string>* tmpToken = token.release();
        tokens.emplace(tmpToken->value, std::shared_ptr<afl::Token<std::string>>(tmpToken));
    }
    mutex.unlock();
}

std::unordered_map<std::string, std::shared_ptr<afl::Token<std::string>>> afl::detail::StringTokenManager::getTokens()
{
    return tokens;
}

#include "../functionformatter.h"

#include <algorithm>
#include <unordered_map>
#include <utility>
#include <vector>

#include "AFunctionLibrary/utility.h"

namespace
{
    template<typename T>
    void extractAliases(const std::string& tokenValue, const std::set<std::string>& aliasStrings,
                        std::vector<std::pair<std::vector<T>, std::string>>& aliases)
    {
        std::vector<T> vector;
        vector.reserve(aliasStrings.size());
        for(const std::string& aliasString : aliasStrings) {
            vector.emplace_back(aliasString);
        }
        aliases.emplace_back(std::move(vector), tokenValue);
    }
    void extractAliases(const std::vector<std::shared_ptr<const afl::detail::TokenWrapper<std::string>>>& tokens,
                        std::vector<std::pair<std::vector<std::string>, std::string>>& stringAliases,  // first: string aliases, second: token->value
                        std::vector<std::pair<std::vector<std::regex>, std::string>>& regexAliases) // first: regex aliases, second: token->value
    {
        for(const std::shared_ptr<const afl::detail::TokenWrapper<std::string>>& wrapper : tokens) {
            for(const afl::TokenAliases<std::string>& alias : wrapper->aliases) {
                if(alias.type == afl::TokenAliasType::Regex)
                    extractAliases(wrapper->token->value, alias.aliases, regexAliases);
                else if(alias.type == afl::TokenAliasType::String)
                    extractAliases(wrapper->token->value, alias.aliases, stringAliases);
            }
        }
    }

    std::string replaceTokenAliases(std::string function,
                                    const std::vector<std::shared_ptr<const afl::detail::TokenWrapper<std::string>>>& uniqueTokens,
                                    const std::vector<std::shared_ptr<const afl::detail::TokenWrapper<std::string>>>& notUniqueTokens)
    {
        function.erase(std::remove_if(function.begin(), function.end(), ::isspace), function.end()); // remove all spaces

        std::vector<std::pair<std::vector<std::string>, std::string>> stringAliases;
        std::vector<std::pair<std::vector<std::regex>, std::string>> regexAliases;
        extractAliases(uniqueTokens, stringAliases, regexAliases);

        // replace unique string aliases with their associated tokens
        std::string formatted = afl::replaceString(std::move(function), stringAliases);
        // replace unique regex aliases with their associated tokens
        for(const std::pair<std::vector<std::regex>, std::string>& pair : regexAliases) {
            for(const std::regex& regex : pair.first) {
                formatted = std::regex_replace(formatted, regex, pair.second);
            }
        }

        // separate unique tokens with spaces
        for(const std::shared_ptr<const afl::detail::TokenWrapper<std::string>>& wrapper : uniqueTokens) {
            formatted = afl::replaceString(formatted, wrapper->token->value, " " + wrapper->token->value + " ");
        }

        stringAliases.clear();
        regexAliases.clear();
        extractAliases(notUniqueTokens, stringAliases, regexAliases);

        // replace not unique string aliases with their associated tokens
        formatted = afl::replaceString(formatted, stringAliases);
        // replace not unique regex aliases with their associated tokens
        for(const std::pair<std::vector<std::regex>, std::string>& pair : regexAliases) {
            for(const std::regex& regex : pair.first) {
                formatted = std::regex_replace(formatted, regex, pair.second);
            }
        }

        formatted.erase(std::remove_if(formatted.begin(), formatted.end(), ::isspace), formatted.end()); // remove all spaces

        return formatted;
    }
}

afl::detail::FunctionFormatter::FunctionFormatter(std::shared_ptr<ResourceManager> resourceManager)
    : m_resourceManager(std::move(resourceManager))
{
    reloadTokens();
}
afl::detail::FunctionFormatter::FunctionFormatter(const FunctionFormatter& other) = default;
afl::detail::FunctionFormatter::FunctionFormatter(FunctionFormatter&& other) noexcept = default;
afl::detail::FunctionFormatter::~FunctionFormatter() = default;

afl::detail::FunctionFormatter& afl::detail::FunctionFormatter::operator=(const FunctionFormatter& other) = default;
afl::detail::FunctionFormatter& afl::detail::FunctionFormatter::operator=(FunctionFormatter&& other) noexcept = default;

void afl::detail::FunctionFormatter::reloadTokens()
{
    for(const auto& tokenWrapper : m_resourceManager->getTokenManager()->getTokens()) {
        if(tokenWrapper->token->type == TokenType::Operator || tokenWrapper->token->type == TokenType::ArgumentDelimiter
           || tokenWrapper->token->type == TokenType::BracketOpen || tokenWrapper->token->type == TokenType::BracketClose)
        {
            m_uniqueTokens.push_back(tokenWrapper);
        } else {
            m_notUniqueTokens.push_back(tokenWrapper);
        }
    }
}

std::string afl::detail::FunctionFormatter::formatFunction(std::string function)
{
    std::string tmp, formattedString = std::move(function);
    while(tmp != formattedString) {
        tmp = formattedString;
        formattedString = replaceTokenAliases(std::move(formattedString), m_uniqueTokens, m_notUniqueTokens);
    }
    return formattedString;
}

std::vector<std::string> afl::detail::FunctionFormatter::splitIntoTokens(std::string string)
{
    for(const std::shared_ptr<const TokenWrapper<std::string>>& wrapper : m_uniqueTokens)
        string = replaceString(string, wrapper->token->value, " " + wrapper->token->value + " ");
    return splitAtSpaces(std::move(string));
}

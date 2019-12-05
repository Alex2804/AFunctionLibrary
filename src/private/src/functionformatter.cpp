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
    void extractAliases(const std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>>& tokens,
                        std::vector<std::pair<std::vector<std::string>, std::string>>& stringAliases,  // first: string aliases, second: token->value
                        std::vector<std::pair<std::vector<std::regex>, std::string>>& regexAliases) // first: regex aliases, second: token->value
    {
        for(const auto& pair : tokens) {
            for(const afl::TokenAliases<std::string>& alias : pair.second.first->aliases) {
                if(alias.type == afl::TokenAliasType::Regex)
                    extractAliases(pair.second.first->token->value, alias.aliases, regexAliases);
                else if(alias.type == afl::TokenAliasType::String)
                    extractAliases(pair.second.first->token->value, alias.aliases, stringAliases);
            }
        }
    }

    std::string replaceTokenAliases(std::string function,
                                    const std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>>& uniqueTokens,
                                    const std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>>& notUniqueTokens)
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
        for(const auto& pair : uniqueTokens)
            formatted = afl::replaceString(formatted, pair.second.first->token->value, " " + pair.second.first->token->value + " ");

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
    reloadPluginFormatFunctions();
}
afl::detail::FunctionFormatter::FunctionFormatter(const FunctionFormatter& other) = default;
afl::detail::FunctionFormatter::FunctionFormatter(FunctionFormatter&& other) noexcept = default;
afl::detail::FunctionFormatter::~FunctionFormatter() = default;

afl::detail::FunctionFormatter& afl::detail::FunctionFormatter::operator=(const FunctionFormatter& other) = default;
afl::detail::FunctionFormatter& afl::detail::FunctionFormatter::operator=(FunctionFormatter&& other) noexcept = default;

void afl::detail::FunctionFormatter::reloadPluginFormatFunctions()
{
    m_pluginFormatFunctions = m_resourceManager->getPluginManager()->getFeatures(kFormatFunctionFeatureGroupName, apl::PluginFeatureFilter::FeatureGroup);
}

std::string afl::detail::FunctionFormatter::formatFunction(std::string function)
{
    std::string tmp1 = function.empty() ? " " : "", tmp2;
    std::string formattedString = std::move(function);
    // replace aliases
    while(tmp1 != formattedString) {
        tmp1 = formattedString;
        formattedString = replaceTokenAliases(std::move(formattedString),
                m_resourceManager->getTokenManager()->m_uniqueTokens,
                m_resourceManager->getTokenManager()->m_notUniqueTokens);
    }
    // format with plugin format functions
    char* formattedCString;
    while(tmp1 != formattedString) {
        tmp1 = formattedString;
        for(const apl::PluginFeatureInfo* featureInfo : m_pluginFormatFunctions) {
            tmp2 = formattedString.empty() ? " " : "";
            while(formattedString != tmp2) {
                tmp2 = formattedString;
                formattedCString = reinterpret_cast<formatFunctionPluginFunction>(featureInfo->functionPointer)(formattedString.c_str());
                if(formattedCString != nullptr) {
                    formattedString = formattedCString;
                    featureInfo->pluginInfo->freeMemory(formattedCString);
                }
            }
        }
    }
    return formattedString;
}

std::vector<std::string> afl::detail::FunctionFormatter::splitIntoTokens(std::string string)
{
    for(const auto& pair : m_resourceManager->getTokenManager()->m_uniqueTokens)
        string = replaceString(string, pair.second.first->token->value, " " + pair.second.first->token->value + " ");
    return splitAtSpaces(std::move(string));
}

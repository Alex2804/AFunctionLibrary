#include "../functionformatter.h"

#include <unordered_map>
#include <algorithm>
#include <utility>
#include <regex>

#include "AFunctionLibrary/implementation/formatfunctionapi/formatfunctionapi_definitions.hpp"

namespace
{
    template<typename T>
    std::vector<std::pair<std::vector<T>, std::string>> extractAliases(const std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>>& tokens, afl::TokenAliasType type)
    {
        std::vector<std::pair<std::vector<T>, std::string>> aliases;
        for(const auto& pair : tokens) {
            std::vector<T> tmp;
            for(const afl::TokenAliases<std::string>& alias : pair.second.first->aliases) {
                if(alias.type == type) {
                    tmp.reserve(tmp.size() + alias.aliases.size());
                    for(const std::string& s : alias.aliases) {
                        tmp.emplace_back(s);
                    }
                }
            }
            aliases.emplace_back(tmp, pair.second.first->token->value);
        }
        return aliases;
    }

    std::string replaceTokenAliases(std::string function,
                                    const std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>>& uniqueTokens,
                                    const std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>>& notUniqueTokens)
    {
        function.erase(std::remove_if(function.begin(), function.end(), ::isspace), function.end()); // remove spaces

        std::vector<std::pair<std::vector<std::string>, std::string>> stringAliases;
        std::vector<std::pair<std::vector<std::regex>, std::string>> regexAliases;
        stringAliases = extractAliases<std::string>(uniqueTokens, afl::TokenAliasType::String);
        regexAliases = extractAliases<std::regex>(uniqueTokens, afl::TokenAliasType::Regex);

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

        stringAliases = extractAliases<std::string>(notUniqueTokens, afl::TokenAliasType::String);
        regexAliases = extractAliases<std::regex>(notUniqueTokens, afl::TokenAliasType::Regex);

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

void afl::detail::FunctionFormatter::reloadPluginFormatFunctions()
{
    std::vector<const apl::PluginFeatureInfo*> tmpFeatures = m_resourceManager->getPluginManager()->getFeatures(k_C_API_FormatFunctionStringFeatureGroupName, apl::PluginFeatureFilter::FeatureGroup);
    m_cApiPluginFormatStringFunctions.reserve(tmpFeatures.size());
    for(const apl::PluginFeatureInfo* info : tmpFeatures)
        m_cApiPluginFormatStringFunctions.push_back(reinterpret_cast<CApiProcessStringPluginFunction>(info->functionPointer));

    tmpFeatures = m_resourceManager->getPluginManager()->getFeatures(k_CPP_API_FormatFunctionStringFeatureGroupName, apl::PluginFeatureFilter::FeatureGroup);
    m_cppApiPluginFormatStringFunctions.reserve(tmpFeatures.size());
    for(const apl::PluginFeatureInfo* info : tmpFeatures)
        m_cppApiPluginFormatStringFunctions.push_back(reinterpret_cast<CppApiProcessStringPluginFunction>(info->functionPointer));
}

std::string afl::detail::FunctionFormatter::replaceAliases(std::string function)
{
    std::string tmp;
    do {
        tmp = function;
        function = replaceTokenAliases(std::move(function), m_resourceManager->getTokenManager()->m_uniqueTokens,
                                       m_resourceManager->getTokenManager()->m_notUniqueTokens);
    } while(function != tmp);
    return function;
}
std::string afl::detail::FunctionFormatter::formatWithPlugins(std::string function)
{
    std::pair<bool, std::string> pair = {false, std::move(function)};
    do {
        pair = invokeCProcessStringPluginFunctions(pair.second, m_cApiPluginFormatStringFunctions);
        pair = invokeCppProcessStringPluginFunctions(std::move(pair.second), m_cppApiPluginFormatStringFunctions);
    } while(pair.first);
    return pair.second;
}
std::string afl::detail::FunctionFormatter::formatFunction(std::string function)
{
    std::string tmp;
    do {
        tmp = function;
        function = replaceAliases(std::move(function));
        function = formatWithPlugins(std::move(function));
    } while(function != tmp);
    return function;
}


std::pair<bool, std::string> afl::detail::invokeCProcessStringPluginFunctions(const std::string& function, const std::vector<CApiProcessStringPluginFunction>& pluginFunctions)
{
    CString* tmpCString = nullptr;
    CString* formattedCString = convert(function);
    bool updatedAnyTime = false, updated;
    do {
        updated = false;
        for(CApiProcessStringPluginFunction formatFunction : pluginFunctions) {
            do {
                tmpCString = formatFunction(formattedCString->string);
                if(tmpCString != nullptr && !equal(tmpCString, formattedCString)) {
                    free(formattedCString);
                    formattedCString = tmpCString;
                    updatedAnyTime = updated = true;
                } else if(tmpCString != nullptr) {
                    free(tmpCString);
                    break;
                }
            } while(tmpCString != nullptr); // breaks if content is equal for two cycles or plugin returns nullptr
        }
    } while(updated);
    return std::make_pair(updatedAnyTime, convert(formattedCString));
}
std::pair<bool, std::string> afl::detail::invokeCppProcessStringPluginFunctions(std::string function, const std::vector<CppApiProcessStringPluginFunction>& pluginFunctions)
{
    std::string tmp;
    bool updatedAnyTime = false, updated;
    do {
        updated = false;
        for(const CppApiProcessStringPluginFunction formatFunction : pluginFunctions) {
            do {
                tmp = formatFunction(function);
                if (tmp.empty() || tmp == function) {
                    break;
                } else {
                    function = std::move(tmp);
                    updatedAnyTime = updated = true;
                }
            } while(!function.empty());
        }
    } while(updated);
    return std::make_pair(updatedAnyTime, std::move(function));
}

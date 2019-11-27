#include "../tokenmanager.h"

#include <cstring>
#include <algorithm>
#include <array>

#include "AFunctionLibrary/implementation/definitions.h"
#include "AFunctionLibrary/utility.h"

#include "../resourcemanager.h"

void afl::detail::TokenManager::addPluginFeatures(const apl::Plugin* plugin)
{
    if(plugin == nullptr || !plugin->isLoaded())
        return;
    const apl::PluginFeatureInfo* const* featureInfos = plugin->getFeatureInfos();
    const size_t featureCount = plugin->getFeatureCount();
    std::vector<createTokenPluginFunction> tokenFunctions;
    std::vector<createTokenAliasesPluginFunction> aliasesFunctions;
    for(size_t i = 0; i < featureCount; i++) {
        if(std::strcmp(featureInfos[i]->featureGroup, afl::kCreateTokenFeatureGroupName) == 0)
            tokenFunctions.push_back(reinterpret_cast<createTokenPluginFunction>(featureInfos[i]->functionPointer));
        else if(std::strcmp(featureInfos[i]->featureGroup, afl::kCreateTokenAliasesFeatureGroupName) == 0)
            aliasesFunctions.push_back(reinterpret_cast<createTokenAliasesPluginFunction>(featureInfos[i]->functionPointer));
    }
    if(!tokenFunctions.empty() || !aliasesFunctions.empty())
        m_pluginFunctions.emplace_back(plugin, tokenFunctions, aliasesFunctions);
}
void afl::detail::TokenManager::removePluginFeatures(const apl::Plugin* plugin)
{
    if(plugin == nullptr)
        return;
    const apl::PluginInfo* pluginInfo = plugin->getPluginInfo();
    m_pluginFunctions.erase(std::remove_if(m_pluginFunctions.begin(), m_pluginFunctions.end(),
                                           [pluginInfo](const std::tuple<const apl::Plugin*, std::vector<createTokenPluginFunction>, std::vector<createTokenAliasesPluginFunction>>& tuple)
                        { return std::get<0>(tuple)->getPluginInfo() == pluginInfo; }), m_pluginFunctions.end());
    removeReferences(getFullPathName(plugin->getPath(), ResourceType::Plugin));
}
void afl::detail::TokenManager::removeReferences(const std::string &refPath)
{
    auto refIterator = m_pathTokenValueRefs.find(refPath);
    if(refIterator != m_pathTokenValueRefs.end()) {
        for(const std::string& value : refIterator->second) {
            auto tokenIterator = m_tokens.find(value);
            if(tokenIterator != m_tokens.end() && --(tokenIterator->second.second) == 0)
                m_tokens.erase(tokenIterator);
        }
        m_pathTokenValueRefs.erase(refIterator);
    }
}

bool afl::detail::TokenManager::addToken(std::shared_ptr<TokenWrapper<std::string>> tokenWrapper, std::string refPath)
{
    if(tokenWrapper == nullptr || tokenWrapper->token == nullptr)
        return false;
    std::string tokenValue = tokenWrapper->token->value;
    auto refIterator = m_pathTokenValueRefs.find(refPath);
    if(refIterator != m_pathTokenValueRefs.end()) {
        if(std::find(refIterator->second.begin(), refIterator->second.end(), tokenValue) != refIterator->second.end()) // token with this reference exists already
            return true;
    } else {
        refIterator = m_pathTokenValueRefs.emplace(std::move(refPath), std::vector<std::string>()).first;
    }
    auto tokenIterator = m_tokens.emplace(tokenValue,
            std::pair<std::shared_ptr<TokenWrapper<std::string>>, size_t>{std::move(tokenWrapper), 0}).first;
    tokenIterator->second.second += 1;
    for(const auto & i : refIterator->second) {
        if(i == tokenValue)
            return true;
    }
    refIterator->second.emplace_back(std::move(tokenValue));
    return true;
}
void afl::detail::TokenManager::removeToken(const std::string& value, const std::string& refPath)
{
    auto tokenIterator = m_tokens.find(value);
    if(tokenIterator != m_tokens.end()) { // if token with this value exists
        auto refIterator = m_pathTokenValueRefs.find(refPath); // search references of refPath
        if (refIterator != m_pathTokenValueRefs.end()) { // if refPath has any references
            auto refTokenStringIterator = std::find(refIterator->second.begin(), refIterator->second.end(), value); // search reference
            if (refTokenStringIterator != refIterator->second.end()) { // if reference between refPath and value exists
                refIterator->second.erase(refTokenStringIterator); // delete reference
                if (--(tokenIterator->second.second) == 0) { // if this is the last reference to the token
                    m_tokens.erase(tokenIterator); // delete the token
                }
            }
            if(refIterator->second.empty())
                m_pathTokenValueRefs.erase(refIterator);
        }
    }
}
void afl::detail::TokenManager::removeToken(const std::shared_ptr<const TokenWrapper<std::string>>& tokenWrapper, const std::string& refPath)
{
    if(tokenWrapper != nullptr && tokenWrapper->token != nullptr)
        removeToken(tokenWrapper->token->value, refPath);
}

std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, std::string> afl::detail::TokenManager::createToken(const char* value, bool createAliases) const
{
    std::vector<afl::TokenAliases<std::string>> aliases;
    if(createAliases)
        aliases = this->createAliases(value);
    const apl::Plugin* plugin = nullptr; // plugin from which the token was created;
    CStringToken* cToken = nullptr;
    for(const auto& tuple : m_pluginFunctions) {
        plugin = std::get<0>(tuple);
        for(createTokenPluginFunction function : std::get<1>(tuple)) {
            cToken = function(value);
            if(cToken != nullptr)
                goto TOKEN_FOUND; // break if token was found
        }
    }
    TOKEN_FOUND:
    if(cToken != nullptr) {
        auto token = std::make_shared<const Token<std::string>>(value, cToken->type, cToken->precedence, cToken->parameterCount, cToken->associativity);
        return {std::make_shared<TokenWrapper<std::string>>(std::move(token), std::move(aliases)),
                getFullPathName(plugin->getPath(), ResourceType::Plugin)};
    }
    return {std::shared_ptr<TokenWrapper<std::string>>(), ""};
}
std::vector<afl::TokenAliases<std::string>> afl::detail::TokenManager::createAliases(const char* value) const
{
    std::array<TokenAliasType, 2> types = {TokenAliasType::String, TokenAliasType::Regex};
    std::vector<TokenAliases<std::string>> aliases;
    CStringTokenAliases* cAlias;
    for(TokenAliasType type : types) {
        aliases.emplace_back();
        aliases.back().type = type;
    }
    for(const auto& tuple : m_pluginFunctions) {
        const apl::Plugin* plugin = std::get<0>(tuple);
        for(createTokenAliasesPluginFunction function : std::get<2>(tuple)) {
            for(size_t i = 0; i < types.size(); i++) {
                cAlias = function(value, types[i]);
                if(cAlias != nullptr) {
                    std::copy(cAlias->aliases, cAlias->aliases + cAlias->aliasesCount, std::inserter(aliases[i].aliases, aliases[i].aliases.end()));
                    for(size_t j = 0; j < cAlias->aliasesCount; j++) {
                        plugin->freeMemory(cAlias->aliases[j]);
                    }
                    plugin->freeMemory(cAlias->aliases);
                    plugin->freeMemory(cAlias);
                }
            }
        }
    }
    aliases.erase(std::remove_if(aliases.begin(), aliases.end(), [](const TokenAliases<std::string>& a){ return a.aliases.empty(); }), aliases.end()); // remove aliases without alias strings
    return aliases;
}

std::shared_ptr<const afl::detail::TokenWrapper<std::string>> afl::detail::TokenManager::getToken(const std::string &value) const
{
    auto iterator = m_tokens.find(value);
    if(iterator != m_tokens.end()) // token exists
        return iterator->second.first;
    return std::shared_ptr<const TokenWrapper<std::string>>();
}
std::shared_ptr<const afl::detail::TokenWrapper<std::string>> afl::detail::TokenManager::getToken(const std::string &value, bool createIfNotExist)
{
    std::shared_ptr<const TokenWrapper<std::string>> token = getToken(value);
    if(token == nullptr && createIfNotExist) {
        std::pair<std::shared_ptr<TokenWrapper<std::string>>, std::string> pair = createToken(value.c_str());
        if(addToken(pair.first, std::move(pair.second)))
            return pair.first;
    }
    return token;
}
std::vector<std::shared_ptr<const afl::detail::TokenWrapper<std::string>>> afl::detail::TokenManager::getTokens() const
{
    std::vector<std::shared_ptr<const TokenWrapper<std::string>>> tokens;
    for(const std::pair<const std::string, std::pair<std::shared_ptr<TokenWrapper<std::string>>, size_t>>& pair : m_tokens)
        tokens.push_back(pair.second.first);
    return tokens;
}
std::vector<std::shared_ptr<const afl::detail::TokenWrapper<std::string>>> afl::detail::TokenManager::getTokens(TokenType type) const
{
    std::vector<std::shared_ptr<const TokenWrapper<std::string>>> tokens;
    for(const std::pair<const std::string, std::pair<std::shared_ptr<TokenWrapper<std::string>>, size_t>>& pair : m_tokens) {
        if(pair.second.first->token->type == type)
            tokens.push_back(pair.second.first);
    }
    return tokens;
}

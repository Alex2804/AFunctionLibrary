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

bool afl::detail::TokenManager::addToken(std::shared_ptr<TokenPtrBundle<std::string>> tokenPtrBundle, std::string refPath)
{
    if(tokenPtrBundle == nullptr || (!isUnique(tokenPtrBundle->token.get()) && !isNotUnique(tokenPtrBundle->token.get())))
        return false;
    std::string tokenValue = tokenPtrBundle->token->value;
    auto refIterator = m_pathTokenValueRefs.find(refPath);
    if(refIterator != m_pathTokenValueRefs.end()) {
        if(std::find(refIterator->second.begin(), refIterator->second.end(), tokenValue) != refIterator->second.end()) // token with this reference exists already
            return true;
    } else {
        refIterator = m_pathTokenValueRefs.emplace(std::move(refPath), std::vector<std::string>()).first;
    }
    // insert or increment ref count
    size_t newRefCount;
    if(isUnique(tokenPtrBundle->token.get()))
        newRefCount = ++(m_uniqueTokens.emplace(tokenValue, std::pair<std::shared_ptr<TokenPtrBundle<std::string>>, size_t>{std::move(tokenPtrBundle), 0}).first->second.second);
    else
        newRefCount = ++(m_notUniqueTokens.emplace(tokenValue, std::pair<std::shared_ptr<TokenPtrBundle<std::string>>, size_t>{std::move(tokenPtrBundle), 0}).first->second.second);
    // create reference if not existing
    refIterator->second.emplace_back(std::move(tokenValue));
    return true;
}
void afl::detail::TokenManager::removeToken(const std::string& value, const std::string& refPath)
{
    // token with this value exists
    auto refIterator = m_pathTokenValueRefs.find(refPath); // search references of refPath
    if (refIterator != m_pathTokenValueRefs.end()) { // if refPath has any references
        auto refTokenStringIterator = std::find(refIterator->second.begin(), refIterator->second.end(), value); // search reference
        if (refTokenStringIterator != refIterator->second.end()) { // if reference between refPath and value exists
            refIterator->second.erase(refTokenStringIterator); // delete reference

            auto tokenIterator = m_uniqueTokens.find(value);
            if (tokenIterator != m_uniqueTokens.end() && --(tokenIterator->second.second) == 0) // if this is the last reference to the token in uniqueTokens
                m_uniqueTokens.erase(tokenIterator); // delete the token
            tokenIterator = m_notUniqueTokens.find(value);
            if (tokenIterator != m_notUniqueTokens.end() && --(tokenIterator->second.second) == 0) // if this is the last reference to the token in notUniqueTokens
                m_notUniqueTokens.erase(tokenIterator); // delete the token
        }
        if(refIterator->second.empty())
            m_pathTokenValueRefs.erase(refIterator); // remove reference entry if there are no associated tokens with this refPath
    }
}
void afl::detail::TokenManager::removeReferences(const std::string& refPath)
{
    auto refIterator = m_pathTokenValueRefs.find(refPath);
    if(refIterator != m_pathTokenValueRefs.end()) {
        for(const std::string& value : refIterator->second) {
            auto tokenIterator = m_uniqueTokens.find(value);
            if(tokenIterator != m_uniqueTokens.end() && --(tokenIterator->second.second) == 0)
                m_uniqueTokens.erase(tokenIterator);
            tokenIterator = m_notUniqueTokens.find(value);
            if(tokenIterator != m_notUniqueTokens.end() && --(tokenIterator->second.second) == 0)
                m_notUniqueTokens.erase(tokenIterator);
        }
        m_pathTokenValueRefs.erase(refIterator);
    }
}

std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, std::string> afl::detail::TokenManager::createToken(const std::string& value, bool createAliases) const
{
    std::vector<afl::TokenAliases<std::string>> aliases;
    if(createAliases)
        aliases = this->createAliases(value);
    const char* cValue = value.c_str();
    const apl::Plugin* plugin = nullptr; // plugin from which the token was created;
    CStringToken* cToken = nullptr;
    for(const auto& tuple : m_pluginFunctions) {
        plugin = std::get<0>(tuple);
        for(createTokenPluginFunction function : std::get<1>(tuple)) {
            cToken = function(cValue);
            if(cToken != nullptr)
                goto TOKEN_FOUND; // break if token was found
        }
    }
    TOKEN_FOUND: // label for nested loop interrupt
    if(cToken != nullptr) {
        auto token = std::make_shared<const Token<std::string>>(value, cToken->type, cToken->precedence, cToken->parameterCount, cToken->associativity);
        plugin->freeMemory(cToken);
        return {std::make_shared<TokenPtrBundle<std::string>>(std::move(token), std::move(aliases)),
                getFullPathName(plugin->getPath(), ResourceType::Plugin)};
    }
    return {std::shared_ptr<TokenPtrBundle<std::string>>(), ""};
}
std::vector<afl::TokenAliases<std::string>> afl::detail::TokenManager::createAliases(const std::string& value) const
{
    std::array<TokenAliasType, 2> types = {TokenAliasType::String, TokenAliasType::Regex};
    std::vector<TokenAliases<std::string>> aliases;
    CStringTokenAliases* cAlias;
    for(TokenAliasType type : types) {
        aliases.emplace_back();
        aliases.back().type = type;
    }
    const char* cValue = value.c_str();
    for(const auto& tuple : m_pluginFunctions) {
        const apl::Plugin* plugin = std::get<0>(tuple);
        for(createTokenAliasesPluginFunction function : std::get<2>(tuple)) {
            for(size_t i = 0; i < types.size(); i++) {
                cAlias = function(cValue, types[i]);
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

std::shared_ptr<const afl::detail::TokenPtrBundle<std::string>> afl::detail::TokenManager::getToken(const std::string &value) const
{
    auto iterator = m_uniqueTokens.find(value);
    if(iterator != m_uniqueTokens.end()) // token exists
        return iterator->second.first;
    iterator = m_notUniqueTokens.find(value);
    if(iterator != m_notUniqueTokens.end())
        return iterator->second.first;
    return std::shared_ptr<const TokenPtrBundle<std::string>>();
}
std::shared_ptr<const afl::detail::TokenPtrBundle<std::string>> afl::detail::TokenManager::getToken(const std::string &value, bool createIfNotExist)
{
    std::shared_ptr<const TokenPtrBundle<std::string>> token = getToken(value);
    if(token == nullptr && createIfNotExist) {
        std::pair<std::shared_ptr<TokenPtrBundle<std::string>>, std::string> pair = createToken(value);
        if(addToken(pair.first, std::move(pair.second)))
            return pair.first;
    }
    return token;
}
std::vector<std::shared_ptr<const afl::detail::TokenPtrBundle<std::string>>> afl::detail::TokenManager::getTokens() const
{
    return filterTokens([](const Token<std::string>* t){ return true;});
}

bool afl::detail::TokenManager::isUnique(const afl::Token<std::string>* token)
{
    return token != nullptr && (token->type == TokenType::Operator || token->type == TokenType::ArgumentDelimiter
                                || token->type == TokenType::BracketOpen || token->type == TokenType::BracketClose);
}
bool afl::detail::TokenManager::isNotUnique(const afl::Token<std::string>* token)
{
    return token != nullptr && !isUnique(token);
}

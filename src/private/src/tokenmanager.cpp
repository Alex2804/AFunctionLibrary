#include "../tokenmanager.h"

#include <algorithm>
#include <cstring>
#include <array>
#include <queue>

#include "AFunctionLibrary/implementation/createtokenapi/createtokenapi_definitions.hpp"

#include "../resourcemanager.h"

void afl::detail::TokenManager::addPluginFeatures(const apl::Plugin* plugin)
{
    if(plugin == nullptr || !plugin->isLoaded())
        return;
    const apl::PluginFeatureInfo* const* featureInfos = plugin->getFeatureInfos();
    const size_t featureCount = plugin->getFeatureCount();
    std::vector<cApiCreateTokenPluginFunction> cApiTokenFunctions;
    std::vector<cppApiCreateTokenPluginFunction > cppApiTokenFunctions;
    std::vector<cApiCreateTokenAliasesPluginFunction> cApiAliasesFunctions;
    std::vector<cppApiCreateTokenAliasesPluginFunction> cppApiAliasesFunctions;
    for(size_t i = 0; i < featureCount; i++) {
        if(std::strcmp(featureInfos[i]->featureGroup, afl::k_C_API_CreateTokenFeatureGroupName) == 0)
            cApiTokenFunctions.push_back(reinterpret_cast<cApiCreateTokenPluginFunction>(featureInfos[i]->functionPointer));
        else if(std::strcmp(featureInfos[i]->featureGroup, afl::k_CPP_API_CreateTokenFeatureGroupName) == 0)
            cppApiTokenFunctions.push_back(reinterpret_cast<cppApiCreateTokenPluginFunction>(featureInfos[i]->functionPointer));
        else if(std::strcmp(featureInfos[i]->featureGroup, afl::k_C_API_CreateTokenAliasesFeatureGroupName) == 0)
            cApiAliasesFunctions.push_back(reinterpret_cast<cApiCreateTokenAliasesPluginFunction>(featureInfos[i]->functionPointer));
        else if(std::strcmp(featureInfos[i]->featureGroup, afl::k_CPP_API_CreateTokenAliasesFeatureGroupName) == 0)
            cppApiAliasesFunctions.push_back(reinterpret_cast<cppApiCreateTokenAliasesPluginFunction>(featureInfos[i]->functionPointer));
    }
    if(!cApiTokenFunctions.empty() || !cApiAliasesFunctions.empty() || !cppApiTokenFunctions.empty() || !cppApiAliasesFunctions.empty())
        m_pluginFunctions.emplace_back(plugin, cApiTokenFunctions, cApiAliasesFunctions, cppApiTokenFunctions, cppApiAliasesFunctions);
}
void afl::detail::TokenManager::removePluginFeatures(const apl::Plugin* plugin)
{
    if(plugin == nullptr)
        return;
    const apl::PluginInfo* pluginInfo = plugin->getPluginInfo();
    m_pluginFunctions.erase(std::remove_if(m_pluginFunctions.begin(), m_pluginFunctions.end(),
                                           [pluginInfo](const std::tuple<const apl::Plugin*, std::vector<cApiCreateTokenPluginFunction>, std::vector<cApiCreateTokenAliasesPluginFunction>, std::vector<cppApiCreateTokenPluginFunction>, std::vector<cppApiCreateTokenAliasesPluginFunction>>& tuple)
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
    std::shared_ptr<afl::Token<std::string>> token;
    for(const auto& tuple : m_pluginFunctions) {
        plugin = std::get<0>(tuple);
        for(cApiCreateTokenPluginFunction function : std::get<1>(tuple)) {
            cToken = function(cValue);
            if(cToken != nullptr)
                goto TOKEN_FOUND; // break if token was found
        }
        for(cppApiCreateTokenPluginFunction function : std::get<3>(tuple)) {
            token = function(value);
            if(token != nullptr)
                goto TOKEN_FOUND;
        }
    }
    TOKEN_FOUND: // label for nested loop interrupt
    if(cToken != nullptr)
        token = convert(cToken);
    if(token != nullptr) {
        return {std::make_shared<TokenPtrBundle<std::string>>(std::make_shared<const Token<std::string>>(std::move(*token)), std::move(aliases)),
                getFullPathName(plugin->getPath(), ResourceType::Plugin)};
    }
    return {std::shared_ptr<TokenPtrBundle<std::string>>(), ""};
}
std::vector<afl::TokenAliases<std::string>> afl::detail::TokenManager::createAliases(const std::string& value) const
{
    std::array<TokenAliasType, 2> aliasTypes = {TokenAliasType::String, TokenAliasType::Regex};
    std::vector<TokenAliases<std::string>> aliases;
    CStringTokenAliases* cAlias;
    aliases.reserve(aliasTypes.size());
    for(TokenAliasType type : aliasTypes) {
        aliases.emplace_back(type);
    }
    const char* cValue = value.c_str();
    for(const auto& tuple : m_pluginFunctions) {
        for(cApiCreateTokenAliasesPluginFunction function : std::get<2>(tuple)) {
            for(size_t i = 0; i < aliasTypes.size(); i++) {
                cAlias = function(cValue, aliasTypes[i]);
                if(cAlias != nullptr)
                    aliases[i].append(convert(cAlias));
            }
        }
        for(cppApiCreateTokenAliasesPluginFunction function : std::get<4>(tuple)) {
            std::vector<TokenAliases<std::string>> tmpAliases = function(value);
            for(size_t i = 0; i < aliasTypes.size(); i++) {
                for(const TokenAliases<std::string>& alias : tmpAliases) {
                    aliases[i].append(alias);
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

std::vector<std::shared_ptr<const afl::Token<std::string>>> afl::detail::TokenManager::stringToTokens(std::string string)
{
    string.erase(std::remove_if(string.begin(), string.end(), ::isspace), string.end()); // remove spaces
    for(const auto& pair : m_uniqueTokens)
        string = replaceString(std::move(string), pair.second.first->token->value, " " + pair.second.first->token->value + " ");
    std::vector<std::string> stringTokens = splitAtSpaces(std::move(string));
    std::vector<std::shared_ptr<const Token<std::string>>> tokens;
    tokens.reserve(stringTokens.size());
    std::shared_ptr<const TokenPtrBundle<std::string>> tokenPtrBundle;
    for(const std::string& stringToken : stringTokens) {
        tokenPtrBundle = getToken(stringToken, true);
        if(tokenPtrBundle == nullptr)
            throw std::runtime_error("No existing token for string " + stringToken);
        tokens.push_back(tokenPtrBundle->token);
    }
    return tokens;
}

std::vector<std::shared_ptr<const afl::Token<std::string>>> afl::detail::TokenManager::tokenGroupsToTokens(std::vector<TokenGroup<std::string>> tokenGroups)
{
    std::shared_ptr<const TokenPtrBundle<std::string>> bracketOpen, bracketClose, argumentDelimiter;
    if((bracketOpen = getToken("(", true)) == nullptr) {
        auto tmp = filterTokens([](const Token<std::string> *token) { return token->type == afl::TokenType::BracketOpen; });
        if(tmp.empty())
            throw std::runtime_error("TokenManager does not contain any Token of type BracketOpen!");
        bracketOpen = tmp.front();
    }
    if((bracketClose = getToken(")", true)) == nullptr) {
        auto tmp = filterTokens([](const Token<std::string> *token) { return token->type == afl::TokenType::BracketClose; });
        if(tmp.empty())
            throw std::runtime_error("TokenManager does not contain any Token of type BracketClose!");
        bracketClose = tmp.front();
    }
    if((argumentDelimiter = getToken(";", true)) == nullptr) {
        auto tmp = filterTokens([](const Token<std::string> *token) { return token->type == afl::TokenType::ArgumentDelimiter; });
        if(tmp.empty())
            throw std::runtime_error("TokenManager does not contain any Token of type ArgumentDelimiter!");
        argumentDelimiter = tmp.front();
    }

    auto predicate = [](const TokenGroup<std::string>& tokenGroup){ return !tokenGroup.isToken(); };
    tokenGroups.erase(std::remove_if(tokenGroups.begin(), tokenGroups.end(), predicate), tokenGroups.end()); // remove non token tokenGroups

    auto comparator = [](const TokenGroup<std::string>& g1, const TokenGroup<std::string>& g2){
        return g1.groupID < g2.groupID;
    };
    std::sort<decltype(tokenGroups.begin()), decltype(comparator)>(tokenGroups.begin(), tokenGroups.end(), comparator); // sort after groupIDs

    std::vector<size_t> tmpGroupID = {0}, lastGroupID = tmpGroupID;
    size_t dif = tokenGroups.front().groupID.size() - 1;
    for(TokenGroup<std::string>& tokenGroup : tokenGroups) {
        tokenGroup.groupID.erase(tokenGroup.groupID.begin(), tokenGroup.groupID.begin() + dif);
        if(tmpGroupID.size() > tokenGroup.groupID.size()) {
            tmpGroupID.erase(tmpGroupID.begin() + tokenGroup.groupID.size(), tmpGroupID.end());
            ++tmpGroupID.back();
        } else if(tmpGroupID.size() < tokenGroup.groupID.size()) {
            tmpGroupID.insert(tmpGroupID.end(), tokenGroup.groupID.size() - tmpGroupID.size(), 0);
        }
        size_t tmp = std::min(lastGroupID.size(), tokenGroup.groupID.size());
        for(size_t i = dif; i < (tmp < 1 ? 0 : tmp - 1); ++i) {
            if(lastGroupID[i] < tokenGroup.groupID[i]) {
                ++tmpGroupID[i - dif];
                for(size_t j = i - dif + 1; j < tmpGroupID.size(); ++j)
                    tmpGroupID[j] = 0;
            }
        }
        lastGroupID = tokenGroup.groupID;
        tokenGroup.groupID = tmpGroupID;
        ++tmpGroupID.back();
    }
    tokenGroups.emplace_back(std::make_shared<const Token<std::string>>("", TokenType::Constant, 0, 0, TokenAssociativity::None), std::vector<size_t>({std::numeric_limits<size_t>::max()}));

    std::vector<std::shared_ptr<const Token<std::string>>> tokens;
    TokenGroup<std::string>& oldGroup = tokenGroups.front();
    std::stack<size_t> functionGroupIDIndex, functionGroupIDOldID;
    std::queue<std::shared_ptr<const Token<std::string>>> bracketOpens;
    for(TokenGroup<std::string>& tokenGroup : tokenGroups) {
        // bracket open
        if(tokenGroup.groupID.size() > oldGroup.groupID.size()) {
            size_t bracketOpenCount = tokenGroup.groupID.size() - oldGroup.groupID.size();
            if(oldGroup.token->type == TokenType::ArgumentDelimiter)
                bracketOpenCount -= 1;
            for(size_t i = 0; i < bracketOpenCount; ++i) {
                if (bracketOpens.empty()) {
                    tokens.push_back(bracketOpen->token); // no bracket token in input
                } else {
                    tokens.push_back(bracketOpens.front()); // token bracket in input
                    bracketOpens.pop();
                }
            }
        }

        // bracket close
        if(tokenGroup.groupID.size() < oldGroup.groupID.size())
        {
            size_t bracketCloseCount = oldGroup.groupID.size() - tokenGroup.groupID.size();
            if(tokenGroup.token->type == TokenType::BracketClose || tokenGroup.token->type == TokenType::ArgumentDelimiter)
                bracketCloseCount -= 1;
            for(size_t i = 0; i < bracketCloseCount; ++i)
                tokens.push_back(bracketClose->token);

            // function arguments for top function(s) are finished, no more argument delimiters required for them
            while (!functionGroupIDIndex.empty() && functionGroupIDIndex.top() >= tokenGroup.groupID.size() - 1) {
                functionGroupIDIndex.pop();
                functionGroupIDOldID.pop();
            }
        }

        // argument delimiter
        if (!functionGroupIDIndex.empty()) {
            if (tokenGroup.token->type != TokenType::ArgumentDelimiter && tokenGroup.groupID[functionGroupIDIndex.top()] > functionGroupIDOldID.top())
                tokens.push_back(argumentDelimiter->token);
            if (tokenGroup.token->type == TokenType::BracketOpen)
                ++functionGroupIDOldID.top();
            else
                functionGroupIDOldID.top() = tokenGroup.groupID[functionGroupIDIndex.top()];
        }

        if(tokenGroup.token->type == TokenType::BracketOpen)
            bracketOpens.push(tokenGroup.token);
        else
            tokens.push_back(tokenGroup.token);

        if(tokenGroup.token->type == TokenType::Function) {
            functionGroupIDIndex.push(tokenGroup.groupID.size() - 1);
            functionGroupIDOldID.push(tokenGroup.groupID.back() + 1);
        }

        oldGroup = tokenGroup;
    }
    tokens.pop_back();
    return tokens;
}

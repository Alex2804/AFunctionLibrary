#ifndef AFUNCTIONLIBRARYSDK_CSTRINGTOKEN_HPP
#define AFUNCTIONLIBRARYSDK_CSTRINGTOKEN_HPP

#include "cstring.hpp"
#include "include/AFunctionLibrary/token.h"

namespace afl
{
    extern "C"
    {
        struct AFUNCTIONLIBRARY_NO_EXPORT CStringToken
        {
            const FreeMemoryFunction freeFunction;
            CString* string;
            TokenType type;
            size_t precedence;
            size_t parameterCount;
            TokenAssociativity associativity;
        };

        struct AFUNCTIONLIBRARY_NO_EXPORT CStringTokenAliases
        {
            const FreeMemoryFunction freeFunction;
            TokenAliasType type;
            size_t aliasesCount;
            CString** aliases;
        };

        struct AFUNCTIONLIBRARY_NO_EXPORT CStringTokenGroup
        {
            const FreeMemoryFunction freeFunction;
            CStringToken* token;
            size_t groupIDSize;
            size_t* groupID;
        };
    }

    inline AFUNCTIONLIBRARY_NO_EXPORT bool operator==(const CStringToken& t1, const CStringToken& t2);
    inline AFUNCTIONLIBRARY_NO_EXPORT bool operator==(const CStringTokenAliases& a1, const CStringTokenAliases& a2);
    inline AFUNCTIONLIBRARY_NO_EXPORT bool operator==(const CStringTokenGroup& g1, const CStringTokenGroup& g2);

    inline AFUNCTIONLIBRARY_NO_EXPORT bool operator!=(const CStringToken& t1, const CStringToken& t2);
    inline AFUNCTIONLIBRARY_NO_EXPORT bool operator!=(const CStringTokenAliases& a1, const CStringTokenAliases& a2);
    inline AFUNCTIONLIBRARY_NO_EXPORT bool operator!=(const CStringTokenGroup& g1, const CStringTokenGroup& g2);

    inline AFUNCTIONLIBRARY_NO_EXPORT bool equal(const CStringToken* t1, const CStringToken* t2);
    inline AFUNCTIONLIBRARY_NO_EXPORT bool equal(const CStringTokenAliases* a1, const CStringTokenAliases* a2);
    inline AFUNCTIONLIBRARY_NO_EXPORT bool equal(const CStringTokenGroup* g1, const CStringTokenGroup* g2);

    inline AFUNCTIONLIBRARY_NO_EXPORT void free(CStringToken* cToken);
    inline AFUNCTIONLIBRARY_NO_EXPORT void free(CStringTokenAliases* cTokenAliases);
    inline AFUNCTIONLIBRARY_NO_EXPORT void free(CStringTokenGroup* cTokenGroup);

    inline AFUNCTIONLIBRARY_NO_EXPORT CStringToken* convert(const Token<std::string>& token);
    inline AFUNCTIONLIBRARY_NO_EXPORT std::shared_ptr<Token<std::string>> convert(CStringToken* cToken, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&)=nullptr);
    inline AFUNCTIONLIBRARY_NO_EXPORT std::shared_ptr<Token<std::string>> convert(const CStringToken* cToken, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&)=nullptr);

    inline AFUNCTIONLIBRARY_NO_EXPORT CStringTokenAliases* convert(const TokenAliases<std::string>& tokenAliases);
    inline AFUNCTIONLIBRARY_NO_EXPORT TokenAliases<std::string> convert(CStringTokenAliases* cTokenAliases);
    inline AFUNCTIONLIBRARY_NO_EXPORT TokenAliases<std::string> convert(const CStringTokenAliases* cTokenAliases);

    inline AFUNCTIONLIBRARY_NO_EXPORT CStringTokenGroup* convert(const TokenGroup<std::string>& tokenGroup);
    inline AFUNCTIONLIBRARY_NO_EXPORT TokenGroup<std::string> convert(CStringTokenGroup* cTokenGroup, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&)=nullptr);
    inline AFUNCTIONLIBRARY_NO_EXPORT TokenGroup<std::string> convert(const CStringTokenGroup* cTokenGroup, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&)=nullptr);
}


bool afl::operator==(const CStringToken& t1, const CStringToken& t2)
{
    return equal(t1.string, t2.string) && t1.type == t2.type && t1.precedence == t2.precedence
           && t1.parameterCount == t2.parameterCount && t1.associativity == t2.associativity;
}
bool afl::operator==(const CStringTokenAliases& a1, const CStringTokenAliases& a2)
{
    if(a1.type != a2.type || a1.aliasesCount != a2.aliasesCount)
        return false;
    for(size_t i = 0; i < a1.aliasesCount; ++i) {
        if(!equal(a1.aliases[i], a2.aliases[i]))
            return false;
    }
    return true;
}
bool afl::operator==(const CStringTokenGroup& g1, const CStringTokenGroup& g2)
{
    if(g1.groupIDSize != g2.groupIDSize || !equal(g1.token, g2.token))
        return false;
    for(size_t i = 0; i < g1.groupIDSize; ++i) {
        if(g1.groupID[i] != g2.groupID[i])
            return false;
    }
    return true;
}

bool afl::operator!=(const CStringToken& t1, const CStringToken& t2)
{
    return !operator==(t1, t2);
}
bool afl::operator!=(const CStringTokenAliases& a1, const CStringTokenAliases& a2)
{
    return !operator==(a1, a2);
}
bool afl::operator!=(const CStringTokenGroup& g1, const CStringTokenGroup& g2)
{
    return !operator==(g1, g2);
}

bool afl::equal(const CStringToken* t1, const CStringToken* t2)
{
    if(t1 == nullptr || t2 == nullptr)
        return t1 == t2;
    return *t1 == *t2;
}
bool afl::equal(const CStringTokenAliases* a1, const CStringTokenAliases* a2)
{
    if(a1 == nullptr || a2 == nullptr || a1 == a2)
        return a1 == a2;
    return *a1 == *a2;
}
bool afl::equal(const CStringTokenGroup* g1, const CStringTokenGroup* g2)
{
    if(g1 == nullptr || g2 == nullptr || g1 == g2)
        return g1 == g2;
    return *g1 == *g2;
}

void afl::free(afl::CStringToken* cToken)
{
    if(cToken == nullptr)
        return;
    free(cToken->string);
    cToken->freeFunction(cToken);
}
void afl::free(afl::CStringTokenAliases* cTokenAliases)
{
    if(cTokenAliases == nullptr)
        return;
    if(cTokenAliases->aliases != nullptr) {
        for(size_t i = 0; i < cTokenAliases->aliasesCount; ++i)
            free(cTokenAliases->aliases[i]);
    }
    cTokenAliases->freeFunction(cTokenAliases->aliases);
    cTokenAliases->freeFunction(cTokenAliases);
}
void afl::free(afl::CStringTokenGroup* cTokenGroup)
{
    if(cTokenGroup == nullptr)
        return;
    free(cTokenGroup->token);
    cTokenGroup->freeFunction(cTokenGroup->groupID);
    cTokenGroup->freeFunction(cTokenGroup);
}

afl::CStringToken* afl::convert(const Token<std::string>& token)
{
    CStringToken tmpCToken = {apl::freeMemory, convert(token.value), token.type, token.precedence, token.parameterCount, token.associativity};
    if(tmpCToken.string == nullptr)
        return nullptr;
    auto cToken = static_cast<CStringToken*>(apl::allocateMemory(sizeof(CStringToken)));
    if(cToken != nullptr)
        std::memcpy(cToken, &tmpCToken, sizeof(CStringToken));
    return cToken;
}
std::shared_ptr<afl::Token<std::string>> afl::convert(CStringToken* cToken, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&))
{
    std::shared_ptr<Token<std::string>> token = convert(const_cast<const CStringToken*>(cToken), getTokenFunction);
    free(cToken);
    return token;
}
std::shared_ptr<afl::Token<std::string>> afl::convert(const CStringToken* cToken, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&))
{
    if(cToken == nullptr)
        return std::shared_ptr<Token<std::string>>();
    std::shared_ptr<Token<std::string>> token;
    const char* tokenCString = (cToken->string == nullptr) ? "" : cToken->string->string;
    if(getTokenFunction != nullptr)
        token = getTokenFunction(tokenCString);
    if(token == nullptr)
        token = std::make_shared<Token<std::string>>(tokenCString, cToken->type, cToken->precedence, cToken->parameterCount, cToken->associativity);
    return token;
}

afl::CStringTokenAliases* afl::convert(const TokenAliases<std::string>& tokenAliases)
{
    CStringTokenAliases tmpCAliases = {apl::freeMemory, tokenAliases.type, tokenAliases.aliases.size(), static_cast<CString**>(apl::allocateMemory(sizeof(void*) * tokenAliases.aliases.size()))};
    if(tmpCAliases.aliases == nullptr)
        return nullptr;
    auto cAliases = static_cast<CStringTokenAliases *>(apl::allocateMemory(sizeof(CStringTokenAliases)));
    if(cAliases != nullptr) {
        memcpy(cAliases, &tmpCAliases, sizeof(CStringTokenAliases));
        size_t index = 0;
        for (const std::string &s : tokenAliases.aliases) {
            cAliases->aliases[index] = convert(s);
            if(cAliases->aliases[index++] == nullptr) {
                cAliases->aliasesCount = index - 1;
                free(cAliases);
                return nullptr;
            }
        }
    }
    return cAliases;
}
afl::TokenAliases<std::string> afl::convert(CStringTokenAliases* cTokenAliases)
{
    TokenAliases<std::string> aliases = convert(const_cast<const CStringTokenAliases*>(cTokenAliases));
    free(cTokenAliases);
    return aliases;
}
afl::TokenAliases<std::string> afl::convert(const CStringTokenAliases* cTokenAliases)
{
    TokenAliases<std::string> aliases(TokenAliasType::String);
    if(cTokenAliases == nullptr)
        return aliases;
    aliases.type = cTokenAliases->type;
    if(cTokenAliases->aliases != nullptr) {
        aliases.aliases.reserve(cTokenAliases->aliasesCount);
        for(size_t i = 0; i < cTokenAliases->aliasesCount; ++i)
            aliases.append(cTokenAliases->aliases[i]->string);
    }
    return aliases;
}

afl::CStringTokenGroup* afl::convert(const TokenGroup<std::string>& tokenGroup)
{
    CStringTokenGroup tmpCTokenGroup = {apl::freeMemory, tokenGroup.isToken() ? convert(*tokenGroup.token) : nullptr, tokenGroup.groupID.size(), static_cast<size_t*>(apl::allocateMemory(sizeof(size_t) * tokenGroup.groupID.size()))};
    if(tmpCTokenGroup.groupID == nullptr && tmpCTokenGroup.groupIDSize > 0)
        return nullptr;
    auto cTokenGroup = static_cast<CStringTokenGroup*>(apl::allocateMemory(sizeof(CStringTokenGroup)));
    if(cTokenGroup != nullptr) {
        memcpy(cTokenGroup, &tmpCTokenGroup, sizeof(tmpCTokenGroup));
        for (size_t i = 0; i < cTokenGroup->groupIDSize; ++i)
            cTokenGroup->groupID[i] = tokenGroup.groupID[i];
    }
    return cTokenGroup;
}
afl::TokenGroup<std::string> afl::convert(CStringTokenGroup* cTokenGroup, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&))
{
    TokenGroup<std::string> tokenGroup = convert(const_cast<const CStringTokenGroup*>(cTokenGroup), getTokenFunction);
    free(cTokenGroup);
    return tokenGroup;
}
afl::TokenGroup<std::string> afl::convert(const CStringTokenGroup* cTokenGroup, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&))
{
    if(cTokenGroup == nullptr)
        return TokenGroup<std::string>(convert(static_cast<const CStringToken*>(nullptr)));
    size_t groupIDSize = (cTokenGroup->groupID == nullptr) ? 0 : cTokenGroup->groupIDSize;
    return TokenGroup<std::string>(convert(const_cast<const CStringToken*>(cTokenGroup->token), getTokenFunction), std::vector<size_t>(cTokenGroup->groupID, cTokenGroup->groupID + groupIDSize));
}

#endif //AFUNCTIONLIBRARYSDK_CSTRINGTOKEN_HPP

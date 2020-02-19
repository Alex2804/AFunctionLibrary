#ifndef AFUNCTIONLIBRARY_CSTRINGTOKEN_HPP
#define AFUNCTIONLIBRARY_CSTRINGTOKEN_HPP

#include "cstring.hpp"
#include "include/AFunctionLibrary/token.h"

namespace afl
{
    extern "C"
    {
        struct CStringToken
        {
            const FreeMemoryFunction freeFunction;
            CString* string;
            TokenType type;
            size_t precedence;
            size_t parameterCount;
            TokenAssociativity associativity;
        };

        struct CStringTokenAliases
        {
            const FreeMemoryFunction freeFunction;
            TokenAliasType type;
            size_t aliasesCount;
            CString** aliases;
        };

        struct CStringTokenGroup
        {
            const FreeMemoryFunction freeFunction;
            CStringToken* token;
            size_t groupIDSize;
            size_t* groupID;
        };
    }

    inline AFUNCTIONLIBRARY_EXPORT bool operator==(const CStringToken& t1, const CStringToken& t2);
    inline AFUNCTIONLIBRARY_EXPORT bool operator==(const CStringTokenAliases& a1, const CStringTokenAliases& a2);
    inline AFUNCTIONLIBRARY_EXPORT bool operator==(const CStringTokenGroup& g1, const CStringTokenGroup& g2);

    inline AFUNCTIONLIBRARY_EXPORT bool operator!=(const CStringToken& t1, const CStringToken& t2);
    inline AFUNCTIONLIBRARY_EXPORT bool operator!=(const CStringTokenAliases& a1, const CStringTokenAliases& a2);
    inline AFUNCTIONLIBRARY_EXPORT bool operator!=(const CStringTokenGroup& g1, const CStringTokenGroup& g2);

    inline AFUNCTIONLIBRARY_EXPORT bool equal(const CStringToken* t1, const CStringToken* t2);
    inline AFUNCTIONLIBRARY_EXPORT bool equal(const CStringTokenAliases* a1, const CStringTokenAliases* a2);
    inline AFUNCTIONLIBRARY_EXPORT bool equal(const CStringTokenGroup* g1, const CStringTokenGroup* g2);

    inline AFUNCTIONLIBRARY_NO_EXPORT void free(CStringToken* cToken);
    inline AFUNCTIONLIBRARY_NO_EXPORT void free(CStringTokenAliases* cTokenAliases);
    inline AFUNCTIONLIBRARY_NO_EXPORT void free(CStringTokenGroup* cTokenGroup);

    inline AFUNCTIONLIBRARY_NO_EXPORT CStringToken* convert(const Token<std::string>& token);
    inline AFUNCTIONLIBRARY_NO_EXPORT std::shared_ptr<Token<std::string>> convert(CStringToken* cToken, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&)=nullptr);

    inline AFUNCTIONLIBRARY_NO_EXPORT CStringTokenAliases* convert(const TokenAliases<std::string>& tokenAliases);
    inline AFUNCTIONLIBRARY_NO_EXPORT TokenAliases<std::string> convert(CStringTokenAliases* cTokenAliases);

    inline AFUNCTIONLIBRARY_NO_EXPORT CStringTokenGroup* convert(const TokenGroup<std::string>& tokenGroup);
    inline AFUNCTIONLIBRARY_NO_EXPORT TokenGroup<std::string> convert(CStringTokenGroup* cTokenGroup, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&)=nullptr);
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
    for(size_t i = 0; i < cTokenAliases->aliasesCount; ++i)
        free(cTokenAliases->aliases[i]);
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
    afl::CStringToken tmpCToken = {apl::freeMemory, convert(token.value), token.type, token.precedence, token.parameterCount, token.associativity};
    auto cToken = static_cast<CStringToken*>(apl::allocateMemory(sizeof(CStringToken)));
    memcpy(cToken, &tmpCToken, sizeof(afl::CStringToken));
    return cToken;
}
std::shared_ptr<afl::Token<std::string>> afl::convert(CStringToken* cToken, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&))
{
    if(cToken == nullptr)
        return std::shared_ptr<Token<std::string>>();
    std::shared_ptr<Token<std::string>> token;
    if(cToken->string == nullptr)
        cToken->string = convert("");
    if(getTokenFunction != nullptr)
        token = getTokenFunction(cToken->string->string);
    if(token == nullptr) {
        token = std::make_shared<Token<std::string>>(convert(cToken->string), cToken->type, cToken->precedence, cToken->parameterCount, cToken->associativity);
        cToken->string = nullptr;
    }
    free(cToken);
    return token;
}

afl::CStringTokenAliases* afl::convert(const TokenAliases<std::string>& tokenAliases)
{
    CStringTokenAliases tmpCAliases = {apl::freeMemory, tokenAliases.type, tokenAliases.aliases.size(), static_cast<CString**>(apl::allocateMemory(sizeof(void*) * tokenAliases.aliases.size()))};
    auto cAliases = static_cast<CStringTokenAliases*>(apl::allocateMemory(sizeof(CStringTokenAliases)));
    memcpy(cAliases, &tmpCAliases, sizeof(CStringTokenAliases));
    size_t index = 0;
    for(const std::string& s : tokenAliases.aliases)
        cAliases->aliases[index++] = convert(s);
    return cAliases;
}
afl::TokenAliases<std::string> afl::convert(CStringTokenAliases* cTokenAliases)
{
    afl::TokenAliases<std::string> aliases(afl::TokenAliasType::String);
    if(cTokenAliases == nullptr)
        return aliases;
    aliases.type = cTokenAliases->type;
    if(cTokenAliases->aliases == nullptr)
        cTokenAliases->aliasesCount = 0;
    aliases.aliases.reserve(cTokenAliases->aliasesCount);
    for(size_t i = 0; i < cTokenAliases->aliasesCount; ++i)
        aliases.append(cTokenAliases->aliases[i]->string);
    free(cTokenAliases);
    return aliases;
}

afl::CStringTokenGroup* afl::convert(const TokenGroup<std::string>& tokenGroup)
{
    CStringTokenGroup tmpCTokenGroup = {apl::freeMemory, tokenGroup.isToken() ? convert(*tokenGroup.token) : nullptr, tokenGroup.groupID.size(), static_cast<size_t*>(apl::allocateMemory(sizeof(size_t) * tokenGroup.groupID.size()))};
    auto cTokenGroup = static_cast<CStringTokenGroup*>(apl::allocateMemory(sizeof(CStringTokenGroup)));
    memcpy(cTokenGroup, &tmpCTokenGroup, sizeof(tmpCTokenGroup));
    for(size_t i = 0; i < cTokenGroup->groupIDSize; ++i)
        cTokenGroup->groupID[i] = tokenGroup.groupID[i];
    return cTokenGroup;
}
afl::TokenGroup<std::string> afl::convert(CStringTokenGroup* cTokenGroup, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&))
{
    if(cTokenGroup == nullptr)
        return TokenGroup<std::string>(convert(static_cast<afl::CStringToken*>(nullptr)));
    if(cTokenGroup->groupID == nullptr)
        cTokenGroup->groupIDSize = 0;
    TokenGroup<std::string> tokenGroup(convert(cTokenGroup->token, getTokenFunction), std::vector<size_t>(cTokenGroup->groupID, cTokenGroup->groupID + cTokenGroup->groupIDSize));
    cTokenGroup->token = nullptr;
    free(cTokenGroup);
    return tokenGroup;
}

#endif //AFUNCTIONLIBRARY_CSTRINGTOKEN_HPP

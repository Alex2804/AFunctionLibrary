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
            CString* string;
            TokenType type;
            int precedence;
            int parameterCount;
            TokenAssociativity associativity;
        };

        struct CStringTokenAliases
        {
            TokenAliasType type;
            CString** aliases;
            size_t aliasesCount;
            FreeMemoryFunction freeFunction;
        };

        struct CStringTokenGroup
        {
            CStringToken* token;
            size_t* groupID;
            size_t groupIDSize;
        };
    }

    inline AFUNCTIONLIBRARY_EXPORT bool operator==(const CStringToken& t1, const CStringToken& t2);
    inline AFUNCTIONLIBRARY_EXPORT bool operator==(const CStringTokenAliases& a1, const CStringTokenAliases& a2);
    inline AFUNCTIONLIBRARY_EXPORT bool operator==(const CStringTokenGroup& g1, const CStringTokenGroup& g2);

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


bool afl::operator==(const afl::CStringToken& t1, const afl::CStringToken& t2)
{
    return equal(t1.string, t2.string) && t1.type == t2.type && t1.precedence == t2.precedence
           && t1.parameterCount == t2.parameterCount && t1.associativity == t2.associativity;
}
bool afl::operator==(const afl::CStringTokenAliases& a1, const afl::CStringTokenAliases& a2)
{
    if(a1.type != a2.type || a1.aliasesCount != a2.aliasesCount)
        return false;
    for(size_t i = 0; i < a1.aliasesCount; ++i) {
        if(!equal(a1.aliases[i], a2.aliases[i]))
            return false;
    }
    return true;
}
bool afl::operator==(const afl::CStringTokenGroup& g1, const afl::CStringTokenGroup& g2)
{
    if(g1.groupIDSize != g2.groupIDSize || !equal(g1.token, g2.token))
        return false;
    for(size_t i = 0; i < g1.groupIDSize; ++i) {
        if(g1.groupID[i] != g2.groupID[i])
            return false;
    }
    return true;
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

void afl::free(struct afl::CStringToken* cToken)
{
    FreeMemoryFunction freeFunction = cToken->string->freeFunction;
    free(cToken->string);
    freeFunction(cToken);
}
void afl::free(struct afl::CStringTokenAliases* cTokenAliases)
{
    for(size_t i = 0; i < cTokenAliases->aliasesCount; i++)
        free(cTokenAliases->aliases[i]);
    cTokenAliases->freeFunction(cTokenAliases->aliases);
    cTokenAliases->freeFunction(cTokenAliases);
}
void afl::free(struct afl::CStringTokenGroup* cTokenGroup)
{
    FreeMemoryFunction freeFunction = cTokenGroup->token->string->freeFunction;
    free(cTokenGroup->token);
    freeFunction(cTokenGroup->groupID);
    freeFunction(cTokenGroup);
}

inline afl::CStringToken* afl::convert(const Token<std::string>& token)
{
    auto cToken = static_cast<CStringToken*>(apl::allocateMemory(sizeof(CStringToken)));
    cToken->string = convert(token.value);
    cToken->type = token.type;
    cToken->precedence = token.precedence;
    cToken->parameterCount = token.parameterCount;
    cToken->associativity = token.associativity;
    return cToken;
}
inline std::shared_ptr<afl::Token<std::string>> afl::convert(CStringToken* cToken, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&))
{
    if(cToken == nullptr)
        return std::make_shared<Token<std::string>>();
    std::shared_ptr<Token<std::string>> token;
    if(getTokenFunction != nullptr)
        token = getTokenFunction(cToken->string->string);
    if(token == nullptr)
        token = std::make_shared<Token<std::string>>(cToken->string->string, cToken->type, cToken->precedence, cToken->parameterCount, cToken->associativity);
    free(cToken);
    return token;
}

inline afl::CStringTokenAliases* afl::convert(const TokenAliases<std::string>& tokenAliases)
{
    auto cTokenAliases = static_cast<CStringTokenAliases*>(apl::allocateMemory(sizeof(CStringTokenAliases)));
    cTokenAliases->type = tokenAliases.type;
    cTokenAliases->aliasesCount = tokenAliases.aliases.size();
    cTokenAliases->aliases = static_cast<CString**>(apl::allocateMemory(sizeof(void*) * cTokenAliases->aliasesCount));
    cTokenAliases->freeFunction = apl::freeMemory;
    size_t index = 0;
    for(const std::string& s : tokenAliases.aliases)
        cTokenAliases->aliases[index++] = convert(s);
    return cTokenAliases;
}
inline afl::TokenAliases<std::string> afl::convert(CStringTokenAliases* cTokenAliases)
{
    afl::TokenAliases<std::string> aliases(afl::TokenAliasType::String);
    if(cTokenAliases == nullptr)
        return aliases;
    aliases.type = cTokenAliases->type;
    aliases.aliases.reserve(cTokenAliases->aliasesCount);
    for(size_t i = 0; i < cTokenAliases->aliasesCount; i++)
        aliases.append(cTokenAliases->aliases[i]->string);
    free(cTokenAliases);
    return aliases;
}

inline afl::CStringTokenGroup* afl::convert(const TokenGroup<std::string>& tokenGroup)
{
    auto cTokenGroup = static_cast<CStringTokenGroup*>(apl::allocateMemory(sizeof(CStringTokenGroup)));
    cTokenGroup->token = convert(*tokenGroup.token);
    cTokenGroup->groupIDSize = tokenGroup.groupID.size();
    cTokenGroup->groupID = static_cast<size_t*>(apl::allocateMemory(sizeof(size_t) * cTokenGroup->groupIDSize));
    for(size_t i = 0; i < cTokenGroup->groupIDSize; i++)
        cTokenGroup->groupID[i] = tokenGroup.groupID[i];
    return cTokenGroup;
}
inline afl::TokenGroup<std::string> afl::convert(CStringTokenGroup* cTokenGroup, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&))
{
    TokenGroup<std::string> tokenGroup(convert(cTokenGroup->token, getTokenFunction), std::vector<size_t>(cTokenGroup->groupID, cTokenGroup->groupID + cTokenGroup->groupIDSize));
    free(cTokenGroup);
    return tokenGroup;
}

#endif //AFUNCTIONLIBRARY_CSTRINGTOKEN_HPP

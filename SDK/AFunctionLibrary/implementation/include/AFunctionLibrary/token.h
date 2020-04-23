#ifndef AFUNCTIONLIBRARYSDK_TOKEN_H
#define AFUNCTIONLIBRARYSDK_TOKEN_H

#include "implementation/exports.h"

#include <algorithm>
#include <iterator>
#include <vector>
#include <memory>

#include "utilites.h"
#include "tokenenums.h"

namespace afl
{
    template<typename T>
    class Token
    {
    public:
        Token();
        Token(T value, TokenType type, int precedence, int parameterCount, TokenAssociativity associativity);
        Token(const Token<T>& other);
        Token(Token<T>&& other) noexcept;
        virtual ~Token();

        Token<T>& operator=(const Token<T>& other);
        Token<T>& operator=(Token<T>&& other) noexcept;

        bool operator==(const Token<T>& other) const;
        bool operator!=(const Token<T>& other) const;

        bool requirePrecedence();
        bool requireAssociativity();
        bool requireParameterCount();

        const T& getValue() const;
        T& getValue();
        TokenType getType() const;
        size_t getPrecedence() const;
        size_t getParameterCount() const;
        TokenAssociativity getAssociativity() const;

        T value;
        TokenType type;
        size_t precedence;
        size_t parameterCount;
        TokenAssociativity associativity;
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Token<T>& token);


    template<typename T>
    class TokenAliases
    {
    public:
        TokenAliases();
        explicit TokenAliases(TokenAliasType type);
        explicit TokenAliases(std::vector<T> aliases);
        TokenAliases(TokenAliasType type, std::vector<T> aliases);
        TokenAliases(const TokenAliases<T>& other);
        TokenAliases(TokenAliases<T>&& other) noexcept;
        ~TokenAliases();

        TokenAliases<T>& operator=(const TokenAliases<T>& other);
        TokenAliases<T>& operator=(TokenAliases<T>&& other) noexcept;

        bool operator==(const TokenAliases& other) const;
        bool operator!=(const TokenAliases& other) const;

        TokenAliases<T>& append(const TokenAliases<T>& tokenAliases);
        TokenAliases<T>& append(std::vector<T> aliasVector);
        TokenAliases<T>& append(T alias);

        TokenAliasType type = TokenAliasType::String;
        std::vector<T> aliases;
    };


    template<typename T>
    class TokenGroup
    {
    public:
        TokenGroup();
        explicit TokenGroup(std::shared_ptr<const Token<T>> token);
        explicit TokenGroup(std::vector<size_t> groupID);
        TokenGroup(std::shared_ptr<const Token<T>> token, std::vector<size_t> groupID);
        TokenGroup(const TokenGroup<T>& other);
        TokenGroup(TokenGroup<T>&& other) noexcept;
        ~TokenGroup();

        TokenGroup<T>& operator=(const TokenGroup<T>& other);
        TokenGroup<T>& operator=(TokenGroup<T>&& other) noexcept;

        bool operator==(const TokenGroup<T>& other) const;
        bool operator!=(const TokenGroup<T>& other) const;

        bool isToken() const;

        const T& getValue() const;
        T& getValue();
        TokenType getType() const;
        size_t getPrecedence() const;
        size_t getParameterCount() const;
        TokenAssociativity getAssociativity() const;

        std::shared_ptr<const Token<T>> token;
        std::vector<size_t> groupID;
        bool printGroupID = true;
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const TokenGroup<T>& tokenGroup);
}

#include "implementation/token.tpp"

#endif //AFUNCTIONLIBRARYSDK_TOKEN_H

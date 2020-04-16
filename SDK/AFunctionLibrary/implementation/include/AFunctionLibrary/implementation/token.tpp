#ifndef AFUNCTIONLIBRARYSDK_TOKEN_TPP
#define AFUNCTIONLIBRARYSDK_TOKEN_TPP

// ====================================================== Token ===================================================== //

template<typename T>
afl::Token<T>::Token()
    : type(TokenType::Constant), precedence(0), parameterCount(0), associativity(TokenAssociativity::None)
{}
template<typename T>
afl::Token<T>::Token(T value, afl::TokenType type, int precedence, int parameterCount, afl::TokenAssociativity associativity)
    : value(std::move(value)), type(type), precedence(precedence)
    , parameterCount(parameterCount), associativity(associativity)
{}
template<typename T>
afl::Token<T>::Token(const afl::Token<T>& other) = default;
template<typename T>
afl::Token<T>::Token(afl::Token<T>&& other) noexcept = default;
template<typename T>
afl::Token<T>::~Token() = default;

template<typename T>
afl::Token<T>& afl::Token<T>::operator=(const afl::Token<T>& other) = default;
template<typename T>
afl::Token<T>& afl::Token<T>::operator=(afl::Token<T>&& other) noexcept = default;

template<typename T>
bool afl::Token<T>::operator==(const afl::Token<T> &other) const
{
    return value == other.value && type == other.type && precedence == other.precedence
        && parameterCount == other.parameterCount && associativity == other.associativity;
}
template<typename T>
bool afl::Token<T>::operator!=(const afl::Token<T> &other) const
{
    return !operator==(other);
}

template<typename T>
bool afl::Token<T>::requirePrecedence()
{
    return type == TokenType::Operator;
}
template<typename T>
bool afl::Token<T>::requireAssociativity()
{
    return type == TokenType::Operator;
}
template<typename T>
bool afl::Token<T>::requireParameterCount()
{
    return type == TokenType::Function;
}

template<typename T>
const T& afl::Token<T>::getValue() const
{
    return value;
}
template<typename T>
T& afl::Token<T>::getValue()
{
    return value;
}
template<typename T>
afl::TokenType afl::Token<T>::getType() const
{
    return type;
}

template<typename T>
size_t afl::Token<T>::getPrecedence() const
{
    return precedence;
}

template<typename T>
size_t afl::Token<T>::getParameterCount() const
{
    return parameterCount;
}
template<typename T>
afl::TokenAssociativity afl::Token<T>::getAssociativity() const
{
    return associativity;
}

template<typename T>
std::ostream& afl::operator<<(std::ostream& os, const Token<T>& token)
{
    return os << stringify(token.getValue());
}


// ================================================== TokenAliases ================================================== //


template<typename T>
afl::TokenAliases<T>::TokenAliases(afl::TokenAliasType type)
    : type(type)
{}
template<typename T>
afl::TokenAliases<T>::TokenAliases(std::vector<T> aliases)
{
    append(std::move(aliases));
}
template<typename T>
afl::TokenAliases<T>::TokenAliases(afl::TokenAliasType type, std::vector<T> aliases) : TokenAliases(type)
{
    append(std::move(aliases));
}
template<typename T>
afl::TokenAliases<T>::TokenAliases() = default;
template<typename T>
afl::TokenAliases<T>::TokenAliases(const afl::TokenAliases<T>& other) = default;
template<typename T>
afl::TokenAliases<T>::TokenAliases(afl::TokenAliases<T>&& other) noexcept = default;
template<typename T>
afl::TokenAliases<T>::~TokenAliases() = default;

template<typename T>
afl::TokenAliases<T>& afl::TokenAliases<T>::operator=(const afl::TokenAliases<T>& other) = default;
template<typename T>
afl::TokenAliases<T>& afl::TokenAliases<T>::operator=(afl::TokenAliases<T>&& other) noexcept = default;

template<typename T>
bool afl::TokenAliases<T>::operator==(const TokenAliases &other) const
{
    return type == other.type && aliases == other.aliases;
}
template<typename T>
bool afl::TokenAliases<T>::operator!=(const TokenAliases &other) const
{
    return !operator==(other);
}

template<typename T>
afl::TokenAliases<T> &afl::TokenAliases<T>::append(const afl::TokenAliases<T>& tokenAliases)
{
    if(type == tokenAliases.type)
        append(tokenAliases.aliases);
    return *this;
}
template<typename T>
afl::TokenAliases<T> &afl::TokenAliases<T>::append(std::vector<T> aliasVector)
{
    aliases.reserve(aliases.size() + aliasVector.size());
    for(std::string& alias : aliasVector) {
        if(std::find(aliases.begin(), aliases.end(), alias) == aliases.end())
            aliases.emplace_back(std::move(alias));
    }
    aliases.shrink_to_fit();
    return *this;
}
template<typename T>
afl::TokenAliases<T> &afl::TokenAliases<T>::append(T alias)
{
    if(std::find<decltype(aliases.begin()), T>(aliases.begin(), aliases.end(), alias) == aliases.end())
        aliases.emplace_back(std::move(alias));
    return *this;
}


// =================================================== TokenGroup =================================================== //


template<typename T>
afl::TokenGroup<T>::TokenGroup(std::shared_ptr<const Token<T>> token)
        : token(std::move(token))
{}
template<typename T>
afl::TokenGroup<T>::TokenGroup(std::vector<size_t> groupID)
        : groupID(std::move(groupID))
{}
template<typename T>
afl::TokenGroup<T>::TokenGroup(std::shared_ptr<const Token <T>> token, std::vector<size_t> groupID)
        : token(std::move(token)), groupID(std::move(groupID))
{}
template<typename T>
afl::TokenGroup<T>::TokenGroup() = default;
template<typename T>
afl::TokenGroup<T>::TokenGroup(const afl::TokenGroup<T>& other) = default;
template<typename T>
afl::TokenGroup<T>::TokenGroup(afl::TokenGroup<T>&& other) noexcept = default;
template<typename T>
afl::TokenGroup<T>::~TokenGroup() = default;

template<typename T>
afl::TokenGroup<T>& afl::TokenGroup<T>::operator=(const afl::TokenGroup<T>& other) = default;
template<typename T>
afl::TokenGroup<T>& afl::TokenGroup<T>::operator=(TokenGroup<T>&& other) noexcept = default;

template<typename T>
bool afl::TokenGroup<T>::operator==(const TokenGroup<T>& other) const
{
    return ((isToken() && other.isToken() && *token == *other.token) || token == other.token) && groupID == other.groupID;
}
template<typename T>
bool afl::TokenGroup<T>::operator!=(const TokenGroup<T>& other) const
{
    return !operator==(other);
}

template<typename T>
bool afl::TokenGroup<T>::isToken() const
{
    return token != nullptr;
}

template<typename T>
const T& afl::TokenGroup<T>::getValue() const
{
    return isToken() ? token->getValue() : T();
}
template<typename T>
T& afl::TokenGroup<T>::getValue()
{
    return isToken() ? token->getValue() : T();
}
template<typename T>
afl::TokenType afl::TokenGroup<T>::getType() const
{
    return isToken() ? token->getType() : TokenType::Constant;
}
template<typename T>
size_t afl::TokenGroup<T>::getPrecedence() const
{
    return isToken() ? token->getPrecedence() : 0;
}
template<typename T>
size_t afl::TokenGroup<T>::getParameterCount() const
{
    return isToken() ? token->getParameterCount() : 0;
}
template<typename T>
afl::TokenAssociativity afl::TokenGroup<T>::getAssociativity() const
{
    return isToken() ? token->getAssociativity() : TokenAssociativity::None;
}

template<typename T>
std::ostream& afl::operator<<(std::ostream& os, const TokenGroup<T>& tokenGroup)
{
    if(tokenGroup.printGroupID)
        os << "[";
    if(tokenGroup.isToken())
        os << *tokenGroup.token;
    else
        os << "nullptr";
    if(tokenGroup.printGroupID) {
        os << " {";
        for(size_t i = 0; i < tokenGroup.groupID.size(); ++i) {
            if(i > 0)
                os << ", ";
            os << std::to_string(tokenGroup.groupID[i]);
        }
        os << "}]";
    }
    return os;
}

#endif //AFUNCTIONLIBRARYSDK_TOKEN_TPP

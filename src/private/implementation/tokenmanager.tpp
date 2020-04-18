#ifndef AFUNCTIONLIBRARY_TOKENMANAGER_TPP
#define AFUNCTIONLIBRARY_TOKENMANAGER_TPP

template<typename T>
std::vector<std::shared_ptr<const afl::detail::TokenPtrBundle<std::string>>> afl::detail::TokenManager::filterTokens(const T& filterFunction) const
{
    std::vector<std::shared_ptr<const afl::detail::TokenPtrBundle<std::string>>> filtered;
    filtered.reserve(m_uniqueTokens.size() + m_notUniqueTokens.size());
    for(const auto& pair : m_uniqueTokens) {
        if(filterFunction(pair.second.first->token.get()))
            filtered.push_back(pair.second.first);
    }
    for(const auto& pair : m_notUniqueTokens) {
        if(filterFunction(pair.second.first->token.get()))
            filtered.push_back(pair.second.first);
    }
    filtered.shrink_to_fit();
    return filtered;
}

template<typename T>
std::vector<afl::TokenGroup<T>> afl::detail::toGroupVector(const std::vector<std::shared_ptr<const Token<T>>>& tokens)
{
    std::vector<TokenGroup<std::string>> tokenGroups;
    std::vector<size_t> tmpGroupID = {0};
    for(const std::shared_ptr<const Token<std::string>>& token : tokens) {
        if(token->getType() == TokenType::BracketClose || token->getType() == TokenType::ArgumentDelimiter) {
            tmpGroupID.pop_back();
            ++tmpGroupID.back();
        }
        tokenGroups.emplace_back(token, tmpGroupID);
        ++tmpGroupID.back();
        if(token->getType() == TokenType::BracketOpen || token->getType() == TokenType::ArgumentDelimiter) {
            tmpGroupID.push_back(0);
        }
    }
    return tokenGroups;
}

#endif //AFUNCTIONLIBRARY_TOKENMANAGER_TPP

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

#endif //AFUNCTIONLIBRARY_TOKENMANAGER_TPP

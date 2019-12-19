#include "../tokenextras.h"

std::string afl::detail::tokensToString(const std::vector<std::shared_ptr<const Token<std::string>>>& tokens)
{
    std::string string;
    for(const std::shared_ptr<const Token<std::string>>& token : tokens)
        string += token->getValue();
    return string;
}

std::vector<afl::TokenGroup<std::string>> afl::detail::toGroupVector(const std::vector<std::shared_ptr<const Token<std::string>>>& tokens)
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

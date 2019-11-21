#include "../syntaxtreehandler.h"

afl::detail::SyntaxTreeHandler::SyntaxTreeHandler(std::shared_ptr<ResourceManager> resourceManager)
    : m_resourceManager(std::move(resourceManager))
{}

std::vector<std::shared_ptr<const afl::Token<std::string>>> afl::detail::SyntaxTreeHandler::parseTokens(std::string string)
{
    return std::vector<std::shared_ptr<const Token<std::string>>>();
}

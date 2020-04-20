#include "AFunctionLibrary/createtokenapi.h"
#include "AFunctionLibrary/formatfunctionapi.h"
#include "AFunctionLibrary/optimizefunctionapi.h"

#include <stack>
#include <regex>
#include <memory>
#include <functional>

AFL_REGISTER_CREATE_TOKEN_FEATURE(just_for_testing, tokenValue)
{
    if(std::strlen(tokenValue) == 1 && std::isdigit(tokenValue[0])) {
        return afl::convert(afl::Token<std::string>(tokenValue, afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None));
    }
    return nullptr;
}

AFL_REGISTER_CREATE_TOKEN_FEATURE(just_for_another_test, tokenValue)
{
    if(std::strlen(tokenValue) == 1 && std::isalpha(tokenValue[0])) {
        return afl::convert(afl::Token<std::string>(tokenValue, afl::TokenType::Constant, 0, 0, afl::TokenAssociativity::None));
    }
    return nullptr;
}

AFL_REGISTER_CREATE_TOKEN_FEATURE(for_formatting_test, tokenValue)
{
    if(std::strcmp(tokenValue, "UPPERCASE_TOKEN") == 0 || std::strcmp(tokenValue, "lowercase_token") == 0)
        return afl::convert(afl::Token<std::string>(tokenValue, afl::TokenType::Constant, 0, 0, afl::TokenAssociativity::None));
    return nullptr;
}

AFL_REGISTER_CREATE_TOKEN_ALIASES_FEATURE(just_for_testing, value, type)
{
    if(type == afl::TokenAliasType::String && std::strcmp(value, "1") == 0) {
        afl::TokenAliases<std::string> alias;
        alias.type = type;
        alias.append("one");
        return afl::convert(alias);
    }
    return nullptr;
}

AFL_REGISTER_FORMAT_FUNCTION_STRING_FEATURE(just_for_testing, function)
{
    std::string f = function, toReplace = "UPPERCASE_TOKEN";
    if(f.find(toReplace) != std::string::npos)
        f.replace(f.find(toReplace), toReplace.size(), "lowercase_token");
    return afl::convert(f);
}

AFL_REGISTER_OPTIMIZE_FUNCTION_STRING_FEATURE(just_for_testing, function)
{
    return afl::convert(std::regex_replace(function, std::regex(R"((^|[^\w])1\*1($|[^\w]))"), "$011$02"));
}

AFL_REGISTER_OPTIMIZE_FUNCTION_SYNTAX_TREE_FEATURE(just_for_testing, syntaxTree)
{
    typedef afl::Node<afl::TokenGroup<std::string>> Node;
    Node root = afl::convert(syntaxTree);
    std::stack<Node*> stack;
    stack.push(&root);
    auto zeroToken = std::make_shared<const afl::Token<std::string>>("0", afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None);
    while(!stack.empty()) {
        Node* node = stack.top();
        stack.pop();
        afl::TokenGroup<std::string> tokenGroup = node->m_value;
        if(tokenGroup.isToken() && tokenGroup.token->type == afl::TokenType::Operator &&
           tokenGroup.token->value == "*" && node->m_children.size() == 2 &&
           (node->m_children.front().m_value.token->value == "0" || node->m_children.back().m_value.token->value == "0"))
        {
            node->m_children.clear();
            node->m_value = afl::TokenGroup<std::string>(zeroToken, node->m_value.groupID);
        } else {
            for(Node& n : node->m_children)
                stack.push(&n);
        }
    }
    return afl::convert(root);
}

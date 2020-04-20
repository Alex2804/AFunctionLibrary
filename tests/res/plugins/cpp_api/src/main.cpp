#include "AFunctionLibrary/createtokenapi.h"
#include "AFunctionLibrary/formatfunctionapi.h"
#include "AFunctionLibrary/optimizefunctionapi.h"

#include <regex>
#include <cmath>

AFL_CPP_API_REGISTER_CREATE_TOKEN_FEATURE(just_for_testing, tokenValue)
{
    if(std::all_of(tokenValue.begin(), tokenValue.end(), ::isalpha)) {
        return std::make_shared<afl::Token<std::string>>(tokenValue, afl::TokenType::Constant, 0, 0, afl::TokenAssociativity::None);
    }
    return std::shared_ptr<afl::Token<std::string>>();
}

AFL_CPP_API_REGISTER_CREATE_TOKEN_ALIASES_FEATURE(just_for_testing, value)
{
    std::vector<afl::TokenAliases<std::string>> aliases;
    if(value == "hello") {
        aliases.emplace_back(afl::TokenAliasType::String, std::vector<std::string>({"hello_world"}));
    }
    return aliases;
}

AFL_CPP_API_REGISTER_FORMAT_FUNCTION_STRING_FEATURE(just_for_testing, function)
{
    std::string toReplace = "hello";
    if(function.find(toReplace) != std::string::npos)
        function.replace(function.find(toReplace), toReplace.size(), "hi");
    return function;
}

AFL_CPP_API_REGISTER_OPTIMIZE_FUNCTION_STRING_FEATURE(just_for_testing, function)
{
    return std::regex_replace(function, std::regex(R"((?:(^|[^\w])0\+(\w+))|(?:(\w+)\+0($|[^\w])))"), "$1$2$3$4");
}

AFL_CPP_API_REGISTER_OPTIMIZE_FUNCTION_SYNTAX_TREE_FEATURE(just_for_testing, syntaxTree)
{
    typedef afl::Node<afl::TokenGroup<std::string>> Node;
    Node* root = syntaxTree.root();
    std::stack<Node*> stack;
    stack.push(root);
    while(!stack.empty()) {
        Node* node = stack.top();
        stack.pop();
        afl::TokenGroup<std::string> tokenGroup = node->m_value;
        if(tokenGroup.isToken() && tokenGroup.token->type == afl::TokenType::Function &&
           tokenGroup.token->value == "pow" && node->m_children.size() == 2 &&
           node->m_children.front().m_value.token->type == afl::TokenType::Number &&
           node->m_children.back().m_value.token->type == afl::TokenType::Number)
        {
            double base = std::stod(node->m_children.front().m_value.token->value);
            double exponent = std::stod(node->m_children.back().m_value.token->value);
            auto tmpToken = std::make_shared<const afl::Token<std::string>>(afl::stringify(std::pow(base, exponent)), afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None);
            node->m_children.clear();
            node->m_value = afl::TokenGroup<std::string>(tmpToken, node->m_value.groupID);
        } else {
            for(Node& n : node->m_children)
                stack.push(&n);
        }
    }
    return syntaxTree;
}
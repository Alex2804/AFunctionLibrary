#include "resourceparser.h"

#include <cstring>
#include <list>
#include <algorithm>

afl::TokenAssociativity afl::detail::stringToTokenAssociativity(const std::string& string, TokenAssociativity defaultValue)
{
    if(string == "left")
        return TokenAssociativity::Left;
    else if(string == "none")
        return TokenAssociativity::None;
    else if(string == "right")
        return TokenAssociativity::Right;
    return defaultValue;
}
afl::TokenType afl::detail::stringToTokenType(const std::string& string, TokenType defaultValue)
{
    if(string == "constant")
        return TokenType::Constant;
    else if(string == "number")
        return TokenType::Number;
    else if(string == "operator")
        return TokenType::Operator;
    else if(string == "function")
        return TokenType::Function;
    else if(string == "bracket_open")
        return TokenType::BracketOpen;
    else if(string == "bracket_close")
        return TokenType::BracketClose;
    else if(string == "argument_delimiter")
        return TokenType::ArgumentDelimiter;
    return defaultValue;
}

void afl::detail::parseExtensionRecursive(const pugi::xml_node &node,
                                          std::vector<std::shared_ptr<TokenWrapper<std::string>>> &tokens)
{
    std::string childName;
    for(const pugi::xml_node& child : node.children()) {
        childName = child.name();
        if(childName == "extension") {
            parseExtensionRecursive(child, tokens);
        } else if(childName == "token") {
            TokenBundle<std::string> defaultBundle;
            defaultBundle.token = Token<std::string>("", TokenType::Constant, 0, 0, TokenAssociativity::None);
            defaultBundle.aliases = std::vector<TokenAliases<std::string>>();
            std::unordered_set<std::string> valueSet;
            parseTokensRecursive(child, std::move(defaultBundle), tokens, valueSet);
        }
    }
}


void afl::detail::parseTokenAliases(std::string aliasString, TokenBundle<std::string> &bundle,
                                    afl::TokenAliasType type)
{
    std::vector<std::string> tmpAliases = splitAtSpaces(std::move(aliasString));
    auto iterator = bundle.aliases.begin();
    while(iterator != bundle.aliases.end() && iterator->type != type)
        ++iterator;
    if(iterator == bundle.aliases.end()) {
        bundle.aliases.push_back(TokenAliases<std::string>{type, std::set<std::string>()});
        iterator = --bundle.aliases.end();
    }
    std::copy(tmpAliases.begin(), tmpAliases.end(), std::inserter(iterator->aliases, iterator->aliases.end()));
}
void afl::detail::parseTokensRecursive(const pugi::xml_node &node, TokenBundle<std::string> token,
                                       std::vector<std::shared_ptr<TokenWrapper<std::string>>>& tokens,
                                       std::unordered_set<std::string>& valueSet)
{
    if(std::strcmp(node.name(), "token") != 0) // if name of node is not "token"
        return;

    std::list<pugi::xml_attribute> attributes;
    for(const pugi::xml_attribute& attribute : node.attributes()) {
        if(strcmp(attribute.name(), "value") != 0 && strcmp(attribute.name(), "values") != 0) {
            attributes.push_front(attribute);
            parseTokenField(pugi::xml_node(), attribute.name(), attribute, token, tokens, valueSet);
        } else {
            attributes.push_back(attribute); // "value" should be at the end of the list
        }
    }

    std::string childName;
    for(const pugi::xml_node& child : node.children()) {
        if(strcmp(child.name(), "token") == 0) {
            parseTokensRecursive(child, token, tokens, valueSet);
        } else { // parse node
            if(child.type() == pugi::xml_node_type::node_pcdata)
                childName = "values";
            else
                childName = child.name();
            parseTokenField(child, childName, child.text(), token, tokens, valueSet);
        }
    }

    for(const pugi::xml_attribute& attribute : attributes) {
        std::string name = attribute.name();
        parseTokenField(pugi::xml_node(), attribute.name(), attribute, token, tokens, valueSet); // value is at end of list ... attributes must be assigned first
    }
}

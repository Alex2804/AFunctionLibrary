#include "gtest/gtest.h"

#include <tuple>
#include <algorithm>

#include "../src/private/resourceparser.h"

GTEST_TEST(ResourceParser_Test, stringToTokenType)
{
    // test valid string
    ASSERT_EQ(afl::detail::stringToTokenType("constant", afl::TokenType::Number), afl::TokenType::Constant);
    ASSERT_EQ(afl::detail::stringToTokenType("number", afl::TokenType::Operator), afl::TokenType::Number);
    ASSERT_EQ(afl::detail::stringToTokenType("operator", afl::TokenType::Function), afl::TokenType::Operator);
    ASSERT_EQ(afl::detail::stringToTokenType("function", afl::TokenType::BracketOpen), afl::TokenType::Function);
    ASSERT_EQ(afl::detail::stringToTokenType("bracket_open", afl::TokenType::BracketClose), afl::TokenType::BracketOpen);
    ASSERT_EQ(afl::detail::stringToTokenType("bracket_close", afl::TokenType::ArgumentDelimiter), afl::TokenType::BracketClose);
    ASSERT_EQ(afl::detail::stringToTokenType("argument_delimiter", afl::TokenType::Constant), afl::TokenType::ArgumentDelimiter);

    // test invalid string --> return default
    ASSERT_EQ(afl::detail::stringToTokenType("not constant", afl::TokenType::Number), afl::TokenType::Number);
    ASSERT_EQ(afl::detail::stringToTokenType("not number", afl::TokenType::Operator), afl::TokenType::Operator);
    ASSERT_EQ(afl::detail::stringToTokenType("not operator", afl::TokenType::Function), afl::TokenType::Function);
    ASSERT_EQ(afl::detail::stringToTokenType("not function", afl::TokenType::BracketOpen), afl::TokenType::BracketOpen);
    ASSERT_EQ(afl::detail::stringToTokenType("not bracket_open", afl::TokenType::BracketClose), afl::TokenType::BracketClose);
    ASSERT_EQ(afl::detail::stringToTokenType("not bracket_close", afl::TokenType::ArgumentDelimiter), afl::TokenType::ArgumentDelimiter);
    ASSERT_EQ(afl::detail::stringToTokenType("not argument_delimiter", afl::TokenType::Constant), afl::TokenType::Constant);
}

GTEST_TEST(ResourceParser_Test, stringToTokenAssociativity)
{
    // test valid string
    ASSERT_EQ(afl::detail::stringToTokenAssociativity("left", afl::TokenAssociativity::None), afl::TokenAssociativity::Left);
    ASSERT_EQ(afl::detail::stringToTokenAssociativity("none", afl::TokenAssociativity::Right), afl::TokenAssociativity::None);
    ASSERT_EQ(afl::detail::stringToTokenAssociativity("right", afl::TokenAssociativity::Left), afl::TokenAssociativity::Right);

    // test invalid string --> return default
    ASSERT_EQ(afl::detail::stringToTokenAssociativity("not left", afl::TokenAssociativity::None), afl::TokenAssociativity::None);
    ASSERT_EQ(afl::detail::stringToTokenAssociativity("not none", afl::TokenAssociativity::Right), afl::TokenAssociativity::Right);
    ASSERT_EQ(afl::detail::stringToTokenAssociativity("not right", afl::TokenAssociativity::Left), afl::TokenAssociativity::Left);
}

void testTokenParsingResult(const std::vector<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>>& tokensVector)
{
    ASSERT_EQ(tokensVector.size(), 6);

    //                     value, type, precedence, parameterCount, associativity
    std::vector<std::tuple<std::string, afl::TokenType, size_t, size_t, afl::TokenAssociativity>> tokens = {
            {"+", afl::TokenType::Operator, 1, 0, afl::TokenAssociativity::Left},
            {"-", afl::TokenType::Operator, 1, 0, afl::TokenAssociativity::Left},
            {"*", afl::TokenType::Operator, 2, 0, afl::TokenAssociativity::Left},
            {"/", afl::TokenType::Operator, 2, 0, afl::TokenAssociativity::Left},
            {"^", afl::TokenType::Operator, 2, 0, afl::TokenAssociativity::Right},
            {"abs", afl::TokenType::Function, std::numeric_limits<size_t>::max(), 1, afl::TokenAssociativity::None}
    };
    afl::TokenAliasType stringAlias = afl::TokenAliasType::String, regexAlias = afl::TokenAliasType::Regex;
    //                     count, types, aliases.size(), aliases
    std::vector<std::tuple<size_t, std::vector<afl::TokenAliasType>, std::vector<size_t>, std::vector<std::vector<std::string>>>> aliases = {
            {0, {}, {}, {{}}},
            {0, {}, {}, {{}}},
            {0, {}, {}, {{}}},
            {0, {}, {}, {{}}},
            {0, {}, {}, {{}}},
            {2, {stringAlias, regexAlias}, {2, 1}, {{"absolute", "absolute2"}, {".*absolute.*"}}}
    };

    for(size_t i = 0; i < tokensVector.size(); i++) {
        ASSERT_EQ(tokensVector.at(i)->token->value, std::get<0>(tokens.at(i)));
        ASSERT_EQ(tokensVector.at(i)->token->type, std::get<1>(tokens.at(i)));
        ASSERT_EQ(tokensVector.at(i)->token->precedence, std::get<2>(tokens.at(i)));
        ASSERT_EQ(tokensVector.at(i)->token->parameterCount, std::get<3>(tokens.at(i)));
        ASSERT_EQ(tokensVector.at(i)->token->associativity, std::get<4>(tokens.at(i)));
        ASSERT_EQ(tokensVector.at(i)->aliases.size(), std::get<0>(aliases.at(i)));
        for(size_t j = 0; j < tokensVector.at(i)->aliases.size(); j++) {
            ASSERT_EQ(tokensVector.at(i)->aliases.at(j).type, std::get<1>(aliases.at(i)).at(j));
            ASSERT_EQ(tokensVector.at(i)->aliases.at(j).aliases.size(), std::get<2>(aliases.at(i)).at(j));
            std::vector<std::string> actualVector = tokensVector.at(i)->aliases.at(j).aliases;
            std::vector<std::string> expectedVector = std::get<3>(aliases.at(i)).at(j);
            std::sort(actualVector.begin(), actualVector.end());
            std::sort(expectedVector.begin(), expectedVector.end());
            ASSERT_EQ(actualVector, expectedVector);
        }
    }
}

GTEST_TEST(ResourceParser_Test, parseTokensRecursive)
{
    std::string path = "res/extensions/test/tokenSample.xml";
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str());
    ASSERT_TRUE(result);
    std::vector<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>> tokensVector;
    afl::detail::TokenInstanceBundle<std::string> defaultBundle;
    defaultBundle.token = afl::Token<std::string>("", afl::TokenType::Constant, 0, 0, afl::TokenAssociativity::None);
    defaultBundle.aliases = std::vector<afl::TokenAliases<std::string>>();
    std::unordered_set<std::string> valueSet;
    afl::detail::parseTokensRecursive(doc.first_child().first_child(), defaultBundle, tokensVector, valueSet);
    testTokenParsingResult(tokensVector);
}


GTEST_TEST(ResourceParser_Test, parseExtensionRecursive_Tokens)
{
    std::string path = "res/extensions/test/tokenSample.xml";
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str());
    ASSERT_TRUE(result);
    std::vector<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>> tokensVector;
    afl::detail::parseExtensionRecursive(doc, tokensVector);
    testTokenParsingResult(tokensVector);
}

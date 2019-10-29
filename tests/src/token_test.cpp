#include "gtest/gtest.h"

#include "AFunctionLibrary/token.h"

GTEST_TEST(Token_Test, constructor)
{
    afl::Token<int> intToken(42, afl::TokenType::Operator, 12, 3, afl::TokenAssociativity::Left);
    ASSERT_EQ(intToken.value, 42);
    ASSERT_EQ(intToken.type, afl::TokenType::Operator);
    ASSERT_EQ(intToken.precedence, 12);
    ASSERT_EQ(intToken.parameterCount, 3);
    ASSERT_EQ(intToken.associativity, afl::TokenAssociativity::Left);

    afl::Token<std::string> stringToken("Hello World!", afl::TokenType::Function, 923, 1, afl::TokenAssociativity::None);
    ASSERT_EQ(stringToken.value, "Hello World!");
    ASSERT_EQ(stringToken.type, afl::TokenType::Function);
    ASSERT_EQ(stringToken.precedence, 923);
    ASSERT_EQ(stringToken.parameterCount, 1);
    ASSERT_EQ(stringToken.associativity, afl::TokenAssociativity::None);
}

GTEST_TEST(Token_Test, requirePrecedence)
{
    afl::Token<std::string> token;
    token.type = afl::TokenType::Operator;
    ASSERT_TRUE(token.requirePrecedence());

    token.type = afl::TokenType::Function;
    ASSERT_FALSE(token.requirePrecedence());
    token.type = afl::TokenType::Constant;
    ASSERT_FALSE(token.requirePrecedence());
    token.type = afl::TokenType::ArgumentDelimiter;
    ASSERT_FALSE(token.requirePrecedence());
    token.type = afl::TokenType::BracketOpen;
    ASSERT_FALSE(token.requirePrecedence());
    token.type = afl::TokenType::BracketClose;
    ASSERT_FALSE(token.requirePrecedence());
    token.type = afl::TokenType::Number;
    ASSERT_FALSE(token.requirePrecedence());
}

GTEST_TEST(Token_Test, requireAssociativity)
{
    afl::Token<std::string> token;
    token.type = afl::TokenType::Operator;
    ASSERT_TRUE(token.requireAssociativity());

    token.type = afl::TokenType::Function;
    ASSERT_FALSE(token.requireAssociativity());
    token.type = afl::TokenType::Constant;
    ASSERT_FALSE(token.requireAssociativity());
    token.type = afl::TokenType::ArgumentDelimiter;
    ASSERT_FALSE(token.requireAssociativity());
    token.type = afl::TokenType::BracketOpen;
    ASSERT_FALSE(token.requireAssociativity());
    token.type = afl::TokenType::BracketClose;
    ASSERT_FALSE(token.requireAssociativity());
    token.type = afl::TokenType::Number;
    ASSERT_FALSE(token.requireAssociativity());
}

GTEST_TEST(Token_Test, requireParameterCount)
{
    afl::Token<std::string> token;
    token.type = afl::TokenType::Function;
    ASSERT_TRUE(token.requireParameterCount());

    token.type = afl::TokenType::Operator;
    ASSERT_FALSE(token.requireParameterCount());
    token.type = afl::TokenType::Constant;
    ASSERT_FALSE(token.requireParameterCount());
    token.type = afl::TokenType::ArgumentDelimiter;
    ASSERT_FALSE(token.requireParameterCount());
    token.type = afl::TokenType::BracketOpen;
    ASSERT_FALSE(token.requireParameterCount());
    token.type = afl::TokenType::BracketClose;
    ASSERT_FALSE(token.requireParameterCount());
    token.type = afl::TokenType::Number;
    ASSERT_FALSE(token.requireParameterCount());
}

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

GTEST_TEST(Token_Test, comparison)
{
    afl::Token<int> intToken1(42, afl::TokenType::Operator, 12, 3, afl::TokenAssociativity::Left);
    afl::Token<int> intToken2(42, afl::TokenType::Operator, 12, 3, afl::TokenAssociativity::Left);
    afl::Token<int> otherIntToken1(21, afl::TokenType::Operator, 12, 3, afl::TokenAssociativity::Left);
    afl::Token<int> otherIntToken2(42, afl::TokenType::Function, 12, 3, afl::TokenAssociativity::Left);
    afl::Token<int> otherIntToken3(42, afl::TokenType::Operator, 24, 3, afl::TokenAssociativity::Left);
    afl::Token<int> otherIntToken4(42, afl::TokenType::Operator, 12, 6, afl::TokenAssociativity::Left);
    afl::Token<int> otherIntToken5(42, afl::TokenType::Operator, 12, 3, afl::TokenAssociativity::Right);
    // operator==
    ASSERT_TRUE(intToken1 == intToken2);
    ASSERT_FALSE(intToken1 == otherIntToken1);
    ASSERT_FALSE(intToken1 == otherIntToken2);
    ASSERT_FALSE(intToken1 == otherIntToken3);
    ASSERT_FALSE(intToken1 == otherIntToken4);
    ASSERT_FALSE(intToken1 == otherIntToken5);
    // operator!=
    ASSERT_FALSE(intToken1 != intToken2);
    ASSERT_TRUE(intToken1 != otherIntToken1);
    ASSERT_TRUE(intToken1 != otherIntToken2);
    ASSERT_TRUE(intToken1 != otherIntToken3);
    ASSERT_TRUE(intToken1 != otherIntToken4);
    ASSERT_TRUE(intToken1 != otherIntToken5);

    afl::Token<std::string> stringToken1("Hello World!", afl::TokenType::Function, 923, 1, afl::TokenAssociativity::None);
    afl::Token<std::string> stringToken2("Hello World!", afl::TokenType::Function, 923, 1, afl::TokenAssociativity::None);
    afl::Token<std::string> otherStringToken1("Hello Other World!", afl::TokenType::Function, 923, 1, afl::TokenAssociativity::None);
    afl::Token<std::string> otherStringToken2("Hello World!", afl::TokenType::ArgumentDelimiter, 923, 1, afl::TokenAssociativity::None);
    afl::Token<std::string> otherStringToken3("Hello World!", afl::TokenType::Function, 1846, 1, afl::TokenAssociativity::None);
    afl::Token<std::string> otherStringToken4("Hello World!", afl::TokenType::Function, 923, 2, afl::TokenAssociativity::None);
    afl::Token<std::string> otherStringToken5("Hello World!", afl::TokenType::Function, 923, 1, afl::TokenAssociativity::Left);
    // operator==
    ASSERT_TRUE(stringToken1 == stringToken2);
    ASSERT_FALSE(stringToken1 == otherStringToken1);
    ASSERT_FALSE(stringToken1 == otherStringToken2);
    ASSERT_FALSE(stringToken1 == otherStringToken3);
    ASSERT_FALSE(stringToken1 == otherStringToken4);
    ASSERT_FALSE(stringToken1 == otherStringToken5);
    // operator!=
    ASSERT_FALSE(stringToken1 != stringToken2);
    ASSERT_TRUE(stringToken1 != otherStringToken1);
    ASSERT_TRUE(stringToken1 != otherStringToken2);
    ASSERT_TRUE(stringToken1 != otherStringToken3);
    ASSERT_TRUE(stringToken1 != otherStringToken4);
    ASSERT_TRUE(stringToken1 != otherStringToken5);
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

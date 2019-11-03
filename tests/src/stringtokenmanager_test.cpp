#include "gtest/gtest.h"

#include "../../src/private/stringtokenmanager.h"

typedef afl::detail::StringTokenManager STManager;

GTEST_TEST(StringTokenManager_Test, addToken)
{
    ASSERT_EQ(STManager::getTokens().size(), 0);
    auto token = new afl::Token<std::string>("+", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    STManager::addToken(std::unique_ptr<afl::Token<std::string>>(token));
    ASSERT_EQ(STManager::getTokens().size(), 1);
    token = new afl::Token<std::string>("+", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    STManager::addToken(std::unique_ptr<afl::Token<std::string>>(token));
    ASSERT_EQ(STManager::getTokens().size(), 1);

    token = new afl::Token<std::string>("-", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    STManager::addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("*", afl::TokenType::Operator, 2, 2, afl::TokenAssociativity::Left);
    STManager::addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("/", afl::TokenType::Operator, 2, 2, afl::TokenAssociativity::Left);
    STManager::addToken(std::unique_ptr<afl::Token<std::string>>(token));
    ASSERT_EQ(STManager::getTokens().size(), 4);
}

GTEST_TEST(StringTokenManager_Test, removeToken)
{
    auto token = new afl::Token<std::string>("+", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    STManager::addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("-", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    STManager::addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("*", afl::TokenType::Operator, 2, 2, afl::TokenAssociativity::Left);
    STManager::addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("/", afl::TokenType::Operator, 2, 2, afl::TokenAssociativity::Left);
    STManager::addToken(std::unique_ptr<afl::Token<std::string>>(token));
    ASSERT_EQ(STManager::getTokens().size(), 4);

    STManager::removeToken("+");
    ASSERT_EQ(STManager::getTokens().size(), 3);
    for(const auto& t : STManager::getTokens()) {
        ASSERT_NE(t.first, "+");
    }

    STManager::removeToken("-");
    ASSERT_EQ(STManager::getTokens().size(), 2);
    for(const auto& t : STManager::getTokens()) {
        ASSERT_NE(t.first, "-");
    }

    STManager::removeToken("/");
    ASSERT_EQ(STManager::getTokens().size(), 1);
    for(const auto& t : STManager::getTokens()) {
        ASSERT_NE(t.first, "/");
    }

    STManager::removeToken("*");
    ASSERT_EQ(STManager::getTokens().size(), 0);
}

GTEST_TEST(StringTokenManager_Test, getToken)
{
    auto token = new afl::Token<std::string>("+", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    STManager::addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("-", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    STManager::addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("*", afl::TokenType::Operator, 2, 2, afl::TokenAssociativity::Left);
    STManager::addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("/", afl::TokenType::Operator, 2, 2, afl::TokenAssociativity::Left);
    STManager::addToken(std::unique_ptr<afl::Token<std::string>>(token));
    ASSERT_EQ(STManager::getTokens().size(), 4);

    // test existing and createIfNotExist = false
    std::shared_ptr<afl::Token<std::string>> sharedToken = STManager::getToken("+", false);
    ASSERT_EQ(sharedToken->value, "+");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 1);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

   sharedToken = STManager::getToken("-", false);
    ASSERT_EQ(sharedToken->value, "-");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 1);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    sharedToken = STManager::getToken("*", false);
    ASSERT_EQ(sharedToken->value, "*");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 2);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    sharedToken = STManager::getToken("/", false);
    ASSERT_EQ(sharedToken->value, "/");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 2);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    ASSERT_EQ(STManager::getTokens().size(), 4);

    // test existing and createIfNotExist = true
    sharedToken = STManager::getToken("+", true);
    ASSERT_EQ(sharedToken->value, "+");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 1);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    sharedToken = STManager::getToken("-", true);
    ASSERT_EQ(sharedToken->value, "-");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 1);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    sharedToken = STManager::getToken("*", true);
    ASSERT_EQ(sharedToken->value, "*");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 2);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    sharedToken = STManager::getToken("/", true);
    ASSERT_EQ(sharedToken->value, "/");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 2);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    ASSERT_EQ(STManager::getTokens().size(), 4);

    // test non existing and createIfNotExist = false
    ASSERT_EQ(STManager::getToken("^", false).get(), nullptr);
    ASSERT_EQ(STManager::getToken("7", false).get(), nullptr);

    ASSERT_EQ(STManager::getTokens().size(), 4);

    // test non existing and createIfNotExist = true
    ASSERT_EQ(STManager::getToken("This token should NEVER exist!", true).get(), nullptr);
    ASSERT_EQ(STManager::getTokens().size(), 4);

    // create number token
    sharedToken = STManager::getToken("7234.232", true);
    ASSERT_EQ(sharedToken->value, "7234.232");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Number);
    ASSERT_EQ(sharedToken->precedence, 0);
    ASSERT_EQ(sharedToken->parameterCount, 0);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::None);
    ASSERT_EQ(STManager::getTokens().size(), 5);

    // create constant token
    sharedToken = STManager::getToken("______a__1_", true);
    ASSERT_EQ(sharedToken->value, "______a__1_");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Constant);
    ASSERT_EQ(sharedToken->precedence, 0);
    ASSERT_EQ(sharedToken->parameterCount, 0);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::None);
    ASSERT_EQ(STManager::getTokens().size(), 6);
}

#include "gtest/gtest.h"

#include "../../src/private/stringtokenmanager.h"

GTEST_TEST(StringTokenManager_Test, addToken)
{
    afl::detail::StringTokenManager manager;
    ASSERT_EQ(manager.getTokens().size(), 0);
    auto token = new afl::Token<std::string>("+", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    manager.addToken(std::unique_ptr<afl::Token<std::string>>(token));
    ASSERT_EQ(manager.getTokens().size(), 1);
    token = new afl::Token<std::string>("+", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    manager.addToken(std::unique_ptr<afl::Token<std::string>>(token));
    ASSERT_EQ(manager.getTokens().size(), 1);

    token = new afl::Token<std::string>("-", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    manager.addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("*", afl::TokenType::Operator, 2, 2, afl::TokenAssociativity::Left);
    manager.addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("/", afl::TokenType::Operator, 2, 2, afl::TokenAssociativity::Left);
    manager.addToken(std::unique_ptr<afl::Token<std::string>>(token));
    ASSERT_EQ(manager.getTokens().size(), 4);
}

GTEST_TEST(StringTokenManager_Test, removeToken)
{
    afl::detail::StringTokenManager manager;
    auto token = new afl::Token<std::string>("+", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    manager.addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("-", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    manager.addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("*", afl::TokenType::Operator, 2, 2, afl::TokenAssociativity::Left);
    manager.addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("/", afl::TokenType::Operator, 2, 2, afl::TokenAssociativity::Left);
    manager.addToken(std::unique_ptr<afl::Token<std::string>>(token));
    ASSERT_EQ(manager.getTokens().size(), 4);

    manager.removeToken("+");
    ASSERT_EQ(manager.getTokens().size(), 3);
    for(const auto& t : manager.getTokens()) {
        ASSERT_NE(t.first, "+");
    }

    manager.removeToken("-");
    ASSERT_EQ(manager.getTokens().size(), 2);
    for(const auto& t : manager.getTokens()) {
        ASSERT_NE(t.first, "-");
    }

    manager.removeToken("/");
    ASSERT_EQ(manager.getTokens().size(), 1);
    for(const auto& t : manager.getTokens()) {
        ASSERT_NE(t.first, "/");
    }

    manager.removeToken("*");
    ASSERT_EQ(manager.getTokens().size(), 0);
}

GTEST_TEST(StringTokenManager_Test, getToken)
{
    afl::detail::StringTokenManager manager;
    auto token = new afl::Token<std::string>("+", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    manager.addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("-", afl::TokenType::Operator, 1, 2, afl::TokenAssociativity::Left);
    manager.addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("*", afl::TokenType::Operator, 2, 2, afl::TokenAssociativity::Left);
    manager.addToken(std::unique_ptr<afl::Token<std::string>>(token));
    token = new afl::Token<std::string>("/", afl::TokenType::Operator, 2, 2, afl::TokenAssociativity::Left);
    manager.addToken(std::unique_ptr<afl::Token<std::string>>(token));
    ASSERT_EQ(manager.getTokens().size(), 4);

    // test existing and createIfNotExist = false
    std::shared_ptr<afl::Token<std::string>> sharedToken = manager.getToken("+", false);
    ASSERT_EQ(sharedToken->value, "+");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 1);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

   sharedToken = manager.getToken("-", false);
    ASSERT_EQ(sharedToken->value, "-");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 1);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    sharedToken = manager.getToken("*", false);
    ASSERT_EQ(sharedToken->value, "*");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 2);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    sharedToken = manager.getToken("/", false);
    ASSERT_EQ(sharedToken->value, "/");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 2);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    ASSERT_EQ(manager.getTokens().size(), 4);

    // test existing and createIfNotExist = true
    sharedToken = manager.getToken("+", true);
    ASSERT_EQ(sharedToken->value, "+");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 1);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    sharedToken = manager.getToken("-", true);
    ASSERT_EQ(sharedToken->value, "-");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 1);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    sharedToken = manager.getToken("*", true);
    ASSERT_EQ(sharedToken->value, "*");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 2);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    sharedToken = manager.getToken("/", true);
    ASSERT_EQ(sharedToken->value, "/");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Operator);
    ASSERT_EQ(sharedToken->precedence, 2);
    ASSERT_EQ(sharedToken->parameterCount, 2);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::Left);

    ASSERT_EQ(manager.getTokens().size(), 4);

    // test non existing and createIfNotExist = false
    ASSERT_EQ(manager.getToken("^", false).get(), nullptr);
    ASSERT_EQ(manager.getToken("7", false).get(), nullptr);

    ASSERT_EQ(manager.getTokens().size(), 4);

    // test non existing and createIfNotExist = true
    ASSERT_EQ(manager.getToken("This token should NEVER exist!", true).get(), nullptr);
    ASSERT_EQ(manager.getTokens().size(), 4);

    // create number token
    sharedToken = manager.getToken("7234.232", true);
    ASSERT_EQ(sharedToken->value, "7234.232");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Number);
    ASSERT_EQ(sharedToken->precedence, 0);
    ASSERT_EQ(sharedToken->parameterCount, 0);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::None);
    ASSERT_EQ(manager.getTokens().size(), 5);

    // create constant token
    sharedToken = manager.getToken("______a__1_", true);
    ASSERT_EQ(sharedToken->value, "______a__1_");
    ASSERT_EQ(sharedToken->type, afl::TokenType::Constant);
    ASSERT_EQ(sharedToken->precedence, 0);
    ASSERT_EQ(sharedToken->parameterCount, 0);
    ASSERT_EQ(sharedToken->associativity, afl::TokenAssociativity::None);
    ASSERT_EQ(manager.getTokens().size(), 6);
}

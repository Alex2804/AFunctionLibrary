#include "gtest/gtest.h"

#define A_PLUGIN_SDK_EXCLUDE_DEFINITIONS
#define A_PLUGIN_SDK_DEBUG
#include "../../AFunctionLibrary/implementation/cstringtoken.hpp"

GTEST_TEST(CStringToken_Test, equal_operator)
{
    auto cStringToken1 = static_cast<afl::CStringToken*>(std::malloc(sizeof(afl::CStringToken)));
    cStringToken1->string = afl::convert("Test value!");
    cStringToken1->type = afl::TokenType::Constant;
    cStringToken1->precedence = 42;
    cStringToken1->parameterCount = 13;
    cStringToken1->associativity = afl::TokenAssociativity::None;

    auto cStringToken2 = static_cast<afl::CStringToken*>(std::malloc(sizeof(afl::CStringToken)));
    cStringToken2->string = afl::convert("Test value!");
    cStringToken2->type = afl::TokenType::Constant;
    cStringToken2->precedence = 42;
    cStringToken2->parameterCount = 13;
    cStringToken2->associativity = afl::TokenAssociativity::None;

    ASSERT_TRUE(*cStringToken1 == *cStringToken1);
    ASSERT_TRUE(*cStringToken1 == *cStringToken2);
    afl::free(cStringToken2->string);
    cStringToken2->string = afl::convert("An other test value!");
    ASSERT_FALSE(*cStringToken1 == *cStringToken2);
    afl::free(cStringToken2->string);
    cStringToken2->string = afl::convert("Test value!");
    ASSERT_TRUE(*cStringToken1 == *cStringToken2);
    cStringToken2->type = afl::TokenType::Number;
    ASSERT_FALSE(*cStringToken1 == *cStringToken2);
    cStringToken2->type = afl::TokenType::Constant;
    ASSERT_TRUE(*cStringToken1 == *cStringToken2);
    cStringToken2->precedence = 0;
    ASSERT_FALSE(*cStringToken1 == *cStringToken2);
    cStringToken2->precedence = 42;
    ASSERT_TRUE(*cStringToken1 == *cStringToken2);
    cStringToken2->parameterCount = 0;
    ASSERT_FALSE(*cStringToken1 == *cStringToken2);
    cStringToken2->parameterCount = 13;
    ASSERT_TRUE(*cStringToken1 == *cStringToken2);
    cStringToken2->associativity = afl::TokenAssociativity::Left;
    ASSERT_FALSE(*cStringToken1 == *cStringToken2);
    cStringToken2->associativity = afl::TokenAssociativity::None;
    ASSERT_TRUE(*cStringToken1 == *cStringToken2);

    ASSERT_FALSE(*cStringToken1 != *cStringToken1);
    ASSERT_FALSE(*cStringToken1 != *cStringToken2);
    afl::free(cStringToken2->string);
    cStringToken2->string = afl::convert("An other test value!");
    ASSERT_TRUE(*cStringToken1 != *cStringToken2);
    afl::free(cStringToken2->string);
    cStringToken2->string = afl::convert("Test value!");
    ASSERT_FALSE(*cStringToken1 != *cStringToken2);
    cStringToken2->type = afl::TokenType::Number;
    ASSERT_TRUE(*cStringToken1 != *cStringToken2);
    cStringToken2->type = afl::TokenType::Constant;
    ASSERT_FALSE(*cStringToken1 != *cStringToken2);
    cStringToken2->precedence = 0;
    ASSERT_TRUE(*cStringToken1 != *cStringToken2);
    cStringToken2->precedence = 42;
    ASSERT_FALSE(*cStringToken1 != *cStringToken2);
    cStringToken2->parameterCount = 0;
    ASSERT_TRUE(*cStringToken1 != *cStringToken2);
    cStringToken2->parameterCount = 13;
    ASSERT_FALSE(*cStringToken1 != *cStringToken2);
    cStringToken2->associativity = afl::TokenAssociativity::Left;
    ASSERT_TRUE(*cStringToken1 != *cStringToken2);
    cStringToken2->associativity = afl::TokenAssociativity::None;
    ASSERT_FALSE(*cStringToken1 != *cStringToken2);

    ASSERT_TRUE(afl::equal(cStringToken1, cStringToken1));
    ASSERT_TRUE(afl::equal(cStringToken1, cStringToken2));
    afl::free(cStringToken2->string);
    cStringToken2->string = afl::convert("An other test value!");
    ASSERT_FALSE(afl::equal(cStringToken1, cStringToken2));
    afl::free(cStringToken2->string);
    cStringToken2->string = afl::convert("Test value!");
    ASSERT_TRUE(afl::equal(cStringToken1, cStringToken2));
    cStringToken2->type = afl::TokenType::Number;
    ASSERT_FALSE(afl::equal(cStringToken1, cStringToken2));
    cStringToken2->type = afl::TokenType::Constant;
    ASSERT_TRUE(afl::equal(cStringToken1, cStringToken2));
    cStringToken2->precedence = 0;
    ASSERT_FALSE(afl::equal(cStringToken1, cStringToken2));
    cStringToken2->precedence = 42;
    ASSERT_TRUE(afl::equal(cStringToken1, cStringToken2));
    cStringToken2->parameterCount = 0;
    ASSERT_FALSE(afl::equal(cStringToken1, cStringToken2));
    cStringToken2->parameterCount = 13;
    ASSERT_TRUE(afl::equal(cStringToken1, cStringToken2));
    cStringToken2->associativity = afl::TokenAssociativity::Left;
    ASSERT_FALSE(afl::equal(cStringToken1, cStringToken2));
    cStringToken2->associativity = afl::TokenAssociativity::None;
    ASSERT_TRUE(afl::equal(cStringToken1, cStringToken2));
    ASSERT_FALSE(afl::equal(cStringToken1, nullptr));
    ASSERT_FALSE(afl::equal(nullptr, cStringToken1));
    ASSERT_TRUE(afl::equal(static_cast<afl::CStringToken*>(nullptr), nullptr));

    afl::free(cStringToken1->string);
    std::free(cStringToken1);
    afl::free(cStringToken2->string);
    std::free(cStringToken2);
}

GTEST_TEST(CStringToken_Test, free)
{
    apl::debug::allocationCount = 0;
    apl::debug::freeCount = 0;

    afl::free(static_cast<afl::CStringToken*>(nullptr));
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    afl::CStringToken tmpCToken = {apl::freeMemory, afl::convert("Test value!"), afl::TokenType::Constant, 0, 0, afl::TokenAssociativity::None};
    auto cToken = static_cast<afl::CStringToken*>(apl::allocateMemory(sizeof(afl::CStringToken)));
    memcpy(cToken, &tmpCToken, sizeof(afl::CStringToken));
    afl::free(cToken);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    tmpCToken.string = nullptr;
    cToken = static_cast<afl::CStringToken*>(apl::allocateMemory(sizeof(afl::CStringToken)));
    memcpy(cToken, &tmpCToken, sizeof(afl::CStringToken));
    afl::free(cToken);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);
}

namespace afl
{
    namespace test
    {
        size_t retrievedTokenCount = 0;
        std::shared_ptr<afl::Token<std::string>> getTokenTestFunction(const std::string& tokenValue)
        {
            if(tokenValue == "Retrieve value!") {
                retrievedTokenCount += 1;
                return std::make_shared<afl::Token<std::string>>("Retrieved test value!", afl::TokenType::ArgumentDelimiter, 128, 512, afl::TokenAssociativity::Right);
            } else {
                return std::shared_ptr<afl::Token<std::string>>();
            }
        }
    }
}

GTEST_TEST(CStringToken_Test, convert)
{
    apl::debug::allocationCount = 0;
    apl::debug::freeCount = 0;
    afl::test::retrievedTokenCount = 0;

    auto token = afl::Token<std::string>("Test value!", afl::TokenType::Constant, 42, 13, afl::TokenAssociativity::None);

    ASSERT_EQ(afl::convert(static_cast<afl::CStringToken*>(nullptr)), nullptr);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    afl::CStringToken* cStringToken = afl::convert(token);
    ASSERT_STREQ(cStringToken->string->string, "Test value!");
    ASSERT_EQ(cStringToken->type, afl::TokenType::Constant);
    ASSERT_EQ(cStringToken->precedence, 42);
    ASSERT_EQ(cStringToken->parameterCount, 13);
    ASSERT_EQ(cStringToken->associativity, afl::TokenAssociativity::None);
    std::shared_ptr<afl::Token<std::string>> tmpToken = afl::convert(cStringToken);
    ASSERT_EQ(*tmpToken, token);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    cStringToken = afl::convert(token);
    afl::free(cStringToken->string);
    cStringToken->string = nullptr;
    tmpToken = afl::convert(cStringToken);
    token.value = "";
    ASSERT_EQ(*tmpToken, token);
    token.value = "Test value!";
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    cStringToken = afl::convert(token);
    tmpToken = afl::convert(cStringToken, afl::test::getTokenTestFunction);
    ASSERT_EQ(*tmpToken, token);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);
    ASSERT_EQ(afl::test::retrievedTokenCount, 0);

    token.value = "Retrieve value!";
    cStringToken = afl::convert(token);
    tmpToken = afl::convert(cStringToken, afl::test::getTokenTestFunction);
    ASSERT_EQ(tmpToken->value, "Retrieved test value!");
    ASSERT_EQ(tmpToken->type, afl::TokenType::ArgumentDelimiter);
    ASSERT_EQ(tmpToken->precedence, 128);
    ASSERT_EQ(tmpToken->parameterCount, 512);
    ASSERT_EQ(tmpToken->associativity, afl::TokenAssociativity::Right);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);
    ASSERT_EQ(afl::test::retrievedTokenCount, 1);
}

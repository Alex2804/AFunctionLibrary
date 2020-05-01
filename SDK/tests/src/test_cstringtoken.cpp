#include "gtest/gtest.h"

#include "../../AFunctionLibrary/implementation/cstringtoken.hpp"

/**************************************************** CStringToken ****************************************************/

GTEST_TEST(Test_CStringToken, equal_operator)
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

GTEST_TEST(Test_CStringToken, convert)
{
    afl::test::retrievedTokenCount = 0;

    auto token = afl::Token<std::string>("Test value!", afl::TokenType::Constant, 42, 13, afl::TokenAssociativity::None);

    ASSERT_EQ(afl::convert(static_cast<afl::CStringToken*>(nullptr)), nullptr);

    afl::CStringToken* cStringToken = afl::convert(token);
    ASSERT_STREQ(cStringToken->string->string, "Test value!");
    ASSERT_EQ(cStringToken->type, afl::TokenType::Constant);
    ASSERT_EQ(cStringToken->precedence, 42);
    ASSERT_EQ(cStringToken->parameterCount, 13);
    ASSERT_EQ(cStringToken->associativity, afl::TokenAssociativity::None);
    std::shared_ptr<afl::Token<std::string>> tmpToken = afl::convert(cStringToken);
    ASSERT_EQ(*tmpToken, token);

    cStringToken = afl::convert(token);
    afl::free(cStringToken->string);
    cStringToken->string = nullptr;
    tmpToken = afl::convert(cStringToken);
    token.value = "";
    ASSERT_EQ(*tmpToken, token);
    token.value = "Test value!";

    cStringToken = afl::convert(token);
    tmpToken = afl::convert(cStringToken, afl::test::getTokenTestFunction);
    ASSERT_EQ(*tmpToken, token);
    ASSERT_EQ(afl::test::retrievedTokenCount, 0);

    token.value = "Retrieve value!";
    cStringToken = afl::convert(token);
    tmpToken = afl::convert(cStringToken, afl::test::getTokenTestFunction);
    ASSERT_EQ(tmpToken->value, "Retrieved test value!");
    ASSERT_EQ(tmpToken->type, afl::TokenType::ArgumentDelimiter);
    ASSERT_EQ(tmpToken->precedence, 128);
    ASSERT_EQ(tmpToken->parameterCount, 512);
    ASSERT_EQ(tmpToken->associativity, afl::TokenAssociativity::Right);
    ASSERT_EQ(afl::test::retrievedTokenCount, 1);
}


/************************************************ CStringTokenAliases *************************************************/

GTEST_TEST(Test_CStringTokenAliases, equal_operator)
{
    auto cStringTokenAliases1 = static_cast<afl::CStringTokenAliases*>(std::malloc(sizeof(afl::CStringTokenAliases)));
    cStringTokenAliases1->type = afl::TokenAliasType::String;
    cStringTokenAliases1->aliasesCount = 2;
    cStringTokenAliases1->aliases = static_cast<afl::CString**>(std::malloc(sizeof(void*) * cStringTokenAliases1->aliasesCount));
    cStringTokenAliases1->aliases[0] = afl::convert("Test string 1!");
    cStringTokenAliases1->aliases[1] = afl::convert("Test string 2!");

    auto cStringTokenAliases2 = static_cast<afl::CStringTokenAliases*>(std::malloc(sizeof(afl::CStringTokenAliases)));
    cStringTokenAliases2->type = afl::TokenAliasType::String;
    cStringTokenAliases2->aliasesCount = 2;
    cStringTokenAliases2->aliases = static_cast<afl::CString**>(std::malloc(sizeof(void*) * cStringTokenAliases2->aliasesCount));
    cStringTokenAliases2->aliases[0] = afl::convert("Test string 1!");
    cStringTokenAliases2->aliases[1] = afl::convert("Test string 2!");

    ASSERT_TRUE(*cStringTokenAliases1 == *cStringTokenAliases1);
    ASSERT_TRUE(*cStringTokenAliases1 == *cStringTokenAliases2);
    afl::free(cStringTokenAliases2->aliases[1]);
    cStringTokenAliases2->aliases[1] = afl::convert("Test string 1!");
    ASSERT_FALSE(*cStringTokenAliases1 == *cStringTokenAliases2);
    afl::free(cStringTokenAliases2->aliases[1]);
    cStringTokenAliases2->aliases[1] = afl::convert("Test string 2!");
    ASSERT_TRUE(*cStringTokenAliases1 == *cStringTokenAliases2);
    cStringTokenAliases2->type = afl::TokenAliasType::Regex;
    ASSERT_FALSE(*cStringTokenAliases1 == *cStringTokenAliases2);
    cStringTokenAliases2->type = afl::TokenAliasType::String;
    ASSERT_TRUE(*cStringTokenAliases1 == *cStringTokenAliases2);
    cStringTokenAliases2->aliasesCount = 0;
    ASSERT_FALSE(*cStringTokenAliases1 == *cStringTokenAliases2);
    cStringTokenAliases2->aliasesCount = 2;
    ASSERT_TRUE(*cStringTokenAliases1 == *cStringTokenAliases2);

    ASSERT_FALSE(*cStringTokenAliases1 != *cStringTokenAliases1);
    ASSERT_FALSE(*cStringTokenAliases1 != *cStringTokenAliases2);
    afl::free(cStringTokenAliases2->aliases[1]);
    cStringTokenAliases2->aliases[1] = afl::convert("Test string 1!");
    ASSERT_TRUE(*cStringTokenAliases1 != *cStringTokenAliases2);
    afl::free(cStringTokenAliases2->aliases[1]);
    cStringTokenAliases2->aliases[1] = afl::convert("Test string 2!");
    ASSERT_FALSE(*cStringTokenAliases1 != *cStringTokenAliases2);
    cStringTokenAliases2->type = afl::TokenAliasType::Regex;
    ASSERT_TRUE(*cStringTokenAliases1 != *cStringTokenAliases2);
    cStringTokenAliases2->type = afl::TokenAliasType::String;
    ASSERT_FALSE(*cStringTokenAliases1 != *cStringTokenAliases2);
    cStringTokenAliases2->aliasesCount = 0;
    ASSERT_TRUE(*cStringTokenAliases1 != *cStringTokenAliases2);
    cStringTokenAliases2->aliasesCount = 2;
    ASSERT_FALSE(*cStringTokenAliases1 != *cStringTokenAliases2);

    ASSERT_TRUE(afl::equal(cStringTokenAliases1, cStringTokenAliases1));
    ASSERT_TRUE(afl::equal(cStringTokenAliases1, cStringTokenAliases2));
    afl::free(cStringTokenAliases2->aliases[1]);
    cStringTokenAliases2->aliases[1] = afl::convert("Test string 1!");
    ASSERT_FALSE(afl::equal(cStringTokenAliases1, cStringTokenAliases2));
    afl::free(cStringTokenAliases2->aliases[1]);
    cStringTokenAliases2->aliases[1] = afl::convert("Test string 2!");
    ASSERT_TRUE(afl::equal(cStringTokenAliases1, cStringTokenAliases2));
    cStringTokenAliases2->type = afl::TokenAliasType::Regex;
    ASSERT_FALSE(afl::equal(cStringTokenAliases1, cStringTokenAliases2));
    cStringTokenAliases2->type = afl::TokenAliasType::String;
    ASSERT_TRUE(afl::equal(cStringTokenAliases1, cStringTokenAliases2));
    cStringTokenAliases2->aliasesCount = 0;
    ASSERT_FALSE(afl::equal(cStringTokenAliases1, cStringTokenAliases2));
    cStringTokenAliases2->aliasesCount = 2;
    ASSERT_TRUE(afl::equal(cStringTokenAliases1, cStringTokenAliases2));
    ASSERT_FALSE(afl::equal(cStringTokenAliases1, nullptr));
    ASSERT_FALSE(afl::equal(nullptr, cStringTokenAliases1));
    ASSERT_TRUE(afl::equal(static_cast<afl::CStringTokenAliases*>(nullptr), nullptr));

    afl::free(cStringTokenAliases1->aliases[0]);
    afl::free(cStringTokenAliases1->aliases[1]);
    std::free(cStringTokenAliases1->aliases);
    std::free(cStringTokenAliases1);
    afl::free(cStringTokenAliases2->aliases[0]);
    afl::free(cStringTokenAliases2->aliases[1]);
    std::free(cStringTokenAliases2->aliases);
    std::free(cStringTokenAliases2);
}

GTEST_TEST(Test_CStringTokenAliases, convert)
{
    auto aliases = afl::TokenAliases<std::string>(afl::TokenAliasType::String, {"alias1", "alias2"});

    ASSERT_EQ(afl::convert(static_cast<afl::CStringTokenAliases*>(nullptr)), afl::TokenAliases<std::string>(afl::TokenAliasType::String));

    afl::CStringTokenAliases* cAliases = afl::convert(aliases);
    ASSERT_EQ(cAliases->type, aliases.type);
    ASSERT_EQ(cAliases->aliasesCount, 2);
    ASSERT_STREQ(cAliases->aliases[0]->string, "alias1");
    ASSERT_STREQ(cAliases->aliases[1]->string, "alias2");
    afl::TokenAliases<std::string> tmpAliases = afl::convert(cAliases);
    ASSERT_EQ(tmpAliases, aliases);

    cAliases = afl::convert(aliases);
    for(size_t i = 0; i < cAliases->aliasesCount; ++i)
        afl::free(cAliases->aliases[i]);
    apl::APluginSDK_free(cAliases->aliases);
    cAliases->aliases = nullptr;
    tmpAliases = afl::convert(cAliases);
    aliases.aliases.clear();
    ASSERT_EQ(tmpAliases, aliases);
}


/************************************************* CStringTokenGroup **************************************************/

GTEST_TEST(Test_CStringTokenGroup, equal_operator)
{
    auto cStringTokenGroup1 = static_cast<afl::CStringTokenGroup*>(std::malloc(sizeof(afl::CStringTokenGroup)));
    cStringTokenGroup1->token = afl::convert(afl::Token<std::string>("Test value!", afl::TokenType::Constant, 42, 13, afl::TokenAssociativity::None));
    cStringTokenGroup1->groupIDSize = 2;
    cStringTokenGroup1->groupID = static_cast<size_t*>(std::malloc(sizeof(void*) * cStringTokenGroup1->groupIDSize));
    cStringTokenGroup1->groupID[0] = 42;
    cStringTokenGroup1->groupID[1] = 7;

    auto cStringTokenGroup2 = static_cast<afl::CStringTokenGroup*>(std::malloc(sizeof(afl::CStringTokenGroup)));
    cStringTokenGroup2->token = afl::convert(afl::Token<std::string>("Test value!", afl::TokenType::Constant, 42, 13, afl::TokenAssociativity::None));
    cStringTokenGroup2->groupIDSize = 2;
    cStringTokenGroup2->groupID = static_cast<size_t*>(std::malloc(sizeof(void*) * cStringTokenGroup2->groupIDSize));
    cStringTokenGroup2->groupID[0] = 42;
    cStringTokenGroup2->groupID[1] = 7;

    ASSERT_TRUE(*cStringTokenGroup1 == *cStringTokenGroup1);
    ASSERT_TRUE(*cStringTokenGroup1 == *cStringTokenGroup2);
    cStringTokenGroup2->groupID[1] = 13;
    ASSERT_FALSE(*cStringTokenGroup1 == *cStringTokenGroup2);
    cStringTokenGroup2->groupID[1] = 7;
    ASSERT_TRUE(*cStringTokenGroup1 == *cStringTokenGroup2);
    cStringTokenGroup2->groupIDSize = 5;
    ASSERT_FALSE(*cStringTokenGroup1 == *cStringTokenGroup2);
    cStringTokenGroup2->groupIDSize = 2;
    ASSERT_TRUE(*cStringTokenGroup1 == *cStringTokenGroup1);
    afl::CStringToken* tmpToken = cStringTokenGroup2->token;
    cStringTokenGroup2->token = nullptr;
    ASSERT_FALSE(*cStringTokenGroup1 == *cStringTokenGroup2);
    cStringTokenGroup2->token = tmpToken;
    ASSERT_TRUE(*cStringTokenGroup1 == *cStringTokenGroup2);

    ASSERT_FALSE(*cStringTokenGroup1 != *cStringTokenGroup1);
    ASSERT_FALSE(*cStringTokenGroup1 != *cStringTokenGroup2);
    cStringTokenGroup2->groupID[1] = 13;
    ASSERT_TRUE(*cStringTokenGroup1 != *cStringTokenGroup2);
    cStringTokenGroup2->groupID[1] = 7;
    ASSERT_FALSE(*cStringTokenGroup1 != *cStringTokenGroup2);
    cStringTokenGroup2->groupIDSize = 5;
    ASSERT_TRUE(*cStringTokenGroup1 != *cStringTokenGroup2);
    cStringTokenGroup2->groupIDSize = 2;
    ASSERT_FALSE(*cStringTokenGroup1 != *cStringTokenGroup1);
    tmpToken = cStringTokenGroup2->token;
    cStringTokenGroup2->token = nullptr;
    ASSERT_TRUE(*cStringTokenGroup1 != *cStringTokenGroup2);
    cStringTokenGroup2->token = tmpToken;
    ASSERT_FALSE(*cStringTokenGroup1 != *cStringTokenGroup2);

    ASSERT_TRUE(equal(cStringTokenGroup1, cStringTokenGroup1));
    ASSERT_TRUE(equal(cStringTokenGroup1, cStringTokenGroup2));
    cStringTokenGroup2->groupID[1] = 13;
    ASSERT_FALSE(equal(cStringTokenGroup1, cStringTokenGroup2));
    cStringTokenGroup2->groupID[1] = 7;
    ASSERT_TRUE(equal(cStringTokenGroup1, cStringTokenGroup2));
    cStringTokenGroup2->groupIDSize = 5;
    ASSERT_FALSE(equal(cStringTokenGroup1, cStringTokenGroup2));
    cStringTokenGroup2->groupIDSize = 2;
    ASSERT_TRUE(equal(cStringTokenGroup1, cStringTokenGroup1));
    tmpToken = cStringTokenGroup2->token;
    cStringTokenGroup2->token = nullptr;
    ASSERT_FALSE(equal(cStringTokenGroup1, cStringTokenGroup2));
    cStringTokenGroup2->token = tmpToken;
    ASSERT_TRUE(equal(cStringTokenGroup1, cStringTokenGroup2));
    ASSERT_FALSE(afl::equal(cStringTokenGroup1, nullptr));
    ASSERT_FALSE(afl::equal(nullptr, cStringTokenGroup1));
    ASSERT_TRUE(afl::equal(static_cast<afl::CStringTokenGroup*>(nullptr), nullptr));

    afl::free(cStringTokenGroup1->token);
    std::free(cStringTokenGroup1->groupID);
    std::free(cStringTokenGroup1);
    afl::free(cStringTokenGroup2->token);
    std::free(cStringTokenGroup2->groupID);
    std::free(cStringTokenGroup2);
}

GTEST_TEST(Test_CStringTokenGroup, convert)
{
    afl::Token<std::string> token("Test token!", afl::TokenType::Constant, 42, 13, afl::TokenAssociativity::None);
    afl::TokenGroup<std::string> tokenGroup(std::make_shared<afl::Token<std::string>>(token), {128, 256, 512});

    ASSERT_EQ(afl::convert(static_cast<afl::CStringTokenGroup*>(nullptr)), afl::TokenGroup<std::string>());

    afl::CStringTokenGroup* cTokenGroup = afl::convert(tokenGroup);
    afl::CStringToken* cToken = afl::convert(token);
    ASSERT_EQ(*cTokenGroup->token, *cToken);
    afl::free(cToken);
    ASSERT_EQ(cTokenGroup->groupIDSize, 3);
    ASSERT_EQ(cTokenGroup->groupID[0], 128);
    ASSERT_EQ(cTokenGroup->groupID[1], 256);
    ASSERT_EQ(cTokenGroup->groupID[2], 512);
    afl::TokenGroup<std::string> tmpTokenGroup = afl::convert(cTokenGroup);
    ASSERT_EQ(tmpTokenGroup, tokenGroup);

    cTokenGroup = afl::convert(tokenGroup);
    afl::free(cTokenGroup->token);
    cTokenGroup->token = nullptr;
    tmpTokenGroup = afl::convert(cTokenGroup);
    tokenGroup.token = nullptr;
    ASSERT_EQ(tmpTokenGroup, tokenGroup);
    tokenGroup.token = std::make_shared<afl::Token<std::string>>(token);

    cTokenGroup = afl::convert(tokenGroup);
    apl::APluginSDK_free(cTokenGroup->groupID);
    cTokenGroup->groupID = nullptr;
    tmpTokenGroup = afl::convert(cTokenGroup);
    tokenGroup.groupID.clear();
    ASSERT_EQ(tmpTokenGroup, tokenGroup);
}

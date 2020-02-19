#include "gtest/gtest.h"

#define A_PLUGIN_SDK_EXCLUDE_DEFINITIONS
#define A_PLUGIN_SDK_DEBUG
#include "../../AFunctionLibrary/implementation/cstringtoken.hpp"

GTEST_TEST(CStringTokenGroup_Test, equal_operator)
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

GTEST_TEST(CStringTokenGroup_Test, free)
{
    apl::debug::allocationCount = 0;
    apl::debug::freeCount = 0;

    afl::free(static_cast<afl::CStringTokenGroup*>(nullptr));
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    afl::CStringTokenGroup tmpCTokenGroup = {apl::freeMemory};
    auto cTokenGroup = static_cast<afl::CStringTokenGroup*>(apl::allocateMemory(sizeof(afl::CStringTokenGroup)));
    memcpy(cTokenGroup, &tmpCTokenGroup, sizeof(afl::CStringTokenAliases));
    cTokenGroup->token = afl::convert(afl::Token<std::string>("Test value!", afl::TokenType::Constant, 42, 13, afl::TokenAssociativity::None));
    cTokenGroup->groupIDSize = 2;
    cTokenGroup->groupID = static_cast<size_t*>(apl::allocateMemory(sizeof(void*) * cTokenGroup->groupIDSize));
    cTokenGroup->groupID[0] = 42;
    cTokenGroup->groupID[1] = 7;
    afl::free(cTokenGroup);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    cTokenGroup = static_cast<afl::CStringTokenGroup*>(apl::allocateMemory(sizeof(afl::CStringTokenGroup)));
    memcpy(cTokenGroup, &tmpCTokenGroup, sizeof(afl::CStringTokenAliases));
    cTokenGroup->token = afl::convert(afl::Token<std::string>("Test value!", afl::TokenType::Constant, 42, 13, afl::TokenAssociativity::None));
    cTokenGroup->groupIDSize = 2;
    cTokenGroup->groupID = nullptr;
    afl::free(cTokenGroup);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    cTokenGroup = static_cast<afl::CStringTokenGroup*>(apl::allocateMemory(sizeof(afl::CStringTokenGroup)));
    memcpy(cTokenGroup, &tmpCTokenGroup, sizeof(afl::CStringTokenAliases));
    cTokenGroup->token = nullptr;
    cTokenGroup->groupIDSize = 2;
    cTokenGroup->groupID = static_cast<size_t*>(apl::allocateMemory(sizeof(void*) * cTokenGroup->groupIDSize));
    cTokenGroup->groupID[0] = 42;
    cTokenGroup->groupID[1] = 7;
    afl::free(cTokenGroup);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);
}

GTEST_TEST(CStringTokenGroup_Test, convert)
{
    apl::debug::allocationCount = 0;
    apl::debug::freeCount = 0;

    afl::Token<std::string> token("Test token!", afl::TokenType::Constant, 42, 13, afl::TokenAssociativity::None);
    afl::TokenGroup<std::string> tokenGroup(std::make_shared<afl::Token<std::string>>(token), {128, 256, 512});

    ASSERT_EQ(afl::convert(static_cast<afl::CStringTokenGroup*>(nullptr)), afl::TokenGroup<std::string>());
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

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
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    cTokenGroup = afl::convert(tokenGroup);
    afl::free(cTokenGroup->token);
    cTokenGroup->token = nullptr;
    tmpTokenGroup = afl::convert(cTokenGroup);
    tokenGroup.token = nullptr;
    ASSERT_EQ(tmpTokenGroup, tokenGroup);
    tokenGroup.token = std::make_shared<afl::Token<std::string>>(token);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    cTokenGroup = afl::convert(tokenGroup);
    apl::freeMemory(cTokenGroup->groupID);
    cTokenGroup->groupID = nullptr;
    tmpTokenGroup = afl::convert(cTokenGroup);
    tokenGroup.groupID.clear();
    ASSERT_EQ(tmpTokenGroup, tokenGroup);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);
}

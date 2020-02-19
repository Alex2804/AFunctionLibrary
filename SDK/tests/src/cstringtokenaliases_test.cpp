#include "gtest/gtest.h"

#define A_PLUGIN_SDK_EXCLUDE_DEFINITIONS
#define A_PLUGIN_SDK_DEBUG
#include "../../AFunctionLibrary/implementation/cstringtoken.hpp"

GTEST_TEST(CStringTokenAliases_Test, equal_operator)
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

GTEST_TEST(CStringTokenAliases_Test, free)
{
    apl::debug::allocationCount = 0;
    apl::debug::freeCount = 0;

    afl::free(static_cast<afl::CStringTokenAliases*>(nullptr));
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    afl::CStringTokenAliases tmpCAliases = {apl::freeMemory, afl::TokenAliasType::String, 2, static_cast<afl::CString**>(apl::allocateMemory(sizeof(void*) * 2))};
    auto cAliases = static_cast<afl::CStringTokenAliases*>(apl::allocateMemory(sizeof(afl::CStringTokenAliases)));
    memcpy(cAliases, &tmpCAliases, sizeof(afl::CStringTokenAliases));
    cAliases->aliases[0] = afl::convert("Test string 1!");
    cAliases->aliases[1] = afl::convert("Test string 2!");
    afl::free(cAliases);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    tmpCAliases.aliasesCount = 0;
    tmpCAliases.aliases = nullptr;
    cAliases = static_cast<afl::CStringTokenAliases*>(apl::allocateMemory(sizeof(afl::CStringTokenAliases)));
    memcpy(cAliases, &tmpCAliases, sizeof(afl::CStringTokenAliases));
    afl::free(cAliases);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);
}

GTEST_TEST(CStringTokenAliases_Test, convert)
{
    apl::debug::allocationCount = 0;
    apl::debug::freeCount = 0;

    auto aliases = afl::TokenAliases<std::string>(afl::TokenAliasType::String, {"alias1", "alias2"});

    ASSERT_EQ(afl::convert(static_cast<afl::CStringTokenAliases*>(nullptr)), afl::TokenAliases<std::string>(afl::TokenAliasType::String));
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    afl::CStringTokenAliases* cAliases = afl::convert(aliases);
    ASSERT_EQ(cAliases->type, aliases.type);
    ASSERT_EQ(cAliases->aliasesCount, 2);
    ASSERT_STREQ(cAliases->aliases[0]->string, "alias1");
    ASSERT_STREQ(cAliases->aliases[1]->string, "alias2");
    afl::TokenAliases<std::string> tmpAliases = afl::convert(cAliases);
    ASSERT_EQ(tmpAliases, aliases);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    cAliases = afl::convert(aliases);
    for(size_t i = 0; i < cAliases->aliasesCount; ++i)
        afl::free(cAliases->aliases[i]);
    apl::freeMemory(cAliases->aliases);
    cAliases->aliases = nullptr;
    tmpAliases = afl::convert(cAliases);
    aliases.aliases.clear();
    ASSERT_EQ(tmpAliases, aliases);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);
}

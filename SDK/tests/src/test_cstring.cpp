#include "gtest/gtest.h"

#include "../../AFunctionLibrary/implementation/cstring.hpp"

GTEST_TEST(Test_CString, equal_operator)
{
    auto cString1 = static_cast<afl::CString*>(std::malloc(sizeof(afl::CString)));
    auto cString2 = static_cast<afl::CString*>(std::malloc(sizeof(afl::CString)));
    auto cString3 = static_cast<afl::CString*>(std::malloc(sizeof(afl::CString)));

    cString1->string = static_cast<char*>(malloc(17 * sizeof(char)));
    memcpy(cString1->string, "Test string 1...", 17);
    cString2->string = static_cast<char*>(malloc(17 * sizeof(char)));
    memcpy(cString2->string, cString1->string, 17);
    cString3->string = static_cast<char*>(malloc(17 * sizeof(char)));
    memcpy(cString3->string, "Test string 2...", 17);

    ASSERT_TRUE(*cString1 == *cString2);
    ASSERT_TRUE(*cString1 == *cString1);
    ASSERT_FALSE(*cString1 == *cString3);

    ASSERT_FALSE(*cString1 != *cString2);
    ASSERT_FALSE(*cString1 != *cString1);
    ASSERT_TRUE(*cString1 != *cString3);

    ASSERT_TRUE(afl::equal(cString1, cString1));
    ASSERT_TRUE(afl::equal(cString1, cString2));
    ASSERT_FALSE(afl::equal(cString1, cString3));
    ASSERT_FALSE(afl::equal(cString2, cString3));
    ASSERT_FALSE(afl::equal(cString1, nullptr));
    ASSERT_FALSE(afl::equal(nullptr, cString1));
    ASSERT_TRUE(afl::equal(static_cast<afl::CString*>(nullptr), nullptr));

    std::free(cString1->string);
    std::free(cString2->string);
    std::free(cString3->string);
    std::free(cString1);
    std::free(cString2);
    std::free(cString3);
}

GTEST_TEST(Test_CString, free)
{
    apl::debug::allocationCount = 0;
    apl::debug::freeCount = 0;

    afl::free(static_cast<afl::CString*>(nullptr));
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    afl::CString tmpCString = {apl::freeMemory, static_cast<char*>(apl::allocateMemory(101))};
    auto cString = static_cast<afl::CString*>(apl::allocateMemory(sizeof(afl::CString)));
    memcpy(cString, &tmpCString, sizeof(afl::CString));
    afl::free(cString);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    tmpCString.string = nullptr;
    cString = static_cast<afl::CString*>(apl::allocateMemory(sizeof(afl::CString)));
    memcpy(cString, &tmpCString, sizeof(afl::CString));
    afl::free(cString);
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);
}

GTEST_TEST(Test_CString, convert)
{
    apl::debug::allocationCount = 0;
    apl::debug::freeCount = 0;

    ASSERT_EQ(afl::convert(static_cast<afl::CString*>(nullptr)), "");

    afl::CString* cString = afl::convert("This is a test String!");
    ASSERT_STREQ(cString->string, "This is a test String!");
    std::string string = afl::convert(cString);
    ASSERT_EQ(string, "This is a test String!");
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);

    cString = afl::convert("This is a test String!");
    cString->freeFunction(cString->string);
    cString->string = nullptr;
    string = afl::convert(cString);
    ASSERT_EQ(string, "");
    ASSERT_EQ(apl::debug::freeCount, apl::debug::allocationCount);
}

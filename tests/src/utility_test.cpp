#include "gtest/gtest.h"

#include <ostream>

#include "AFunctionLibrary/utility.h"

#include "test_include.h"

GTEST_TEST(Utility_Test, stringify_templateSpecializations)
{
    ASSERT_EQ(afl::stringify('c'), "c");
    ASSERT_EQ(afl::stringify('9'), "9");

    ASSERT_EQ(afl::stringify<float>(12.3231), "12.3231");
    ASSERT_EQ(afl::stringify<float>(-12.3231), "-12.3231");
    ASSERT_EQ(afl::stringify<float>(-12.0), "-12");
    ASSERT_EQ(afl::stringify<float>(0.0), "0");

    ASSERT_EQ(afl::stringify<double>(12.3231), "12.3231");
    ASSERT_EQ(afl::stringify<double>(-12.3231), "-12.3231");
    ASSERT_EQ(afl::stringify<double>(-12.0), "-12");
    ASSERT_EQ(afl::stringify<double>(0.0), "0");

    ASSERT_EQ(afl::stringify<long double>(12.3231), "12.3231");
    ASSERT_EQ(afl::stringify<long double>(-12.3231), "-12.3231");
    ASSERT_EQ(afl::stringify<long double>(-12.0), "-12");
    ASSERT_EQ(afl::stringify<long double>(0.0), "0");
}
GTEST_TEST(Utility_Test, stringify_toStringSupport)
{
    ASSERT_EQ(afl::stringify<int>(123100), "123100");
    ASSERT_EQ(afl::stringify<int>(-123100), "-123100");
    ASSERT_EQ(afl::stringify<int>(0), "0");

    ASSERT_EQ(afl::stringify<long int>(123100), "123100");
    ASSERT_EQ(afl::stringify<long int>(-123100), "-123100");
    ASSERT_EQ(afl::stringify<long int>(0), "0");

    ASSERT_EQ(afl::stringify<long long int>(123100), "123100");
    ASSERT_EQ(afl::stringify<long long int>(-123100), "-123100");
    ASSERT_EQ(afl::stringify<long long int>(0), "0");

    ASSERT_EQ(afl::stringify<unsigned int>(123100), "123100");
    ASSERT_EQ(afl::stringify<unsigned int>(0), "0");

    ASSERT_EQ(afl::stringify<unsigned long int>(123100), "123100");
    ASSERT_EQ(afl::stringify<unsigned long int>(0), "0");

    ASSERT_EQ(afl::stringify<unsigned long long int>(123100), "123100");
    ASSERT_EQ(afl::stringify<unsigned long long int>(0), "0");
}
GTEST_TEST(Utility_Test, stringify_ostreamOperator)
{
    ASSERT_EQ(afl::stringify<afl::test::Token>({18, 3}), "(18, 3)");
    ASSERT_EQ(afl::stringify<afl::test::Token>({6, 6}), "(6, 6)");
    ASSERT_EQ(afl::stringify<afl::test::Token>({3, 6}), "(3, 6)");
}

GTEST_TEST(Utility_Test, splitAtSpaces)
{
    ASSERT_EQ(afl::splitAtSpaces("  no_splitting-for:this;string?!  "),
              std::vector<std::string>({"no_splitting-for:this;string?!"}));
    ASSERT_EQ(afl::splitAtSpaces("     hello you beautiful world!    "),
              std::vector<std::string>({"hello", "you", "beautiful", "world!"}));
    ASSERT_EQ(afl::splitAtSpaces(" 0 1 2 3 4 5 6 7 8 9 10 11 ... "),
              std::vector<std::string>({"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "..."}));
}

GTEST_TEST(Utility_Test, trimString)
{
    ASSERT_EQ(afl::trimString(""), "");
    ASSERT_EQ(afl::trimString("       \n      "), "");
    ASSERT_EQ(afl::trimString(" \n  This is a string to trim!!!  \n  "), "This is a string to trim!!!");
    ASSERT_EQ(afl::trimString("This is no\nstring to trim!!!"), "This is no\nstring to trim!!!");
    ASSERT_EQ(afl::trimString("   f   testing bounds!   e   "), "f   testing bounds!   e");
}

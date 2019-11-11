#include "gtest/gtest.h"

#include <ostream>

#include "AFunctionLibrary/utility.h"

namespace afl
{
    namespace test
    {
        struct Date
        {
            int day, month, year;
        };

        std::ostream& operator<<(std::ostream& os, const Date& dt)
        {
            os << dt.day << '.' << dt.month << '.' << dt.year;
            return os;
        }
    }
}

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
    ASSERT_EQ(afl::stringify<afl::test::Date>({18, 3, 1998}), "18.3.1998");
    ASSERT_EQ(afl::stringify<afl::test::Date>({6, 6, 2000}), "6.6.2000");
    ASSERT_EQ(afl::stringify<afl::test::Date>({3, 6, 2005}), "3.6.2005");
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

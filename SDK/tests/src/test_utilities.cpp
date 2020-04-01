#include "gtest/gtest.h"

#include "../../AFunctionLibrary/implementation/include/AFunctionLibrary/utilites.h"

#include "include.h"

GTEST_TEST(Test_utilities_h, stringify_templateSpecializations)
{
    ASSERT_EQ(afl::stringify('c'), "c");
    ASSERT_EQ(afl::stringify('9'), "9");

    ASSERT_EQ(afl::stringify<float>(static_cast<float>(12.3231)), "12.3231");
    ASSERT_EQ(afl::stringify<float>(static_cast<float>(-12.3231)), "-12.3231");
    ASSERT_EQ(afl::stringify<float>(static_cast<float>(-12.0)), "-12");
    ASSERT_EQ(afl::stringify<float>(static_cast<float>(0.0)), "0");

    ASSERT_EQ(afl::stringify<double>(12.3231), "12.3231");
    ASSERT_EQ(afl::stringify<double>(-12.3231), "-12.3231");
    ASSERT_EQ(afl::stringify<double>(-12.0), "-12");
    ASSERT_EQ(afl::stringify<double>(0.0), "0");

    ASSERT_EQ(afl::stringify<long double>(12.3231), "12.3231");
    ASSERT_EQ(afl::stringify<long double>(-12.3231), "-12.3231");
    ASSERT_EQ(afl::stringify<long double>(-12.0), "-12");
    ASSERT_EQ(afl::stringify<long double>(0.0), "0");
}
GTEST_TEST(Test_utilities_h, stringify_toStringSupport)
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
GTEST_TEST(Test_utilities_h, stringify_ostreamOperator)
{
    ASSERT_EQ(afl::stringify<afl::test::Point>({18, 3}), "(18, 3)");
    ASSERT_EQ(afl::stringify<afl::test::Point>({6, 6}), "(6, 6)");
    ASSERT_EQ(afl::stringify<afl::test::Point>({3, 6}), "(3, 6)");
}

GTEST_TEST(Test_utilities_h, splitAtSpaces)
{
    ASSERT_EQ(afl::splitAtSpaces("  no_splitting-for:this;string?!  "),
              std::vector<std::string>({"no_splitting-for:this;string?!"}));
    ASSERT_EQ(afl::splitAtSpaces("     hello you beautiful world!    "),
              std::vector<std::string>({"hello", "you", "beautiful", "world!"}));
    ASSERT_EQ(afl::splitAtSpaces(" 0 1 2 3 4 5 6 7 8 9 10 11 ... "),
              std::vector<std::string>({"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "..."}));
}

GTEST_TEST(Test_utilities_h, trimString)
{
    ASSERT_EQ(afl::trimString(""), "");
    ASSERT_EQ(afl::trimString("       \n      "), "");
    ASSERT_EQ(afl::trimString(" \n  This is a string to trim!!!  \n  "), "This is a string to trim!!!");
    ASSERT_EQ(afl::trimString("This is no\nstring to trim!!!"), "This is no\nstring to trim!!!");
    ASSERT_EQ(afl::trimString("   f   testing bounds!   e   "), "f   testing bounds!   e");
}

GTEST_TEST(Test_utilities_h, replaceString)
{
    ASSERT_EQ(afl::replaceString("", "+", "-"), "");
    ASSERT_EQ(afl::replaceString("+", "+", "-"), "-");
    ASSERT_EQ(afl::replaceString("++", "+", "-"), "--");
    ASSERT_EQ(afl::replaceString("adsklfjasdf+lkajsdf+ laksjdf +    klöjasd + +++", "+", "-"), "adsklfjasdf-lkajsdf- laksjdf -    klöjasd - ---");
    ASSERT_EQ(afl::replaceString("Hello Test lloll", "ll", "$"), "He$o Test $o$");
    ASSERT_EQ(afl::replaceString("Hello Test lloll", "l", "..."), "He......o Test ......o......");

    ASSERT_EQ(afl::replaceString("Hello Test", {"ll", "T", "t"}, "!"), "He!o !es!");

    ASSERT_EQ(afl::replaceString("Hello Test", {{{"ll", "T", "t"}, "!"}, {{"o", "e"}, "O.K."}}), "HO.K.!O.K. !O.K.s!");
}

GTEST_TEST(Test_utilities_h, is_dereferencable)
{
    ASSERT_FALSE(afl::detail::is_dereferenceable<int>::value);
    ASSERT_FALSE(afl::detail::is_dereferenceable<int&>::value);
    ASSERT_TRUE(afl::detail::is_dereferenceable<int*>::value);
    ASSERT_TRUE(afl::detail::is_dereferenceable<std::shared_ptr<int>>::value);
    ASSERT_TRUE(afl::detail::is_dereferenceable<afl::test::Point>::value);
    ASSERT_TRUE(afl::detail::is_dereferenceable<afl::test::Point>::value);
}
GTEST_TEST(Test_utilities_h, is_arrow_dereferencable)
{
    ASSERT_FALSE(afl::detail::is_arrow_dereferenceable<int>::value);
    ASSERT_FALSE(afl::detail::is_arrow_dereferenceable<int&>::value);
    ASSERT_FALSE(afl::detail::is_arrow_dereferenceable<afl::test::Point>::value);
    ASSERT_TRUE(afl::detail::is_arrow_dereferenceable<int*>::value);
    ASSERT_TRUE(afl::detail::is_arrow_dereferenceable<std::shared_ptr<int>>::value);
    ASSERT_TRUE(afl::detail::is_arrow_dereferenceable<afl::test::Point*>::value);
}

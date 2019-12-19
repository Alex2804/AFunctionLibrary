#include "gtest/gtest.h"

#include "test_include.h"

#include "AFunctionLibrary/utility.h"

GTEST_TEST(utility_h_Test, stringify_templateSpecializations)
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
GTEST_TEST(utility_h_Test, stringify_toStringSupport)
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
GTEST_TEST(utility_h_Test, stringify_ostreamOperator)
{
    ASSERT_EQ(afl::stringify<afl::test::Token>({18, 3}), "(18, 3)");
    ASSERT_EQ(afl::stringify<afl::test::Token>({6, 6}), "(6, 6)");
    ASSERT_EQ(afl::stringify<afl::test::Token>({3, 6}), "(3, 6)");
}

GTEST_TEST(utility_h_Test, splitAtSpaces)
{
    ASSERT_EQ(afl::splitAtSpaces("  no_splitting-for:this;string?!  "),
              std::vector<std::string>({"no_splitting-for:this;string?!"}));
    ASSERT_EQ(afl::splitAtSpaces("     hello you beautiful world!    "),
              std::vector<std::string>({"hello", "you", "beautiful", "world!"}));
    ASSERT_EQ(afl::splitAtSpaces(" 0 1 2 3 4 5 6 7 8 9 10 11 ... "),
              std::vector<std::string>({"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "..."}));
}

GTEST_TEST(utility_h_Test, trimString)
{
    ASSERT_EQ(afl::trimString(""), "");
    ASSERT_EQ(afl::trimString("       \n      "), "");
    ASSERT_EQ(afl::trimString(" \n  This is a string to trim!!!  \n  "), "This is a string to trim!!!");
    ASSERT_EQ(afl::trimString("This is no\nstring to trim!!!"), "This is no\nstring to trim!!!");
    ASSERT_EQ(afl::trimString("   f   testing bounds!   e   "), "f   testing bounds!   e");
}

GTEST_TEST(utility_h_Test, replaceString)
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

GTEST_TEST(utility_h_Test, is_dereferencable)
{
    ASSERT_FALSE(afl::detail::is_dereferenceable<int>::value);
    ASSERT_FALSE(afl::detail::is_dereferenceable<int&>::value);
    ASSERT_TRUE(afl::detail::is_dereferenceable<int*>::value);
    ASSERT_TRUE(afl::detail::is_dereferenceable<std::shared_ptr<int>>::value);
    ASSERT_TRUE(afl::detail::is_dereferenceable<afl::test::Token>::value);
    ASSERT_TRUE(afl::detail::is_dereferenceable<afl::test::Token*>::value);
}
GTEST_TEST(utility_h_Test, is_arrow_dereferencable)
{
    ASSERT_FALSE(afl::detail::is_arrow_dereferenceable<int>::value);
    ASSERT_FALSE(afl::detail::is_arrow_dereferenceable<int&>::value);
    ASSERT_FALSE(afl::detail::is_arrow_dereferenceable<afl::test::Token>::value);
    ASSERT_TRUE(afl::detail::is_arrow_dereferenceable<int*>::value);
    ASSERT_TRUE(afl::detail::is_arrow_dereferenceable<std::shared_ptr<int>>::value);
    ASSERT_TRUE(afl::detail::is_arrow_dereferenceable<afl::test::Token*>::value);
}


GTEST_TEST(token_h_Test, shuntingYard)
{
    // 3+5*abs(ab)^3*pow(3^5;abs(3+ab))
    std::vector<std::shared_ptr<const afl::Token<std::string>>> sharedPtrTokens = {
            afl::test::token3, afl::test::tokenPlus, afl::test::token5, afl::test::tokenMultiply, afl::test::tokenAbs,
            afl::test::tokenBracketOpen, afl::test::tokenAb, afl::test::tokenBracketClose, afl::test::tokenPower,
            afl::test::token3, afl::test::tokenMultiply, afl::test::tokenPow, afl::test::tokenBracketOpen,
            afl::test::token3, afl::test::tokenPower, afl::test::token5, afl::test::tokenSemicolon, afl::test::tokenAbs,
            afl::test::tokenBracketOpen, afl::test::token3, afl::test::tokenPlus, afl::test::tokenAb,
            afl::test::tokenBracketClose, afl::test::tokenBracketClose
    };
    std::vector<afl::Token<std::string>> instanceTokens;
    std::vector<const afl::Token<std::string>*> ptrTokens;
    std::vector<std::shared_ptr<const afl::TokenGroup<std::string>>> sharedPtrTokenGroups;
    std::vector<afl::TokenGroup<std::string>> instanceTokenGroups;
    std::vector<const afl::TokenGroup<std::string>*> ptrTokenGroups;
    instanceTokens.reserve(sharedPtrTokens.size());
    ptrTokens.reserve(sharedPtrTokens.size());
    sharedPtrTokenGroups.reserve(sharedPtrTokens.size());
    instanceTokenGroups.reserve(sharedPtrTokens.size());
    ptrTokenGroups.reserve(sharedPtrTokens.size());
    for(const auto& token : sharedPtrTokens) {
        instanceTokens.push_back(*token);
        ptrTokens.push_back(token.get());
        sharedPtrTokenGroups.emplace_back(new afl::TokenGroup<std::string>(token, afl::test::generateRandomGroupID()));
        instanceTokenGroups.push_back(*sharedPtrTokenGroups.back());
        ptrTokenGroups.push_back(sharedPtrTokenGroups.back().get());
    }
    // 3 5 ab abs 3 ^ * 3 5 ^ 3 ab + abs pow * +
    std::vector<std::shared_ptr<const afl::Token<std::string>>> expected = {
            afl::test::token3, afl::test::token5, afl::test::tokenAb, afl::test::tokenAbs, afl::test::token3,
            afl::test::tokenPower, afl::test::tokenMultiply, afl::test::token3, afl::test::token5, afl::test::tokenPower,
            afl::test::token3, afl::test::tokenAb, afl::test::tokenPlus, afl::test::tokenAbs, afl::test::tokenPow,
            afl::test::tokenMultiply, afl::test::tokenPlus
    };

    std::vector<std::shared_ptr<const afl::Token<std::string>>> sharedPtrTokensResult = afl::shuntingYard(sharedPtrTokens);
    std::vector<afl::Token<std::string>> instanceTokensResult = afl::shuntingYard(instanceTokens);
    std::vector<const afl::Token<std::string>*> ptrTokensResult = afl::shuntingYard(ptrTokens);
    std::vector<std::shared_ptr<const afl::TokenGroup<std::string>>> sharedPtrTokenGroupsResult = afl::shuntingYard(sharedPtrTokenGroups);
    std::vector<afl::TokenGroup<std::string>> instanceTokenGroupsResult = afl::shuntingYard(instanceTokenGroups);
    std::vector<const afl::TokenGroup<std::string>*> ptrTokenGroupsResult = afl::shuntingYard(ptrTokenGroups);

    ASSERT_EQ(sharedPtrTokensResult.size(), expected.size());
    ASSERT_EQ(instanceTokensResult.size(), expected.size());
    ASSERT_EQ(ptrTokensResult.size(), expected.size());
    ASSERT_EQ(sharedPtrTokenGroupsResult.size(), expected.size());
    ASSERT_EQ(instanceTokenGroupsResult.size(), expected.size());
    ASSERT_EQ(ptrTokenGroupsResult.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        ASSERT_EQ(*sharedPtrTokensResult[i], *expected[i]);
        ASSERT_EQ(instanceTokensResult[i], *expected[i]);
        ASSERT_EQ(*ptrTokensResult[i], *expected[i]);
        ASSERT_EQ(*sharedPtrTokenGroupsResult[i]->token, *expected[i]);
        ASSERT_EQ(*instanceTokenGroupsResult[i].token, *expected[i]);
        ASSERT_EQ(*ptrTokenGroupsResult[i]->token, *expected[i]);
    }
}

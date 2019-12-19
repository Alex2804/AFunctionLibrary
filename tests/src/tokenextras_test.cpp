#include "gtest/gtest.h"

#include "test_include.h"

#include "../../src/private/tokenextras.h"

GTEST_TEST(tokenextras_h_Test, tokensToString)
{
    std::vector<std::shared_ptr<const afl::Token<std::string>>> tokens;
    ASSERT_EQ(afl::detail::tokensToString(tokens), "");
    tokens = {afl::test::token3, afl::test::tokenPlus, afl::test::token5, afl::test::tokenMultiply, afl::test::tokenAbs, afl::test::tokenBracketOpen, afl::test::tokenAb, afl::test::tokenBracketClose};
    ASSERT_EQ(afl::detail::tokensToString(tokens), "3+5*abs(ab)");
}

GTEST_TEST(tokenextras_h_Test, toGroupVector)
{
    // 3+5*abs(ab)^3*pow(3^5;abs(3+ab))
    std::vector<std::shared_ptr<const afl::Token<std::string>>> tokens = {
        afl::test::token3, afl::test::tokenPlus, afl::test::token5, afl::test::tokenMultiply, afl::test::tokenAbs,
        afl::test::tokenBracketOpen, afl::test::tokenAb, afl::test::tokenBracketClose, afl::test::tokenPower,
        afl::test::token3, afl::test::tokenMultiply, afl::test::tokenPow, afl::test::tokenBracketOpen,
        afl::test::token3, afl::test::tokenPower, afl::test::token5, afl::test::tokenSemicolon, afl::test::tokenAbs,
        afl::test::tokenBracketOpen, afl::test::token3, afl::test::tokenPlus, afl::test::tokenAb,
        afl::test::tokenBracketClose, afl::test::tokenBracketClose
    };

    std::vector<afl::TokenGroup<std::string>> expected, tokenGroups = afl::detail::toGroupVector(tokens);
    expected.emplace_back(afl::test::token3, std::vector<size_t>({0}));
    expected.emplace_back(afl::test::tokenPlus, std::vector<size_t>({1}));
    expected.emplace_back(afl::test::token5, std::vector<size_t>({2}));
    expected.emplace_back(afl::test::tokenMultiply, std::vector<size_t>({3}));
    expected.emplace_back(afl::test::tokenAbs, std::vector<size_t>({4}));
    expected.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({5}));
    expected.emplace_back(afl::test::tokenAb, std::vector<size_t>({6, 0}));
    expected.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({7}));
    expected.emplace_back(afl::test::tokenPower, std::vector<size_t>({8}));
    expected.emplace_back(afl::test::token3, std::vector<size_t>({9}));
    expected.emplace_back(afl::test::tokenMultiply, std::vector<size_t>({10}));
    expected.emplace_back(afl::test::tokenPow, std::vector<size_t>({11}));
    expected.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({12}));
    expected.emplace_back(afl::test::token3, std::vector<size_t>({13, 0}));
    expected.emplace_back(afl::test::tokenPower, std::vector<size_t>({13, 1}));
    expected.emplace_back(afl::test::token5, std::vector<size_t>({13, 2}));
    expected.emplace_back(afl::test::tokenSemicolon, std::vector<size_t>({14}));
    expected.emplace_back(afl::test::tokenAbs, std::vector<size_t>({15, 0}));
    expected.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({15, 1}));
    expected.emplace_back(afl::test::token3, std::vector<size_t>({15, 2, 0}));
    expected.emplace_back(afl::test::tokenPlus, std::vector<size_t>({15, 2, 1}));
    expected.emplace_back(afl::test::tokenAb, std::vector<size_t>({15, 2, 2}));
    expected.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({15, 3}));
    expected.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({16}));

    ASSERT_EQ(tokenGroups, expected);
}

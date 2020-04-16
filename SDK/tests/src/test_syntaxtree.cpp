#include "gtest/gtest.h"

#include "include.h"

#include "../../AFunctionLibrary/implementation/include/AFunctionLibrary/syntaxtree.h"

GTEST_TEST(Test_SyntaxTree, constructors)
{
    auto tokenSyntaxTree = new afl::SyntaxTree<afl::test::Point>();
    auto intSyntaxTree = new afl::SyntaxTree<int>();
    ASSERT_EQ(tokenSyntaxTree->toString(), "");
    ASSERT_EQ(intSyntaxTree->toString(), "");
    delete tokenSyntaxTree;
    delete intSyntaxTree;

    tokenSyntaxTree = new afl::SyntaxTree<afl::test::Point>({5, 6});
    intSyntaxTree = new afl::SyntaxTree<int>(42);
    ASSERT_EQ(tokenSyntaxTree->toString(), "(5, 6)");
    ASSERT_EQ(intSyntaxTree->toString(), "42");
    delete tokenSyntaxTree;
    delete intSyntaxTree;

    auto tokenNode = std::unique_ptr<afl::Node<afl::test::Point>>(new afl::Node<afl::test::Point>({5, 6}));
    auto intNode = std::unique_ptr<afl::Node<int>>(new afl::Node<int>(42));
    tokenSyntaxTree = new afl::SyntaxTree<afl::test::Point>(std::move(tokenNode));
    intSyntaxTree = new afl::SyntaxTree<int>(std::move(intNode));
    ASSERT_EQ(tokenSyntaxTree->toString(), "(5, 6)");
    ASSERT_EQ(intSyntaxTree->toString(), "42");
    delete tokenSyntaxTree;
    delete intSyntaxTree;

    tokenSyntaxTree = new afl::SyntaxTree<afl::test::Point>(std::unique_ptr<afl::Node<afl::test::Point>>(new afl::Node<afl::test::Point>(afl::test::createPointNode(42, 5, 6, 4))));
    intSyntaxTree = new afl::SyntaxTree<int>(std::unique_ptr<afl::Node<int>>(new afl::Node<int>(afl::test::createIntNode(42, -1, 1, 4))));
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedIntNodeString);
    delete tokenSyntaxTree;
    delete intSyntaxTree;
}

GTEST_TEST(Test_SyntaxTree, copy_constructor_assignment)
{
    // copy constructor
    auto tokenSyntaxTree = new afl::SyntaxTree<afl::test::Point>(std::unique_ptr<afl::Node<afl::test::Point>>(new afl::Node<afl::test::Point>(afl::test::createPointNode(42, 5, 6, 4))));
    auto intSyntaxTree = new afl::SyntaxTree<int>(std::unique_ptr<afl::Node<int>>(new afl::Node<int>(afl::test::createIntNode(42, -1, 1, 4))));
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedIntNodeString);

    auto tokenSyntaxTreeRef = afl::SyntaxTree<afl::test::Point>(*tokenSyntaxTree);
    auto intSyntaxTreeRef = afl::SyntaxTree<int>(*intSyntaxTree);
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedIntNodeString);
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedIntNodeString);
    delete tokenSyntaxTree;
    delete intSyntaxTree;
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedIntNodeString);

    // copy assignment
    tokenSyntaxTree = new afl::SyntaxTree<afl::test::Point>(std::unique_ptr<afl::Node<afl::test::Point>>(new afl::Node<afl::test::Point>(afl::test::createPointNode(42, 5, 6, 4))));
    intSyntaxTree = new afl::SyntaxTree<int>(std::unique_ptr<afl::Node<int>>(new afl::Node<int>(afl::test::createIntNode(42, -1, 1, 4))));
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedIntNodeString);

    tokenSyntaxTreeRef = *tokenSyntaxTree;
    intSyntaxTreeRef = *intSyntaxTree;
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedIntNodeString);
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedIntNodeString);
    delete tokenSyntaxTree;
    delete intSyntaxTree;
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedIntNodeString);

    // self assignment
    tokenSyntaxTreeRef = tokenSyntaxTreeRef;
    intSyntaxTreeRef = intSyntaxTreeRef;
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedIntNodeString);
}

GTEST_TEST(Test_SyntaxTree, move_constructor_assignment)
{
    // move constructor
    auto tokenSyntaxTree = new afl::SyntaxTree<afl::test::Point>(std::unique_ptr<afl::Node<afl::test::Point>>(new afl::Node<afl::test::Point>(afl::test::createPointNode(42, 5, 6, 4))));
    auto intSyntaxTree = new afl::SyntaxTree<int>(std::unique_ptr<afl::Node<int>>(new afl::Node<int>(afl::test::createIntNode(42, -1, 1, 4))));
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedIntNodeString);

    auto tokenSyntaxTreeRef = afl::SyntaxTree<afl::test::Point>(std::move(*tokenSyntaxTree));
    auto intSyntaxTreeRef = afl::SyntaxTree<int>(std::move(*intSyntaxTree));
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedIntNodeString);
    delete tokenSyntaxTree;
    delete intSyntaxTree;
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedIntNodeString);

    // move assignment
    tokenSyntaxTree = new afl::SyntaxTree<afl::test::Point>(std::unique_ptr<afl::Node<afl::test::Point>>(new afl::Node<afl::test::Point>(afl::test::createPointNode(42, 5, 6, 4))));
    intSyntaxTree = new afl::SyntaxTree<int>(std::unique_ptr<afl::Node<int>>(new afl::Node<int>(afl::test::createIntNode(42, -1, 1, 4))));
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedIntNodeString);

    tokenSyntaxTreeRef = std::move(*tokenSyntaxTree);
    intSyntaxTreeRef = std::move(*intSyntaxTree);
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedIntNodeString);
    delete tokenSyntaxTree;
    delete intSyntaxTree;
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedIntNodeString);
}


GTEST_TEST(Test_syntaxtree_h, shuntingYard)
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


GTEST_TEST(Test_syntaxtree_h, generateSyntaxTree)
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
    std::vector<std::shared_ptr<afl::TokenGroup<std::string>>> sharedPtrTokenGroups;
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
        sharedPtrTokenGroups.back()->printGroupID = false;
        instanceTokenGroups.push_back(*sharedPtrTokenGroups.back());
        ptrTokenGroups.push_back(sharedPtrTokenGroups.back().get());
    }

    afl::SyntaxTree<std::shared_ptr<const afl::Token<std::string>>> sharedPtrTokensSyntaxTree = afl::generateSyntaxTree(sharedPtrTokens);
    afl::SyntaxTree<afl::Token<std::string>> instanceTokensSyntaxTree = afl::generateSyntaxTree(instanceTokens);
    afl::SyntaxTree<const afl::Token<std::string>*> ptrTokensSyntaxTree = afl::generateSyntaxTree(ptrTokens);
    afl::SyntaxTree<std::shared_ptr<afl::TokenGroup<std::string>>> sharedPtrTokenGroupsSyntaxTree = afl::generateSyntaxTree(sharedPtrTokenGroups);
    afl::SyntaxTree<afl::TokenGroup<std::string>> instanceTokenGroupsSyntaxTree = afl::generateSyntaxTree(instanceTokenGroups);
    afl::SyntaxTree<const afl::TokenGroup<std::string>*> ptrTokenGroupsSyntaxTree = afl::generateSyntaxTree(ptrTokenGroups);

    std::string expected = "       +                        \n\xE2\x94\x8C\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90                \n3              *                \n       \xE2\x94\x8C\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90       \n       *               pow      \n    \xE2\x94\x8C\xE2\x94\x80\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90        \xE2\x94\x8C\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90   \n    5      ^        ^      abs  \n         \xE2\x94\x8C\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90   \xE2\x94\x8C\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x90     |   \n        abs   3   3   5     +   \n         |                \xE2\x94\x8C\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90\n        ab                3   ab";

    ASSERT_EQ(sharedPtrTokensSyntaxTree.toString(), expected);
    ASSERT_EQ(instanceTokensSyntaxTree.toString(), expected);
    ASSERT_EQ(ptrTokensSyntaxTree.toString(), expected);
    ASSERT_EQ(sharedPtrTokenGroupsSyntaxTree.toString(), expected);
    ASSERT_EQ(instanceTokenGroupsSyntaxTree.toString(), expected);
    ASSERT_EQ(ptrTokenGroupsSyntaxTree.toString(), expected);
}

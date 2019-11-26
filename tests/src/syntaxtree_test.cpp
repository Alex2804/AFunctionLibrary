#include "gtest/gtest.h"

#include "test_include.h"

GTEST_TEST(SyntaxTree_Test, constructors)
{
    auto tokenSyntaxTree = new afl::SyntaxTree<afl::test::Token>();
    auto intSyntaxTree = new afl::SyntaxTree<int>();
    ASSERT_EQ(tokenSyntaxTree->toString(), "");
    ASSERT_EQ(intSyntaxTree->toString(), "");
    delete tokenSyntaxTree;
    delete intSyntaxTree;

    tokenSyntaxTree = new afl::SyntaxTree<afl::test::Token>({5, 6});
    intSyntaxTree = new afl::SyntaxTree<int>(42);
    ASSERT_EQ(tokenSyntaxTree->toString(), "(5, 6)");
    ASSERT_EQ(intSyntaxTree->toString(), "42");
    delete tokenSyntaxTree;
    delete intSyntaxTree;

    auto tokenNode = new afl::Node<afl::test::Token>({5, 6});
    auto intNode = new afl::Node<int>(42);
    tokenSyntaxTree = new afl::SyntaxTree<afl::test::Token>(tokenNode);
    intSyntaxTree = new afl::SyntaxTree<int>(intNode);
    ASSERT_EQ(tokenSyntaxTree->toString(), "(5, 6)");
    ASSERT_EQ(intSyntaxTree->toString(), "42");
    ASSERT_EQ(tokenSyntaxTree->root(), tokenNode);
    ASSERT_EQ(intSyntaxTree->root(), intNode);
    delete tokenSyntaxTree;
    delete intSyntaxTree;

    tokenSyntaxTree = new afl::SyntaxTree<afl::test::Token>(afl::test::createTokenNode(42, 5, 6, 4));
    intSyntaxTree = new afl::SyntaxTree<int>(afl::test::createIntNode(42, -1, 1, 4));
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedInt);
    delete tokenSyntaxTree;
    delete intSyntaxTree;
}

GTEST_TEST(SyntaxTree_Test, copy_constructor_assignement)
{
    // copy constructor
    auto tokenSyntaxTree = new afl::SyntaxTree<afl::test::Token>(afl::test::createTokenNode(42, 5, 6, 4));
    auto intSyntaxTree = new afl::SyntaxTree<int>(afl::test::createIntNode(42, -1, 1, 4));
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedInt);

    auto tokenSyntaxTreeRef = afl::SyntaxTree<afl::test::Token>(*tokenSyntaxTree);
    auto intSyntaxTreeRef = afl::SyntaxTree<int>(*intSyntaxTree);
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedInt);
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedInt);
    delete tokenSyntaxTree;
    delete intSyntaxTree;
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedInt);

    // copy assignment
    tokenSyntaxTree = new afl::SyntaxTree<afl::test::Token>(afl::test::createTokenNode(42, 5, 6, 4));
    intSyntaxTree = new afl::SyntaxTree<int>(afl::test::createIntNode(42, -1, 1, 4));
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedInt);

    tokenSyntaxTreeRef = *tokenSyntaxTree;
    intSyntaxTreeRef = *intSyntaxTree;
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedInt);
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedInt);
    delete tokenSyntaxTree;
    delete intSyntaxTree;
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedInt);

    // self assignment
    tokenSyntaxTreeRef = tokenSyntaxTreeRef;
    intSyntaxTreeRef = intSyntaxTreeRef;
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedInt);
}

GTEST_TEST(SyntaxTree_Test, move_constructor_assignement)
{
    // move constructor
    auto tokenSyntaxTree = new afl::SyntaxTree<afl::test::Token>(afl::test::createTokenNode(42, 5, 6, 4));
    auto intSyntaxTree = new afl::SyntaxTree<int>(afl::test::createIntNode(42, -1, 1, 4));
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedInt);

    auto tokenSyntaxTreeRef = afl::SyntaxTree<afl::test::Token>(std::move(*tokenSyntaxTree));
    auto intSyntaxTreeRef = afl::SyntaxTree<int>(std::move(*intSyntaxTree));
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedInt);
    delete tokenSyntaxTree;
    delete intSyntaxTree;
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedInt);

    // move assignment
    tokenSyntaxTree = new afl::SyntaxTree<afl::test::Token>(afl::test::createTokenNode(42, 5, 6, 4));
    intSyntaxTree = new afl::SyntaxTree<int>(afl::test::createIntNode(42, -1, 1, 4));
    ASSERT_EQ(tokenSyntaxTree->toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTree->toString(), afl::test::expectedInt);

    tokenSyntaxTreeRef = std::move(*tokenSyntaxTree);
    intSyntaxTreeRef = std::move(*intSyntaxTree);
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedInt);
    delete tokenSyntaxTree;
    delete intSyntaxTree;
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedInt);

    // self assignment
    tokenSyntaxTreeRef = std::move(tokenSyntaxTreeRef);
    intSyntaxTreeRef = std::move(intSyntaxTreeRef);
    ASSERT_EQ(tokenSyntaxTreeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intSyntaxTreeRef.toString(), afl::test::expectedInt);
}

#include "gtest/gtest.h"

#include "test_include.h"

GTEST_TEST(Node_Test, constructors)
{
    auto tokenNode = new afl::Node<afl::test::Token>({5, 6});
    auto intNode = new afl::Node<int>(42);
    ASSERT_EQ(tokenNode->toString(), "(5, 6)");
    ASSERT_EQ(intNode->toString(), "42");
    delete tokenNode;
    delete intNode;

    tokenNode = afl::test::createTokenNode(42, 5, 6, 4);
    intNode = afl::test::createIntNode(42, -1, 1, 4);
    ASSERT_EQ(tokenNode->toString(), afl::test::expectedToken);
    ASSERT_EQ(intNode->toString(), afl::test::expectedInt);
    delete tokenNode;
    delete intNode;
}

GTEST_TEST(Node_Test, copy_constructor_assignement)
{
    // copy constructor
    auto tokenNode = afl::test::createTokenNode(42, 5, 6, 4);
    auto intNode = afl::test::createIntNode(42, -1, 1, 4);
    ASSERT_EQ(tokenNode->toString(), afl::test::expectedToken);
    ASSERT_EQ(intNode->toString(), afl::test::expectedInt);

    auto tokenNodeRef = afl::Node<afl::test::Token>(*tokenNode);
    auto intNodeRef = afl::Node<int>(*intNode);
    ASSERT_EQ(tokenNode->toString(), afl::test::expectedToken);
    ASSERT_EQ(intNode->toString(), afl::test::expectedInt);
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedInt);
    delete tokenNode;
    delete intNode;
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedInt);

    // copy assignment
    tokenNode = afl::test::createTokenNode(42, 5, 6, 4);
    intNode = afl::test::createIntNode(42, -1, 1, 4);
    ASSERT_EQ(tokenNode->toString(), afl::test::expectedToken);
    ASSERT_EQ(intNode->toString(), afl::test::expectedInt);

    tokenNodeRef = *tokenNode;
    intNodeRef = *intNode;
    ASSERT_EQ(tokenNode->toString(), afl::test::expectedToken);
    ASSERT_EQ(intNode->toString(), afl::test::expectedInt);
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedInt);
    delete tokenNode;
    delete intNode;
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedInt);

    // self assignment
    tokenNodeRef = tokenNodeRef;
    intNodeRef = intNodeRef;
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedInt);
}

GTEST_TEST(Node_Test, move_constructor_assignement)
{
    // move constructor
    auto tokenNode = afl::test::createTokenNode(42, 5, 6, 4);
    auto intNode = afl::test::createIntNode(42, -1, 1, 4);
    ASSERT_EQ(tokenNode->toString(), afl::test::expectedToken);
    ASSERT_EQ(intNode->toString(), afl::test::expectedInt);

    auto tokenNodeRef = afl::Node<afl::test::Token>(std::move(*tokenNode));
    auto intNodeRef = afl::Node<int>(std::move(*intNode));
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedInt);
    delete tokenNode;
    delete intNode;
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedInt);

    // move assignment
    tokenNode = afl::test::createTokenNode(42, 5, 6, 4);
    intNode = afl::test::createIntNode(42, -1, 1, 4);
    ASSERT_EQ(tokenNode->toString(), afl::test::expectedToken);
    ASSERT_EQ(intNode->toString(), afl::test::expectedInt);

    tokenNodeRef = std::move(*tokenNode);
    intNodeRef = std::move(*intNode);
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedInt);
    delete tokenNode;
    delete intNode;
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedInt);

    // self assignment
    tokenNodeRef = std::move(tokenNodeRef);
    intNodeRef = std::move(intNodeRef);
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedToken);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedInt);
}

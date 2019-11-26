#include "gtest/gtest.h"

#include <sstream>

#include "test_include.h"

GTEST_TEST(Node_Test, toString_ostream_operator)
{
    auto tokenNode = new afl::Node<afl::test::Token>({5, 6});
    tokenNode->m_value = afl::test::Token({5, 6});
    ASSERT_EQ(tokenNode->toString(), "(5, 6)");
    std::stringstream stringstream;
    stringstream << *tokenNode;
    ASSERT_EQ(stringstream.str(), "(5, 6)");
    delete tokenNode;

    auto intNode = new afl::Node<int>(42);
    intNode->m_value = 42;
    ASSERT_EQ(intNode->toString(), "42");
    stringstream = std::stringstream();
    stringstream << *intNode;
    ASSERT_EQ(stringstream.str(), "42");
    delete intNode;

    auto tokenNode1 = new afl::Node<afl::test::Token>({321, 123});
    auto tokenNode2 = new afl::Node<afl::test::Token>({227, 953});
    std::vector<afl::Node<afl::test::Token>*> tokenChildren = {tokenNode1, tokenNode2};
    tokenNode = new afl::Node<afl::test::Token>({5123, 690});
    tokenNode->m_value = afl::test::Token({5, 6});
    tokenNode->m_children = tokenChildren;
    ASSERT_EQ(tokenNode->toString(), "        (5, 6)         \n     \xE2\x94\x8C\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90    \n(321, 123)   (227, 953)");
    stringstream = std::stringstream();
    stringstream << *tokenNode;
    ASSERT_EQ(stringstream.str(), "        (5, 6)         \n     \xE2\x94\x8C\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90    \n(321, 123)   (227, 953)");
    delete tokenNode;

    auto intNode1 = new afl::Node<int>(321);
    auto intNode2 = new afl::Node<int>(227);
    std::vector<afl::Node<int>*> intChildren = {intNode1, intNode2};
    intNode = new afl::Node<int>(432);
    intNode->m_value = 432;
    intNode->m_children = intChildren;
    ASSERT_EQ(intNode->toString(), "   432   \n \xE2\x94\x8C\xE2\x94\x80\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90 \n321   227");
    stringstream = std::stringstream();
    stringstream << *intNode;
    ASSERT_EQ(stringstream.str(), "   432   \n \xE2\x94\x8C\xE2\x94\x80\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90 \n321   227");
    delete intNode;
}

GTEST_TEST(Node_Test, constructors)
{
    auto tokenNode = new afl::Node<afl::test::Token>({5, 6});
    ASSERT_TRUE(tokenNode->m_value == afl::test::Token({5, 6}));
    ASSERT_EQ(tokenNode->m_children.size(), 0);
    ASSERT_EQ(tokenNode->toString(), "(5, 6)");
    delete tokenNode;

    auto intNode = new afl::Node<int>(42);
    ASSERT_EQ(intNode->m_value, 42);
    ASSERT_EQ(intNode->m_children.size(), 0);
    ASSERT_EQ(intNode->toString(), "42");
    delete intNode;

    tokenNode = afl::test::createTokenNode(42, 5, 6, 4);
    ASSERT_TRUE(tokenNode->m_value == afl::test::Token({42, -42}));
    ASSERT_EQ(tokenNode->m_children.size(), 2);
    ASSERT_EQ(tokenNode->toString(), afl::test::expectedToken);
    delete tokenNode;

    intNode = afl::test::createIntNode(42, -1, 1, 4);
    ASSERT_EQ(intNode->m_value, 42);
    ASSERT_EQ(intNode->m_children.size(), 3);
    ASSERT_EQ(intNode->toString(), afl::test::expectedInt);
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

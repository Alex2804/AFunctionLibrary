#include "gtest/gtest.h"

#include "../../AFunctionLibrary/implementation/include/AFunctionLibrary/syntaxtree.h"

#include "include.h"

GTEST_TEST(Test_Node, toString_ostream_operator)
{
    auto tokenNode = afl::Node<afl::test::Point>({5, 6});
    tokenNode.m_value = afl::test::Point({5, 6});
    ASSERT_EQ(tokenNode.toString(), "(5, 6)");
    std::stringstream stringstream;
    stringstream << tokenNode;
    ASSERT_EQ(stringstream.str(), "(5, 6)");

    auto intNode = afl::Node<int>(42);
    intNode.m_value = 42;
    ASSERT_EQ(intNode.toString(), "42");
    stringstream = std::stringstream();
    stringstream << intNode;
    ASSERT_EQ(stringstream.str(), "42");

    std::vector<afl::Node<afl::test::Point>> tokenChildren;
    tokenChildren.push_back(afl::Node<afl::test::Point>({321, 123}));
    tokenChildren.push_back(afl::Node<afl::test::Point>({227, 953}));
    tokenNode = afl::Node<afl::test::Point>({5123, 690});
    tokenNode.m_value = afl::test::Point({5, 6});
    tokenNode.m_children = std::move(tokenChildren);
    ASSERT_EQ(tokenNode.toString(), "        (5, 6)         \n     \xE2\x94\x8C\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90    \n(321, 123)   (227, 953)");
    stringstream = std::stringstream();
    stringstream << tokenNode;
    ASSERT_EQ(stringstream.str(), "        (5, 6)         \n     \xE2\x94\x8C\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90    \n(321, 123)   (227, 953)");

    std::vector<afl::Node<int>> intChildren;
    intChildren.emplace_back(321);
    intChildren.emplace_back(227);
    intNode = afl::Node<int>(432);
    intNode.m_value = 432;
    intNode.m_children = std::move(intChildren);
    ASSERT_EQ(intNode.toString(), "   432   \n \xE2\x94\x8C\xE2\x94\x80\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90 \n321   227");
    stringstream = std::stringstream();
    stringstream << intNode;
    ASSERT_EQ(stringstream.str(), "   432   \n \xE2\x94\x8C\xE2\x94\x80\xE2\x94\x80\xE2\x94\xB4\xE2\x94\x80\xE2\x94\x80\xE2\x94\x90 \n321   227");
}

GTEST_TEST(Test_Node, constructors)
{
    auto tokenNode = afl::Node<afl::test::Point>({5, 6});
    ASSERT_TRUE(tokenNode.m_value == afl::test::Point({5, 6}));
    ASSERT_EQ(tokenNode.m_children.size(), 0);
    ASSERT_EQ(tokenNode.toString(), "(5, 6)");

    auto intNode = afl::Node<int>(42);
    ASSERT_EQ(intNode.m_value, 42);
    ASSERT_EQ(intNode.m_children.size(), 0);
    ASSERT_EQ(intNode.toString(), "42");

    tokenNode = afl::test::createPointNode(42, 5, 6, 4);
    ASSERT_TRUE(tokenNode.m_value == afl::test::Point({42, -42}));
    ASSERT_EQ(tokenNode.m_children.size(), 2);
    ASSERT_EQ(tokenNode.toString(), afl::test::expectedPointNodeString);

    intNode = afl::test::createIntNode(42, -1, 1, 4);
    ASSERT_EQ(intNode.m_value, 42);
    ASSERT_EQ(intNode.m_children.size(), 3);
    ASSERT_EQ(intNode.toString(), afl::test::expectedIntNodeString);
}

GTEST_TEST(Test_Node, copy_constructor_assignment)
{
    // copy constructor
    auto tokenNode = afl::test::createPointNode(42, 5, 6, 4);
    auto intNode = afl::test::createIntNode(42, -1, 1, 4);
    ASSERT_EQ(tokenNode.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intNode.toString(), afl::test::expectedIntNodeString);

    auto tokenNodeRef = afl::Node<afl::test::Point>(tokenNode);
    auto intNodeRef = afl::Node<int>(intNode);
    ASSERT_EQ(tokenNode.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intNode.toString(), afl::test::expectedIntNodeString);
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedIntNodeString);

    // copy assignment
    tokenNode = afl::test::createPointNode(42, 5, 6, 4);
    intNode = afl::test::createIntNode(42, -1, 1, 4);
    ASSERT_EQ(tokenNode.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intNode.toString(), afl::test::expectedIntNodeString);

    tokenNodeRef = tokenNode;
    intNodeRef = intNode;
    ASSERT_EQ(tokenNode.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intNode.toString(), afl::test::expectedIntNodeString);
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedIntNodeString);

    // self assignment
    tokenNodeRef = tokenNodeRef;
    intNodeRef = intNodeRef;
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedIntNodeString);
}

GTEST_TEST(Test_Node, move_constructor_assignment)
{
    // move constructor
    auto tokenNode = afl::test::createPointNode(42, 5, 6, 4);
    auto intNode = afl::test::createIntNode(42, -1, 1, 4);
    ASSERT_EQ(tokenNode.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intNode.toString(), afl::test::expectedIntNodeString);

    auto tokenNodeRef = afl::Node<afl::test::Point>(std::move(tokenNode));
    auto intNodeRef = afl::Node<int>(std::move(intNode));
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedIntNodeString);

    // move assignment
    tokenNode = afl::test::createPointNode(42, 5, 6, 4);
    intNode = afl::test::createIntNode(42, -1, 1, 4);
    ASSERT_EQ(tokenNode.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intNode.toString(), afl::test::expectedIntNodeString);

    tokenNodeRef = std::move(tokenNode);
    intNodeRef = std::move(intNode);
    ASSERT_EQ(tokenNodeRef.toString(), afl::test::expectedPointNodeString);
    ASSERT_EQ(intNodeRef.toString(), afl::test::expectedIntNodeString);
}

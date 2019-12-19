#include "gtest/gtest.h"

#include "../src/private/functionformatter.h"
#include "../src/private/resourcemanager.h"

GTEST_TEST(FunctionFormatter_Test, replaceAliases)
{
    auto resourceManager = std::make_shared<afl::detail::ResourceManager>();
    resourceManager->load("../extensions/tokens", afl::detail::ResourceType::Extension);
    afl::detail::FunctionFormatter formatter(resourceManager);
    ASSERT_EQ(formatter.formatFunction("absolute(3)+absolute(3)"), "abs(3)+abs(3)");
    ASSERT_EQ(formatter.formatFunction("4+2*absolute(3)+absolute(3)"), "4+2*abs(3)+abs(3)");
    ASSERT_EQ(formatter.formatFunction("absolute(3)+absolute(3)/3+4"), "abs(3)+abs(3)/3+4");
    ASSERT_EQ(formatter.formatFunction("4+2*absolute(3)+absolute(3)/3+4"), "4+2*abs(3)+abs(3)/3+4");
}

GTEST_TEST(FunctionFormatter_Test, formatWithPlugins_c_api)
{
    auto resourceManager = std::make_shared<afl::detail::ResourceManager>();
    resourceManager->load("../extensions/tokens", afl::detail::ResourceType::Extension);
    resourceManager->load("res/plugins/c_api/c_api_plugin", afl::detail::ResourceType::Plugin);
    afl::detail::FunctionFormatter formatter(resourceManager);
    ASSERT_EQ(formatter.formatWithPlugins("UPPERCASE_TOKEN+UPPERCASE_TOKEN"), "lowercase_token+lowercase_token");
    ASSERT_EQ(formatter.formatWithPlugins("4+2*UPPERCASE_TOKEN+UPPERCASE_TOKEN"), "4+2*lowercase_token+lowercase_token");
    ASSERT_EQ(formatter.formatWithPlugins("UPPERCASE_TOKEN+UPPERCASE_TOKEN/3+4"), "lowercase_token+lowercase_token/3+4");
    ASSERT_EQ(formatter.formatWithPlugins("4+2*UPPERCASE_TOKEN+UPPERCASE_TOKEN/3+4"), "4+2*lowercase_token+lowercase_token/3+4");
}

GTEST_TEST(FunctionFormatter_Test, formatWithPlugins_cpp_api)
{
    auto resourceManager = std::make_shared<afl::detail::ResourceManager>();
    resourceManager->load("../extensions/tokens", afl::detail::ResourceType::Extension);
    resourceManager->load("res/plugins/cpp_api/cpp_api_plugin", afl::detail::ResourceType::Plugin);
    afl::detail::FunctionFormatter formatter(resourceManager);
    ASSERT_EQ(formatter.formatWithPlugins("hello+hello"), "hi+hi");
    ASSERT_EQ(formatter.formatWithPlugins("4+2*hello+hello"), "4+2*hi+hi");
    ASSERT_EQ(formatter.formatWithPlugins("hello+hello/3+4"), "hi+hi/3+4");
    ASSERT_EQ(formatter.formatWithPlugins("4+2*hello+hello/3+4"), "4+2*hi+hi/3+4");
}

GTEST_TEST(FunctionFormatter_Test, formatFunction)
{
    auto resourceManager = std::make_shared<afl::detail::ResourceManager>();
    resourceManager->load("../extensions/tokens", afl::detail::ResourceType::Extension);
    resourceManager->load("res/plugins/c_api/c_api_plugin", afl::detail::ResourceType::Plugin);
    resourceManager->load("res/plugins/cpp_api/cpp_api_plugin", afl::detail::ResourceType::Plugin);
    afl::detail::FunctionFormatter formatter(resourceManager);
    // test replaceAliases
    ASSERT_EQ(formatter.formatFunction("absolute(3)+absolute(3)"), "abs(3)+abs(3)");
    ASSERT_EQ(formatter.formatFunction("4+2*absolute(3)+absolute(3)"), "4+2*abs(3)+abs(3)");
    ASSERT_EQ(formatter.formatFunction("absolute(3)+absolute(3)/3+4"), "abs(3)+abs(3)/3+4");
    ASSERT_EQ(formatter.formatFunction("4+2*absolute(3)+absolute(3)/3+4"), "4+2*abs(3)+abs(3)/3+4");
    // test formatWithPlugins
    ASSERT_EQ(formatter.formatFunction("UPPERCASE_TOKEN+UPPERCASE_TOKEN"), "lowercase_token+lowercase_token");
    ASSERT_EQ(formatter.formatFunction("4+2*UPPERCASE_TOKEN+hello"), "4+2*lowercase_token+hi");
    ASSERT_EQ(formatter.formatFunction("UPPERCASE_TOKEN+hello+UPPERCASE_TOKEN/3+4"), "lowercase_token+hi+lowercase_token/3+4");
    ASSERT_EQ(formatter.formatFunction("4+2*UPPERCASE_TOKEN+hello+UPPERCASE_TOKEN/3+4"), "4+2*lowercase_token+hi+lowercase_token/3+4");
    // test all
    ASSERT_EQ(formatter.formatFunction("UPPERCASE_TOKEN+UPPERCASE_TOKEN*absolute(3)+absolute(3)"), "lowercase_token+lowercase_token*abs(3)+abs(3)");
    ASSERT_EQ(formatter.formatFunction("4+2*UPPERCASE_TOKEN+UPPERCASE_TOKEN*absolute(3)+absolute(3)"), "4+2*lowercase_token+lowercase_token*abs(3)+abs(3)");
    ASSERT_EQ(formatter.formatFunction("UPPERCASE_TOKEN+UPPERCASE_TOKEN*absolute(3)+absolute(3)/3+4"), "lowercase_token+lowercase_token*abs(3)+abs(3)/3+4");
    ASSERT_EQ(formatter.formatFunction("4+2*UPPERCASE_TOKEN+UPPERCASE_TOKEN*absolute(3)+absolute(3)/3+4"), "4+2*lowercase_token+lowercase_token*abs(3)+abs(3)/3+4");
}

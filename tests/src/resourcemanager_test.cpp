#include "gtest/gtest.h"

#include "../src/private/resourcemanager.h"

GTEST_TEST(ResourceManager_Test, load_unload_plugin)
{
    afl::detail::ResourceManager manager;
    manager.load(afl::detail::ResourceType::Plugin, "res/plugins/first/first_plugin");
    ASSERT_EQ(manager.getPluginManager().getLoadedPluginCount(), 1);
    manager.unload(afl::detail::ResourceType::Plugin, manager.getPluginManager().getLoadedPlugins().front()->getPath());
    ASSERT_EQ(manager.getPluginManager().getLoadedPluginCount(), 0);
}
GTEST_TEST(ResourceManager_Test, loadDirectory_unload_plugin)
{
    afl::detail::ResourceManager manager;
    manager.loadDirectory(afl::detail::ResourceType::Plugin, "res/plugins/", false);
    ASSERT_EQ(manager.getPluginManager().getLoadedPluginCount(), 0);
    manager.loadDirectory(afl::detail::ResourceType::Plugin, "res/plugins/", true);
    ASSERT_EQ(manager.getPluginManager().getLoadedPluginCount(), 1);
    manager.unload(afl::detail::ResourceType::Plugin, manager.getPluginManager().getLoadedPlugins().front()->getPath());
    ASSERT_EQ(manager.getPluginManager().getLoadedPluginCount(), 0);
}

GTEST_TEST(ResourceManager_Test, load_extension)
{
    afl::detail::ResourceManager manager;
    manager.load(afl::detail::ResourceType::Extension, "res/extensions/test/tokenSample");
    ASSERT_EQ(manager.getTokens().size(), 6);
    manager.load(afl::detail::ResourceType::Extension, "res/extensions/test/tokenSample");
    ASSERT_EQ(manager.getTokens().size(), 6);
    manager.unload(afl::detail::ResourceType::Extension, "res/extensions/test/tokenSample");
    ASSERT_EQ(manager.getTokens().size(), 0);
}
GTEST_TEST(ResourceManager_Test, loadDirectory_extension)
{
    afl::detail::ResourceManager manager;
    manager.loadDirectory(afl::detail::ResourceType::Extension, "res/extensions", false);
    ASSERT_EQ(manager.getTokens().size(), 0);
    manager.loadDirectory(afl::detail::ResourceType::Extension, "res/extensions", true);
    ASSERT_EQ(manager.getTokens().size(), 6);
    manager.loadDirectory(afl::detail::ResourceType::Extension, "res/extensions", true);
    ASSERT_EQ(manager.getTokens().size(), 6);
    manager.unload(afl::detail::ResourceType::Extension, "res/extensions/test/tokenSample");
    ASSERT_EQ(manager.getTokens().size(), 0);
}

GTEST_TEST(ResourceManager_Test, getToken_disabled_createIfNotExist)
{
    afl::detail::ResourceManager manager;
    manager.load(afl::detail::ResourceType::Plugin, "res/plugins/first/first_plugin");
    manager.load(afl::detail::ResourceType::Extension, "res/extensions/test/tokenSample");
    ASSERT_EQ(manager.getTokens().size(), 6);
    ASSERT_NE(manager.getToken("*", false), nullptr);
    ASSERT_EQ(manager.getToken("*", false)->token->precedence, 2);
    ASSERT_EQ(manager.getToken("*", false)->token->associativity, afl::TokenAssociativity::Left);
    ASSERT_NE(manager.getToken("^", false), nullptr);
    ASSERT_EQ(manager.getToken("^", false)->token->parameterCount, 0);
    ASSERT_EQ(manager.getToken("^", false)->token->associativity, afl::TokenAssociativity::Right);
    ASSERT_NE(manager.getToken("abs", false), nullptr);
    ASSERT_EQ(manager.getToken("abs", false)->token->parameterCount, 1);
    ASSERT_EQ(manager.getToken("abs", false)->token->associativity, afl::TokenAssociativity::None);
    ASSERT_EQ(manager.getTokens().size(), 6);
    ASSERT_EQ(manager.getToken("1", false), nullptr);
    ASSERT_EQ(manager.getTokens().size(), 6);
    manager.unload(afl::detail::ResourceType::Plugin, "res/plugins/first/first_plugin");
    ASSERT_EQ(manager.getTokens().size(), 6);
    manager.unload(afl::detail::ResourceType::Extension, "res/extensions/test/tokenSample");
    ASSERT_EQ(manager.getTokens().size(), 0);
}
GTEST_TEST(ResourceManager_Test, getToken_enabled_createIfNotExist)
{
    afl::detail::ResourceManager manager;
    manager.load(afl::detail::ResourceType::Plugin, "res/plugins/first/first_plugin");
    ASSERT_EQ(manager.getTokens().size(), 0);
    manager.load(afl::detail::ResourceType::Extension, "res/extensions/test/tokenSample");
    ASSERT_EQ(manager.getTokens().size(), 6);
    ASSERT_NE(manager.getToken("*", true), nullptr);
    ASSERT_EQ(manager.getToken("*", true)->token->precedence, 2);
    ASSERT_EQ(manager.getToken("*", true)->token->associativity, afl::TokenAssociativity::Left);
    ASSERT_NE(manager.getToken("^", true), nullptr);
    ASSERT_EQ(manager.getToken("^", true)->token->parameterCount, 0);
    ASSERT_EQ(manager.getToken("^", true)->token->associativity, afl::TokenAssociativity::Right);
    ASSERT_NE(manager.getToken("abs", true), nullptr);
    ASSERT_EQ(manager.getToken("abs", true)->token->parameterCount, 1);
    ASSERT_EQ(manager.getToken("abs", true)->token->associativity, afl::TokenAssociativity::None);
    ASSERT_EQ(manager.getTokens().size(), 6);
    ASSERT_NE(manager.getToken("1", true), nullptr);
    ASSERT_EQ(manager.getToken("1", true)->token->type, afl::TokenType::Number);
    ASSERT_EQ(manager.getToken("1", true)->aliases.size(), 1);
    ASSERT_EQ(manager.getToken("1", true)->aliases.front().aliases, std::set<std::string>{"one"});
    ASSERT_EQ(manager.getTokens().size(), 7);
    manager.unload(afl::detail::ResourceType::Plugin, "res/plugins/first/first_plugin");
    ASSERT_EQ(manager.getTokens().size(), 6);
    manager.unload(afl::detail::ResourceType::Extension, "res/extensions/test/tokenSample");
    ASSERT_EQ(manager.getTokens().size(), 0);
}

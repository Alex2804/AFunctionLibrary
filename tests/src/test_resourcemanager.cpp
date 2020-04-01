#include "gtest/gtest.h"

#include "../src/private/resourcemanager.h"

GTEST_TEST(Test_ResourceManager, load_unload)
{
    // plugins
    afl::detail::ResourceManager manager;
    ASSERT_TRUE(manager.getTokenManager()->m_pluginFunctions.empty());
    manager.load("res/plugins/c_api/c_api_plugin", afl::detail::ResourceType::Plugin);
    ASSERT_EQ(manager.getPluginManager()->getLoadedPluginCount(), 1);
    ASSERT_FALSE(manager.getTokenManager()->m_pluginFunctions.empty());
    manager.unload(manager.getPluginManager()->getLoadedPlugins().front()->getPath(), afl::detail::ResourceType::Plugin);
    ASSERT_EQ(manager.getPluginManager()->getLoadedPluginCount(), 0);
    ASSERT_TRUE(manager.getTokenManager()->m_pluginFunctions.empty());

    // extensions
    manager = afl::detail::ResourceManager();
    manager.load("res/extensions/test/tokenSample", afl::detail::ResourceType::Extension);
    ASSERT_EQ(manager.getTokenManager()->getTokens().size(), 6);
    manager.load("res/extensions/test/tokenSample", afl::detail::ResourceType::Extension);
    ASSERT_EQ(manager.getTokenManager()->getTokens().size(), 6);
    manager.unload("res/extensions/test/tokenSample", afl::detail::ResourceType::Extension);
    ASSERT_EQ(manager.getTokenManager()->getTokens().size(), 0);
}
GTEST_TEST(Test_ResourceManager, loadDirectory_unload)
{
    // plugins
    afl::detail::ResourceManager manager;
    ASSERT_TRUE(manager.getTokenManager()->m_pluginFunctions.empty());
    manager.loadDirectory("res/plugins/", afl::detail::ResourceType::Plugin, false);
    ASSERT_EQ(manager.getPluginManager()->getLoadedPluginCount(), 0);
    ASSERT_TRUE(manager.getTokenManager()->m_pluginFunctions.empty());
    manager.loadDirectory("res/plugins/", afl::detail::ResourceType::Plugin, true);
    ASSERT_EQ(manager.getPluginManager()->getLoadedPluginCount(), 2);
    ASSERT_FALSE(manager.getTokenManager()->m_pluginFunctions.empty());
    manager.unload(manager.getPluginManager()->getLoadedPlugins().front()->getPath(), afl::detail::ResourceType::Plugin);
    ASSERT_EQ(manager.getPluginManager()->getLoadedPluginCount(), 1);
    manager.unload(manager.getPluginManager()->getLoadedPlugins().front()->getPath(), afl::detail::ResourceType::Plugin);
    ASSERT_EQ(manager.getPluginManager()->getLoadedPluginCount(), 0);
    ASSERT_TRUE(manager.getTokenManager()->m_pluginFunctions.empty());

    // extensions
    manager = afl::detail::ResourceManager();
    manager.loadDirectory("res/extensions", afl::detail::ResourceType::Extension, false);
    ASSERT_EQ(manager.getTokenManager()->getTokens().size(), 0);
    manager.loadDirectory("res/extensions", afl::detail::ResourceType::Extension, true);
    ASSERT_EQ(manager.getTokenManager()->getTokens().size(), 6);
    manager.loadDirectory("res/extensions", afl::detail::ResourceType::Extension, true);
    ASSERT_EQ(manager.getTokenManager()->getTokens().size(), 6);
    manager.unload("res/extensions/test/tokenSample", afl::detail::ResourceType::Extension);
    ASSERT_EQ(manager.getTokenManager()->getTokens().size(), 0);
}
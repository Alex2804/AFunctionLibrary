#include "gtest/gtest.h"

#include <memory>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cstring>
#include <tuple>

#include "../../src/private/tokenmanager.h"
#include "../../src/private/resourcemanager.h"

std::unordered_map<std::string, std::shared_ptr<afl::detail::TokenWrapper<std::string>>> allTokens = {
        {"+", std::make_shared<afl::detail::TokenWrapper<std::string>>(std::make_shared<afl::Token<std::string>>("+", afl::TokenType::Operator, 1, 0, afl::TokenAssociativity::Left), std::vector<afl::TokenAliases<std::string>>{})},
        {"*", std::make_shared<afl::detail::TokenWrapper<std::string>>(std::make_shared<afl::Token<std::string>>("*", afl::TokenType::Operator, 2, 0, afl::TokenAssociativity::Left), std::vector<afl::TokenAliases<std::string>>{})},
        {"^", std::make_shared<afl::detail::TokenWrapper<std::string>>(std::make_shared<afl::Token<std::string>>("^", afl::TokenType::Operator, 2, 0, afl::TokenAssociativity::Right), std::vector<afl::TokenAliases<std::string>>{})},
        {"abs", std::make_shared<afl::detail::TokenWrapper<std::string>>(std::make_shared<afl::Token<std::string>>("abs", afl::TokenType::Function, std::numeric_limits<size_t>::max(), 1, afl::TokenAssociativity::None),
                                                                         std::vector<afl::TokenAliases<std::string>>{afl::TokenAliases<std::string>{afl::TokenAliasType::String, {"absolute", "absolute2"}}, afl::TokenAliases<std::string>{afl::TokenAliasType::Regex, {".*absolute.*"}}})},
};

GTEST_TEST(TokenManager_Test, copy_constructor_assignment)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>> tokens =
            {
                    {"+", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("+"), 23}},
                    {"*", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("*"), 46}},
                    {"^", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("^"), 92}},
                    {"abs", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("abs"), 184}}
            };
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs =
            {
                    {"path1", {"v1", "v2"}},
                    {"path2", {"v3", "v4"}}
            };
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::createTokenPluginFunction>, std::vector<afl::detail::createTokenAliasesPluginFunction>>> pluginFunctions =
            {
                    {nullptr, {nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr}},
                    {nullptr, {}, {}}
            };

    afl::detail::TokenManager manager1;
    manager1.m_tokens = tokens;
    manager1.m_pathTokenValueRefs = pathTokenValueRefs;
    manager1.m_pluginFunctions = pluginFunctions;

    // copy constructor
    afl::detail::TokenManager copiedManager(manager1);
    ASSERT_EQ(copiedManager.m_tokens, tokens);
    ASSERT_EQ(copiedManager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(copiedManager.m_pluginFunctions, pluginFunctions);
    copiedManager.m_tokens.clear();
    copiedManager.m_pathTokenValueRefs.clear();
    copiedManager.m_pluginFunctions.clear();

    // copy assignment
    copiedManager = manager1;
    ASSERT_EQ(copiedManager.m_tokens, tokens);
    ASSERT_EQ(copiedManager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(copiedManager.m_pluginFunctions, pluginFunctions);
    copiedManager.m_tokens.clear();
    copiedManager.m_pathTokenValueRefs.clear();
    copiedManager.m_pluginFunctions.clear();

    ASSERT_EQ(manager1.m_tokens, tokens);
    ASSERT_EQ(manager1.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager1.m_pluginFunctions, pluginFunctions);
}

GTEST_TEST(TokenManager_Test, move_constructor_assignment)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>> tokens =
            {
                    {"+", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("+"), 23}},
                    {"*", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("*"), 46}},
                    {"^", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("^"), 92}},
                    {"abs", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("abs"), 184}}
            };
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs =
            {
                    {"path1", {"v1", "v2"}},
                    {"path2", {"v3", "v4"}}
            };
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::createTokenPluginFunction>, std::vector<afl::detail::createTokenAliasesPluginFunction>>> pluginFunctions =
            {
                    {nullptr, {nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr}},
                    {nullptr, {}, {}}
            };

    afl::detail::TokenManager manager1;
    manager1.m_tokens = tokens;
    manager1.m_pathTokenValueRefs = pathTokenValueRefs;
    manager1.m_pluginFunctions = pluginFunctions;

    // move constructor
    afl::detail::TokenManager movedManager(std::move(manager1));
    ASSERT_EQ(movedManager.m_tokens, tokens);
    ASSERT_EQ(movedManager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(movedManager.m_pluginFunctions, pluginFunctions);

    afl::detail::TokenManager manager2;
    manager2.m_tokens = tokens;
    manager2.m_pathTokenValueRefs = pathTokenValueRefs;
    manager2.m_pluginFunctions = pluginFunctions;

    // move assignment
    movedManager = std::move(manager2);
    ASSERT_EQ(movedManager.m_tokens, tokens);
    ASSERT_EQ(movedManager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(movedManager.m_pluginFunctions, pluginFunctions);
}

GTEST_TEST(TokenManager_Test, addPluginFeatures)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>> tokens =
            {
                    {"+", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("+"), 2}},
                    {"*", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("*"), 2}},
                    {"^", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("^"), 1}},
                    {"abs", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("abs"), 1}}
            };
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs =
            {
                    {"path/to/operators", {"+", "*", "^"}},
                    {"path/to/alternative_operators", {"+", "*"}},
                    {"path/to/functions", {"abs"}}
            };
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::createTokenPluginFunction>, std::vector<afl::detail::createTokenAliasesPluginFunction>>> pluginFunctions;

    afl::detail::TokenManager manager;
    manager.m_tokens = tokens;
    manager.m_pathTokenValueRefs = pathTokenValueRefs;

    manager.addPluginFeatures(nullptr);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    apl::Plugin* plugin = apl::Plugin::load("res/plugins/first/first_plugin");
    ASSERT_NE(plugin, nullptr);
    plugin->unload();
    ASSERT_FALSE(plugin->isLoaded());
    manager.addPluginFeatures(plugin);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    plugin = apl::Plugin::load("res/plugins/first/first_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    manager.addPluginFeatures(plugin);
    ASSERT_EQ(manager.m_pluginFunctions.size(), 1);
    std::vector<afl::detail::createTokenPluginFunction> createTokenPluginFunctions;
    std::vector<afl::detail::createTokenAliasesPluginFunction> createTokenAliasesPluginFunctions;
    const apl::PluginFeatureInfo* const* featureInfos = plugin->getFeatureInfos();
    const apl::PluginFeatureInfo* featureInfo;
    for(size_t i = 0; i < plugin->getFeatureCount(); i++) {
        featureInfo = featureInfos[i];
        if(std::strcmp(featureInfo->featureGroup, "afl_create_token") == 0)
            createTokenPluginFunctions.push_back(reinterpret_cast<afl::detail::createTokenPluginFunction>(featureInfo->functionPointer));
        else if(std::strcmp(featureInfo->featureGroup, "afl_create_token_aliases") == 0)
            createTokenAliasesPluginFunctions.push_back(reinterpret_cast<afl::detail::createTokenAliasesPluginFunction>(featureInfo->functionPointer));
    }
    std::sort(createTokenPluginFunctions.begin(), createTokenPluginFunctions.end());
    std::sort(createTokenAliasesPluginFunctions.begin(), createTokenAliasesPluginFunctions.end());
    pluginFunctions.emplace_back(plugin, createTokenPluginFunctions, createTokenAliasesPluginFunctions);
    std::sort(std::get<1>(manager.m_pluginFunctions.front()).begin(), std::get<1>(manager.m_pluginFunctions.front()).end());
    std::sort(std::get<2>(manager.m_pluginFunctions.front()).begin(), std::get<2>(manager.m_pluginFunctions.front()).end());
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    ASSERT_EQ(manager.m_tokens, tokens);
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
}

GTEST_TEST(TokenManager_Test, removePluginFeatures)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>> tokens =
            {
                    {"+", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("+"), 2}},
                    {"*", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("*"), 2}},
                    {"^", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("^"), 1}},
                    {"abs", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("abs"), 1}}
            };
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs =
            {
                    {afl::detail::getFullPathName("res/plugins/first/first_plugin", afl::detail::ResourceType::Plugin), {"+", "*", "^"}},
                    {"path/to/alternative_operators", {"+", "*"}},
                    {"path/to/functions", {"abs"}}
            };
    apl::Plugin* plugin = apl::Plugin::load("res/plugins/first/first_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    std::vector<afl::detail::createTokenPluginFunction> createTokenPluginFunctions;
    std::vector<afl::detail::createTokenAliasesPluginFunction> createTokenAliasesPluginFunctions;
    const apl::PluginFeatureInfo* const* featureInfos = plugin->getFeatureInfos();
    const apl::PluginFeatureInfo* featureInfo;
    for(size_t i = 0; i < plugin->getFeatureCount(); i++) {
        featureInfo = featureInfos[i];
        if(std::strcmp(featureInfo->featureGroup, "afl_create_token") == 0)
            createTokenPluginFunctions.push_back(reinterpret_cast<afl::detail::createTokenPluginFunction>(featureInfo->functionPointer));
        else if(std::strcmp(featureInfo->featureGroup, "afl_create_token_aliases") == 0)
            createTokenAliasesPluginFunctions.push_back(reinterpret_cast<afl::detail::createTokenAliasesPluginFunction>(featureInfo->functionPointer));
    }
    std::sort(createTokenPluginFunctions.begin(), createTokenPluginFunctions.end());
    std::sort(createTokenAliasesPluginFunctions.begin(), createTokenAliasesPluginFunctions.end());
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::createTokenPluginFunction>, std::vector<afl::detail::createTokenAliasesPluginFunction>>> pluginFunctions =
            {{plugin, createTokenPluginFunctions, createTokenAliasesPluginFunctions}};

    afl::detail::TokenManager manager;
    manager.m_tokens = tokens;
    manager.m_pathTokenValueRefs = pathTokenValueRefs;
    manager.m_pluginFunctions = pluginFunctions;

    manager.removePluginFeatures(nullptr);
    ASSERT_EQ(manager.m_tokens, tokens);
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removePluginFeatures(plugin);
    tokens.erase("^");
    tokens.at("+").second = 1;
    tokens.at("*").second = 1;
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs.erase(afl::detail::getFullPathName("res/plugins/first/first_plugin", afl::detail::ResourceType::Plugin));
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    pluginFunctions.clear();
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);
}

GTEST_TEST(TokenManager_Test, removeReferences)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>> tokens =
            {
                    {"+", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("+"), 2}},
                    {"*", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("*"), 2}},
                    {"^", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("^"), 1}},
                    {"abs", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("abs"), 1}}
            };
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs =
            {
                    {"path/to/operators", {"+", "*", "^"}},
                    {"path/to/alternative_operators", {"+", "*"}},
                    {"path/to/functions", {"abs"}}
            };
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::createTokenPluginFunction>, std::vector<afl::detail::createTokenAliasesPluginFunction>>> pluginFunctions =
            {{nullptr, {nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr}}};

    afl::detail::TokenManager manager;
    manager.m_tokens = tokens;
    manager.m_pathTokenValueRefs = pathTokenValueRefs;
    manager.m_pluginFunctions = pluginFunctions;

    manager.removeReferences("path/to/operators");
    tokens.erase("^");
    tokens.at("+").second = 1;
    tokens.at("*").second = 1;
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs.erase("path/to/operators");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeReferences("path/to/alternative_operators");
    tokens.erase("+");
    tokens.erase("*");
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs.erase("path/to/alternative_operators");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeReferences("path/to/functions");
    tokens.erase("abs");
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs.erase("path/to/functions");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    ASSERT_TRUE(manager.m_tokens.empty());
    ASSERT_TRUE(manager.m_pathTokenValueRefs.empty());
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);
}

GTEST_TEST(TokenManager_Test, addToken)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>> tokens;
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs;
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::createTokenPluginFunction>, std::vector<afl::detail::createTokenAliasesPluginFunction>>> pluginFunctions;

    afl::detail::TokenManager manager;
    manager.addToken(allTokens.at("+"), "path/to/operators");
    tokens = {{"+", {allTokens.at("+"), 1}}};
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs = {{"path/to/operators", {"+"}}};
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.addToken(allTokens.at("abs"), "path/to/functions");
    tokens.emplace("abs", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("abs"), 1});
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs.emplace("path/to/functions", std::vector<std::string>{"abs"});
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.addToken(allTokens.at("*"), "path/to/operators");
    tokens.emplace("*", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("*"), 1});
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs.at("path/to/operators").push_back("*");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.addToken(allTokens.at("+"), "path/to/operators");
    ASSERT_EQ(manager.m_tokens, tokens);
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.addToken(allTokens.at("*"), "path/to/alternative_operators");
    tokens.at("*").second = 2;
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs.emplace("path/to/alternative_operators", std::vector<std::string>{"*"});
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);
}

GTEST_TEST(TokenManager_Test, removeToken)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>> tokens =
            {
                    {"+", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("+"), 2}},
                    {"*", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("*"), 2}},
                    {"^", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("^"), 1}},
                    {"abs", std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, size_t>{allTokens.at("abs"), 1}}
            };
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs =
            {
                    {"path/to/operators", {"+", "*", "^"}},
                    {"path/to/alternative_operators", {"+", "*"}},
                    {"path/to/functions", {"abs"}}
            };
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::createTokenPluginFunction>, std::vector<afl::detail::createTokenAliasesPluginFunction>>> pluginFunctions =
            {{nullptr, {nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr}}};

    afl::detail::TokenManager manager;
    manager.m_tokens = tokens;
    manager.m_pathTokenValueRefs = pathTokenValueRefs;
    manager.m_pluginFunctions = pluginFunctions;

    manager.removeToken("*", "path/to/operators");
    tokens.at("*").second = 1;
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs.at("path/to/operators") = {"+", "^"};
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken(allTokens.at("*"), "path/to/operators");
    ASSERT_EQ(manager.m_tokens, tokens);
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken("*", "path/to/alternative_operators");
    tokens.erase("*");
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs.at("path/to/alternative_operators") = {"+"};
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken("abs", "pat/to/not/to/remove/functions");
    ASSERT_EQ(manager.m_tokens, tokens);
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken(allTokens.at("abs"), "path/to/functions");
    tokens.erase("abs");
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs.erase("path/to/functions");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken("+", "path/to/alternative_operators");
    tokens.at("+").second = 1;
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs.erase("path/to/alternative_operators");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken("some_operator", "path/to/operators");
    ASSERT_EQ(manager.m_tokens, tokens);
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken("^", "path/to/operators");
    tokens.erase("^");
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs.at("path/to/operators") = {"+"};
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken(allTokens.at("+"), "path/to/operators");
    tokens.erase("+");
    ASSERT_EQ(manager.m_tokens, tokens);
    pathTokenValueRefs.erase("path/to/operators");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    ASSERT_TRUE(manager.m_tokens.empty());
    ASSERT_TRUE(manager.m_pathTokenValueRefs.empty());
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);
}

GTEST_TEST(TokenManager_Test, createToken)
{
    afl::detail::TokenManager manager;
    apl::Plugin* plugin = apl::Plugin::load("res/plugins/first/first_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    manager.addPluginFeatures(plugin);

    std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, std::string> created = manager.createToken("1", false);
    std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, std::string> expected = {std::make_shared<afl::detail::TokenWrapper<std::string>>(std::make_shared<afl::Token<std::string>>("1", afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None), std::vector<afl::TokenAliases<std::string>>{}), afl::detail::getFullPathName("res/plugins/first/first_plugin", afl::detail::ResourceType::Plugin)};
    ASSERT_EQ(*created.first, *expected.first);
    ASSERT_EQ(created.second, expected.second);

    created = manager.createToken("1", true);
    std::pair<std::shared_ptr<afl::detail::TokenWrapper<std::string>>, std::string> created2 = manager.createToken("1");
    ASSERT_EQ(*created.first, *created2.first);
    ASSERT_EQ(created.second, created2.second);

    expected = {std::make_shared<afl::detail::TokenWrapper<std::string>>(std::make_shared<afl::Token<std::string>>("1", afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None),
                std::vector<afl::TokenAliases<std::string>>{afl::TokenAliases<std::string>{afl::TokenAliasType::String, {"one"}}}), afl::detail::getFullPathName("res/plugins/first/first_plugin", afl::detail::ResourceType::Plugin)};
    ASSERT_EQ(*created.first, *expected.first);
    ASSERT_EQ(created.second, expected.second);

    created = manager.createToken("12", true);
    ASSERT_EQ(created.first.get(), nullptr);
    ASSERT_EQ(created.second, "");
}

GTEST_TEST(TokenManager_Test, createAliases)
{
    afl::detail::TokenManager manager;
    apl::Plugin* plugin = apl::Plugin::load("res/plugins/first/first_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    manager.addPluginFeatures(plugin);

    std::vector<afl::TokenAliases<std::string>> aliases = manager.createAliases("1");
    std::vector<afl::TokenAliases<std::string>> expected = {{afl::TokenAliasType::String, {"one"}}};
    ASSERT_EQ(aliases, expected);

    aliases = manager.createAliases("12");
    ASSERT_TRUE(aliases.empty());
}

GTEST_TEST(TokenManager_Test, getToken)
{
    afl::detail::TokenManager manager;
    manager.addToken(allTokens.at("+"), "path/to/operators");
    manager.addToken(allTokens.at("*"), "path/to/operators");
    manager.addToken(allTokens.at("^"), "path/to/operators");
    apl::Plugin* plugin = apl::Plugin::load("res/plugins/first/first_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    manager.addPluginFeatures(plugin);

    ASSERT_EQ(manager.getToken("+"), allTokens.at("+"));
    ASSERT_EQ(manager.getToken("*"), allTokens.at("*"));
    ASSERT_EQ(manager.getToken("^"), allTokens.at("^"));
    ASSERT_EQ(manager.getToken("1").get(), nullptr);
    ASSERT_EQ(manager.getToken("no_token").get(), nullptr);

    ASSERT_EQ(manager.getToken("+", false), allTokens.at("+"));
    ASSERT_EQ(manager.getToken("*", false), allTokens.at("*"));
    ASSERT_EQ(manager.getToken("^", false), allTokens.at("^"));
    ASSERT_EQ(manager.getToken("1", false).get(), nullptr);
    ASSERT_EQ(manager.getToken("no_token", false).get(), nullptr);

    ASSERT_EQ(manager.getToken("+", true), allTokens.at("+"));
    ASSERT_EQ(manager.getToken("*", true), allTokens.at("*"));
    ASSERT_EQ(manager.getToken("^", true), allTokens.at("^"));
    afl::detail::TokenWrapper<std::string> wrapper(std::make_shared<afl::Token<std::string>>("1", afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None),
                                                   std::vector<afl::TokenAliases<std::string>>{afl::TokenAliases<std::string>{afl::TokenAliasType::String, std::set<std::string>{"one"}}});
    ASSERT_EQ(*manager.getToken("1", true), wrapper);
    ASSERT_EQ(manager.getToken("no_token", true).get(), nullptr);
}

GTEST_TEST(TokenManager_Test, getTokens)
{
    afl::detail::TokenManager manager;
    manager.addToken(allTokens.at("+"), "path/to/operators");
    manager.addToken(allTokens.at("*"), "path/to/operators");
    manager.addToken(allTokens.at("^"), "path/to/operators");
    manager.addToken(allTokens.at("abs"), "path/to/functions");

    std::vector<std::shared_ptr<const afl::detail::TokenWrapper<std::string>>> expected =
            {allTokens.at("+"), allTokens.at("*"), allTokens.at("^"), allTokens.at("abs") };
    std::sort(expected.begin(), expected.end());
    std::vector<std::shared_ptr<const afl::detail::TokenWrapper<std::string>>> tokens = manager.getTokens();
    std::sort(tokens.begin(), tokens.end());
    ASSERT_EQ(expected, tokens);

    expected = {allTokens.at("+"), allTokens.at("*"), allTokens.at("^") };
    std::sort(expected.begin(), expected.end());
    tokens = manager.getTokens(afl::TokenType::Operator);
    std::sort(tokens.begin(), tokens.end());
    ASSERT_EQ(expected, tokens);

    expected = {allTokens.at("abs") };
    tokens = manager.getTokens(afl::TokenType::Function);
    ASSERT_EQ(expected, tokens);
}

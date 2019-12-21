#include "gtest/gtest.h"

#include "test_include.h"

#include "../../src/private/tokenmanager.h"
#include "../../src/private/resourcemanager.h"
#include "AFunctionLibrary/implementation/createtokenapi/createtokenapi_definitions.hpp"

std::unordered_map<std::string, std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>> allTokens = {
        {"+", std::make_shared<afl::detail::TokenPtrBundle<std::string>>(std::make_shared<afl::Token<std::string>>("+", afl::TokenType::Operator, 1, 0, afl::TokenAssociativity::Left), std::vector<afl::TokenAliases<std::string>>{})},
        {"*", std::make_shared<afl::detail::TokenPtrBundle<std::string>>(std::make_shared<afl::Token<std::string>>("*", afl::TokenType::Operator, 2, 0, afl::TokenAssociativity::Left), std::vector<afl::TokenAliases<std::string>>{})},
        {"^", std::make_shared<afl::detail::TokenPtrBundle<std::string>>(std::make_shared<afl::Token<std::string>>("^", afl::TokenType::Operator, 2, 0, afl::TokenAssociativity::Right), std::vector<afl::TokenAliases<std::string>>{})},
        {"abs", std::make_shared<afl::detail::TokenPtrBundle<std::string>>(std::make_shared<afl::Token<std::string>>("abs", afl::TokenType::Function, std::numeric_limits<size_t>::max(), 1, afl::TokenAssociativity::None),
                                                                           std::vector<afl::TokenAliases<std::string>>{afl::TokenAliases<std::string>{afl::TokenAliasType::String, {"absolute", "absolute2"}}, afl::TokenAliases<std::string>{afl::TokenAliasType::Regex, {".*absolute.*"}}})},
};

GTEST_TEST(TokenManager_Test, copy_constructor_assignment)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>> uniqueTokens =
            {
                    {"+", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("+"), 23}},
                    {"*", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("*"), 46}},
                    {"^", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("^"), 92}}
            };
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>> notUniqueTokens =
            {
                    {"abs", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("abs"), 184}}
            };
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs =
            {
                    {"path1", {"v1", "v2"}},
                    {"path2", {"v3", "v4"}}
            };
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::cApiCreateTokenPluginFunction>, std::vector<afl::detail::cApiCreateTokenAliasesPluginFunction>, std::vector<afl::detail::cppApiCreateTokenPluginFunction>, std::vector<afl::detail::cppApiCreateTokenAliasesPluginFunction>>> pluginFunctions =
            {
                    {nullptr, {nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr}, {nullptr}, {nullptr, nullptr}},
                    {nullptr, {}, {}, {}, {}}
            };

    afl::detail::TokenManager manager1;
    manager1.m_uniqueTokens = uniqueTokens;
    manager1.m_notUniqueTokens = notUniqueTokens;
    manager1.m_pathTokenValueRefs = pathTokenValueRefs;
    manager1.m_pluginFunctions = pluginFunctions;

    // copy constructor
    afl::detail::TokenManager copiedManager(manager1);
    ASSERT_EQ(copiedManager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(copiedManager.m_notUniqueTokens, notUniqueTokens);
    ASSERT_EQ(copiedManager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(copiedManager.m_pluginFunctions, pluginFunctions);
    copiedManager.m_uniqueTokens.clear();
    copiedManager.m_notUniqueTokens.clear();
    copiedManager.m_pathTokenValueRefs.clear();
    copiedManager.m_pluginFunctions.clear();

    // copy assignment
    copiedManager = manager1;
    ASSERT_EQ(copiedManager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(copiedManager.m_notUniqueTokens, notUniqueTokens);
    ASSERT_EQ(copiedManager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(copiedManager.m_pluginFunctions, pluginFunctions);
    copiedManager.m_uniqueTokens.clear();
    copiedManager.m_notUniqueTokens.clear();
    copiedManager.m_pathTokenValueRefs.clear();
    copiedManager.m_pluginFunctions.clear();

    ASSERT_EQ(manager1.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager1.m_notUniqueTokens, notUniqueTokens);
    ASSERT_EQ(manager1.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager1.m_pluginFunctions, pluginFunctions);
}

GTEST_TEST(TokenManager_Test, move_constructor_assignment)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>> uniqueTokens =
            {
                    {"+", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("+"), 23}},
                    {"*", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("*"), 46}},
                    {"^", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("^"), 92}}
            };
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>> notUniqueTokens =
            {
                    {"abs", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("abs"), 184}}
            };
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs =
            {
                    {"path1", {"v1", "v2"}},
                    {"path2", {"v3", "v4"}}
            };
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::cApiCreateTokenPluginFunction>, std::vector<afl::detail::cApiCreateTokenAliasesPluginFunction>, std::vector<afl::detail::cppApiCreateTokenPluginFunction>, std::vector<afl::detail::cppApiCreateTokenAliasesPluginFunction>>> pluginFunctions =
            {
                    {nullptr, {nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr}, {nullptr}, {nullptr, nullptr}},
                    {nullptr, {}, {}, {}, {}}
            };

    afl::detail::TokenManager manager1;
    manager1.m_uniqueTokens = uniqueTokens;
    manager1.m_notUniqueTokens = notUniqueTokens;
    manager1.m_pathTokenValueRefs = pathTokenValueRefs;
    manager1.m_pluginFunctions = pluginFunctions;

    // move constructor
    afl::detail::TokenManager movedManager(std::move(manager1));
    ASSERT_EQ(movedManager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(movedManager.m_notUniqueTokens, notUniqueTokens);
    ASSERT_EQ(movedManager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(movedManager.m_pluginFunctions, pluginFunctions);

    afl::detail::TokenManager manager2;
    manager2.m_uniqueTokens = uniqueTokens;
    manager2.m_notUniqueTokens = notUniqueTokens;
    manager2.m_pathTokenValueRefs = pathTokenValueRefs;
    manager2.m_pluginFunctions = pluginFunctions;

    // move assignment
    movedManager = std::move(manager2);
    ASSERT_EQ(movedManager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(movedManager.m_notUniqueTokens, notUniqueTokens);
    ASSERT_EQ(movedManager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(movedManager.m_pluginFunctions, pluginFunctions);
}

GTEST_TEST(TokenManager_Test, addPluginFeatures)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>> uniqueTokens =
            {
                    {"+", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("+"), 2}},
                    {"*", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("*"), 2}},
                    {"^", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("^"), 1}}
            };
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>> notUniqueTokens =
            {
                    {"abs", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("abs"), 1}}
            };
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs =
            {
                    {"path/to/operators", {"+", "*", "^"}},
                    {"path/to/alternative_operators", {"+", "*"}},
                    {"path/to/functions", {"abs"}}
            };
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::cApiCreateTokenPluginFunction>, std::vector<afl::detail::cApiCreateTokenAliasesPluginFunction>, std::vector<afl::detail::cppApiCreateTokenPluginFunction>, std::vector<afl::detail::cppApiCreateTokenAliasesPluginFunction>>> pluginFunctions;

    afl::detail::TokenManager manager;
    manager.m_uniqueTokens = uniqueTokens;
    manager.m_notUniqueTokens = notUniqueTokens;
    manager.m_pathTokenValueRefs = pathTokenValueRefs;

    manager.addPluginFeatures(nullptr);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    apl::Plugin* plugin = apl::Plugin::load("res/plugins/c_api/c_api_plugin");
    ASSERT_NE(plugin, nullptr);
    plugin->unload();
    ASSERT_FALSE(plugin->isLoaded());
    manager.addPluginFeatures(plugin);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);
    delete plugin;

    plugin = apl::Plugin::load("res/plugins/c_api/c_api_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    manager.addPluginFeatures(plugin);
    ASSERT_EQ(manager.m_pluginFunctions.size(), 1);
    std::vector<afl::detail::cApiCreateTokenPluginFunction> cApiCreateTokenPluginFunctions;
    std::vector<afl::detail::cppApiCreateTokenPluginFunction> cppApiCreateTokenPluginFunctions;
    std::vector<afl::detail::cApiCreateTokenAliasesPluginFunction> cApiCreateTokenAliasesPluginFunctions;
    std::vector<afl::detail::cppApiCreateTokenAliasesPluginFunction> cppApiCreateTokenAliasesPluginFunctions;
    const apl::PluginFeatureInfo* const* featureInfos = plugin->getFeatureInfos();
    const apl::PluginFeatureInfo* featureInfo;
    for(size_t i = 0; i < plugin->getFeatureCount(); i++) {
        featureInfo = featureInfos[i];
        if(std::strcmp(featureInfo->featureGroup, afl::k_C_API_CreateTokenFeatureGroupName) == 0)
            cApiCreateTokenPluginFunctions.push_back(reinterpret_cast<afl::detail::cApiCreateTokenPluginFunction>(featureInfo->functionPointer));
        else if(std::strcmp(featureInfo->featureGroup, afl::k_CPP_API_CreateTokenFeatureGroupName) == 0)
            cppApiCreateTokenPluginFunctions.push_back(reinterpret_cast<afl::detail::cppApiCreateTokenPluginFunction>(featureInfo->functionPointer));
        else if(std::strcmp(featureInfo->featureGroup, afl::k_C_API_CreateTokenAliasesFeatureGroupName) == 0)
            cApiCreateTokenAliasesPluginFunctions.push_back(reinterpret_cast<afl::detail::cApiCreateTokenAliasesPluginFunction>(featureInfo->functionPointer));
        else if(std::strcmp(featureInfo->featureGroup, afl::k_CPP_API_CreateTokenAliasesFeatureGroupName) == 0)
            cppApiCreateTokenAliasesPluginFunctions.push_back(reinterpret_cast<afl::detail::cppApiCreateTokenAliasesPluginFunction>(featureInfo->functionPointer));
    }
    std::sort(cApiCreateTokenPluginFunctions.begin(), cApiCreateTokenPluginFunctions.end());
    std::sort(cApiCreateTokenAliasesPluginFunctions.begin(), cApiCreateTokenAliasesPluginFunctions.end());
    pluginFunctions.emplace_back(plugin, cApiCreateTokenPluginFunctions, cApiCreateTokenAliasesPluginFunctions, cppApiCreateTokenPluginFunctions, cppApiCreateTokenAliasesPluginFunctions);
    std::sort(std::get<1>(manager.m_pluginFunctions.front()).begin(), std::get<1>(manager.m_pluginFunctions.front()).end());
    std::sort(std::get<2>(manager.m_pluginFunctions.front()).begin(), std::get<2>(manager.m_pluginFunctions.front()).end());
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    delete plugin;
}

GTEST_TEST(TokenManager_Test, removePluginFeatures)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>> uniqueTokens =
            {
                    {"+", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("+"), 2}},
                    {"*", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("*"), 2}},
                    {"^", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("^"), 1}}
            };
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>> notUniqueTokens =
            {
                    {"abs", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("abs"), 1}}
            };
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs =
            {
                    {afl::detail::getFullPathName("res/plugins/c_api/c_api_plugin", afl::detail::ResourceType::Plugin), {"+", "*", "^"}},
                    {"path/to/alternative_operators", {"+", "*"}},
                    {"path/to/functions", {"abs"}}
            };
    apl::Plugin* plugin = apl::Plugin::load("res/plugins/c_api/c_api_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    std::vector<afl::detail::cApiCreateTokenPluginFunction> cApiCreateTokenPluginFunctions;
    std::vector<afl::detail::cppApiCreateTokenPluginFunction> cppApiCreateTokenPluginFunctions;
    std::vector<afl::detail::cApiCreateTokenAliasesPluginFunction> cApiCreateTokenAliasesPluginFunctions;
    std::vector<afl::detail::cppApiCreateTokenAliasesPluginFunction> cppApiCreateTokenAliasesPluginFunctions;
    const apl::PluginFeatureInfo* const* featureInfos = plugin->getFeatureInfos();
    const apl::PluginFeatureInfo* featureInfo;
    for(size_t i = 0; i < plugin->getFeatureCount(); i++) {
        featureInfo = featureInfos[i];
        if(std::strcmp(featureInfo->featureGroup, afl::k_C_API_CreateTokenFeatureGroupName) == 0)
            cApiCreateTokenPluginFunctions.push_back(reinterpret_cast<afl::detail::cApiCreateTokenPluginFunction>(featureInfo->functionPointer));
        else if(std::strcmp(featureInfo->featureGroup, afl::k_CPP_API_CreateTokenFeatureGroupName) == 0)
            cppApiCreateTokenPluginFunctions.push_back(reinterpret_cast<afl::detail::cppApiCreateTokenPluginFunction>(featureInfo->functionPointer));
        else if(std::strcmp(featureInfo->featureGroup, afl::k_C_API_CreateTokenAliasesFeatureGroupName) == 0)
            cApiCreateTokenAliasesPluginFunctions.push_back(reinterpret_cast<afl::detail::cApiCreateTokenAliasesPluginFunction>(featureInfo->functionPointer));
        else if(std::strcmp(featureInfo->featureGroup, afl::k_CPP_API_CreateTokenAliasesFeatureGroupName) == 0)
            cppApiCreateTokenAliasesPluginFunctions.push_back(reinterpret_cast<afl::detail::cppApiCreateTokenAliasesPluginFunction>(featureInfo->functionPointer));
    }
    std::sort(cApiCreateTokenPluginFunctions.begin(), cApiCreateTokenPluginFunctions.end());
    std::sort(cApiCreateTokenAliasesPluginFunctions.begin(), cApiCreateTokenAliasesPluginFunctions.end());
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::cApiCreateTokenPluginFunction>, std::vector<afl::detail::cApiCreateTokenAliasesPluginFunction>, std::vector<afl::detail::cppApiCreateTokenPluginFunction>, std::vector<afl::detail::cppApiCreateTokenAliasesPluginFunction>>> pluginFunctions =
            {{plugin, cApiCreateTokenPluginFunctions, cApiCreateTokenAliasesPluginFunctions, cppApiCreateTokenPluginFunctions, cppApiCreateTokenAliasesPluginFunctions}};

    afl::detail::TokenManager manager;
    manager.m_uniqueTokens = uniqueTokens;
    manager.m_notUniqueTokens = notUniqueTokens;
    manager.m_pathTokenValueRefs = pathTokenValueRefs;
    manager.m_pluginFunctions = pluginFunctions;

    manager.removePluginFeatures(nullptr);
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removePluginFeatures(plugin);
    uniqueTokens.erase("^");
    uniqueTokens.at("+").second = 1;
    uniqueTokens.at("*").second = 1;
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs.erase(afl::detail::getFullPathName("res/plugins/c_api/c_api_plugin", afl::detail::ResourceType::Plugin));
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    pluginFunctions.clear();
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);
    delete plugin;
}

GTEST_TEST(TokenManager_Test, removeReferences)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>> uniqueTokens =
            {
                    {"+", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("+"), 2}},
                    {"*", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("*"), 2}},
                    {"^", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("^"), 1}}
            };
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>> notUniqueTokens =
            {
                    {"abs", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("abs"), 1}}
            };
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs =
            {
                    {"path/to/operators", {"+", "*", "^"}},
                    {"path/to/alternative_operators", {"+", "*"}},
                    {"path/to/functions", {"abs"}}
            };
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::cApiCreateTokenPluginFunction>, std::vector<afl::detail::cApiCreateTokenAliasesPluginFunction>, std::vector<afl::detail::cppApiCreateTokenPluginFunction>, std::vector<afl::detail::cppApiCreateTokenAliasesPluginFunction>>> pluginFunctions =
            {
                    {nullptr, {nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr}, {nullptr}, {nullptr, nullptr}},
                    {nullptr, {}, {}, {}, {}}
            };

    afl::detail::TokenManager manager;
    manager.m_uniqueTokens = uniqueTokens;
    manager.m_notUniqueTokens = notUniqueTokens;
    manager.m_pathTokenValueRefs = pathTokenValueRefs;
    manager.m_pluginFunctions = pluginFunctions;

    manager.removeReferences("path/to/operators");
    uniqueTokens.erase("^");
    uniqueTokens.at("+").second = 1;
    uniqueTokens.at("*").second = 1;
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs.erase("path/to/operators");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeReferences("path/to/alternative_operators");
    uniqueTokens.erase("+");
    uniqueTokens.erase("*");
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs.erase("path/to/alternative_operators");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeReferences("path/to/functions");
    notUniqueTokens.erase("abs");
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs.erase("path/to/functions");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    ASSERT_TRUE(manager.m_uniqueTokens.empty());
    ASSERT_TRUE(manager.m_notUniqueTokens.empty());
    ASSERT_TRUE(manager.m_pathTokenValueRefs.empty());
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);
}

GTEST_TEST(TokenManager_Test, addToken)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>> uniqueTokens, notUniqueTokens;
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs;
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::cApiCreateTokenPluginFunction>, std::vector<afl::detail::cApiCreateTokenAliasesPluginFunction>, std::vector<afl::detail::cppApiCreateTokenPluginFunction>, std::vector<afl::detail::cppApiCreateTokenAliasesPluginFunction>>> pluginFunctions;

    afl::detail::TokenManager manager;
    manager.addToken(allTokens.at("+"), "path/to/operators");
    uniqueTokens = {{"+", {allTokens.at("+"), 1}}};
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs = {{"path/to/operators", {"+"}}};
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.addToken(allTokens.at("abs"), "path/to/functions");
    notUniqueTokens.emplace("abs", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("abs"), 1});
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs.emplace("path/to/functions", std::vector<std::string>{"abs"});
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.addToken(allTokens.at("*"), "path/to/operators");
    uniqueTokens.emplace("*", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("*"), 1});
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs.at("path/to/operators").push_back("*");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.addToken(allTokens.at("+"), "path/to/operators");
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.addToken(allTokens.at("*"), "path/to/alternative_operators");
    uniqueTokens.at("*").second = 2;
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs.emplace("path/to/alternative_operators", std::vector<std::string>{"*"});
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);
}

GTEST_TEST(TokenManager_Test, removeToken)
{
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>> uniqueTokens =
            {
                    {"+", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("+"), 2}},
                    {"*", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("*"), 2}},
                    {"^", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("^"), 1}}
            };
    std::unordered_map<std::string, std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>> notUniqueTokens =
            {
                    {"abs", std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, size_t>{allTokens.at("abs"), 1}}
            };
    std::unordered_map<std::string, std::vector<std::string>> pathTokenValueRefs =
            {
                    {"path/to/operators", {"+", "*", "^"}},
                    {"path/to/alternative_operators", {"+", "*"}},
                    {"path/to/functions", {"abs"}}
            };
    std::vector<std::tuple<const apl::Plugin*, std::vector<afl::detail::cApiCreateTokenPluginFunction>, std::vector<afl::detail::cApiCreateTokenAliasesPluginFunction>, std::vector<afl::detail::cppApiCreateTokenPluginFunction>, std::vector<afl::detail::cppApiCreateTokenAliasesPluginFunction>>> pluginFunctions =
            {
                    {nullptr, {nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr}, {nullptr}, {nullptr, nullptr}},
                    {nullptr, {}, {}, {}, {}}
            };

    afl::detail::TokenManager manager;
    manager.m_uniqueTokens = uniqueTokens;
    manager.m_notUniqueTokens = notUniqueTokens;
    manager.m_pathTokenValueRefs = pathTokenValueRefs;
    manager.m_pluginFunctions = pluginFunctions;

    manager.removeToken("*", "path/to/operators");
    uniqueTokens.at("*").second = 1;
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs.at("path/to/operators") = {"+", "^"};
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken("*", "path/to/operators");
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken("*", "path/to/alternative_operators");
    uniqueTokens.erase("*");
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs.at("path/to/alternative_operators") = {"+"};
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken("abs", "pat/to/not/to/remove/functions");
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken("abs", "path/to/functions");
    notUniqueTokens.erase("abs");
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs.erase("path/to/functions");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken("+", "path/to/alternative_operators");
    uniqueTokens.at("+").second = 1;
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs.erase("path/to/alternative_operators");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken("some_operator", "path/to/operators");
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken("^", "path/to/operators");
    uniqueTokens.erase("^");
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs.at("path/to/operators") = {"+"};
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    manager.removeToken("+", "path/to/operators");
    uniqueTokens.erase("+");
    ASSERT_EQ(manager.m_uniqueTokens, uniqueTokens);
    ASSERT_EQ(manager.m_notUniqueTokens, notUniqueTokens);
    pathTokenValueRefs.erase("path/to/operators");
    ASSERT_EQ(manager.m_pathTokenValueRefs, pathTokenValueRefs);
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);

    ASSERT_TRUE(manager.m_uniqueTokens.empty());
    ASSERT_TRUE(manager.m_notUniqueTokens.empty());
    ASSERT_TRUE(manager.m_pathTokenValueRefs.empty());
    ASSERT_EQ(manager.m_pluginFunctions, pluginFunctions);
}

GTEST_TEST(TokenManager_Test, createAliases_c_api)
{
    afl::detail::TokenManager manager;
    apl::Plugin* plugin = apl::Plugin::load("res/plugins/c_api/c_api_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    manager.addPluginFeatures(plugin);

    std::vector<afl::TokenAliases<std::string>> aliases = manager.createAliases("1");
    std::vector<afl::TokenAliases<std::string>> expected = {{afl::TokenAliasType::String, {"one"}}};
    ASSERT_EQ(aliases, expected);

    aliases = manager.createAliases("12");
    ASSERT_TRUE(aliases.empty());
    delete plugin;
}

GTEST_TEST(TokenManager_Test, createAliases_cpp_api)
{
    afl::detail::TokenManager manager;
    apl::Plugin* plugin = apl::Plugin::load("res/plugins/cpp_api/cpp_api_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    manager.addPluginFeatures(plugin);

    std::vector<afl::TokenAliases<std::string>> aliases = manager.createAliases("hello");
    std::vector<afl::TokenAliases<std::string>> expected = {{afl::TokenAliasType::String, {"hello_world"}}};
    ASSERT_EQ(aliases, expected);

    aliases = manager.createAliases("12");
    ASSERT_TRUE(aliases.empty());
    delete plugin;
}

GTEST_TEST(TokenManager_Test, createToken_c_api)
{
    afl::detail::TokenManager manager;
    apl::Plugin* plugin = apl::Plugin::load("res/plugins/c_api/c_api_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    manager.addPluginFeatures(plugin);

    std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, std::string> created = manager.createToken("1", false);
    std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, std::string> expected = {std::make_shared<afl::detail::TokenPtrBundle<std::string>>(std::make_shared<afl::Token<std::string>>("1", afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None), std::vector<afl::TokenAliases<std::string>>{}), afl::detail::getFullPathName("res/plugins/c_api/c_api_plugin", afl::detail::ResourceType::Plugin)};
    ASSERT_NE(created.first, nullptr);
    ASSERT_EQ(*created.first, *expected.first);
    ASSERT_EQ(created.second, expected.second);

    created = manager.createToken("1", true);
    ASSERT_NE(created.first, nullptr);
    std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, std::string> created2 = manager.createToken("1");
    ASSERT_NE(created2.first, nullptr);
    ASSERT_EQ(*created.first, *created2.first);
    ASSERT_EQ(created.second, created2.second);

    expected = {std::make_shared<afl::detail::TokenPtrBundle<std::string>>(std::make_shared<afl::Token<std::string>>("1", afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None),
                                                                           std::vector<afl::TokenAliases<std::string>>{afl::TokenAliases<std::string>{afl::TokenAliasType::String, {"one"}}}), afl::detail::getFullPathName("res/plugins/c_api/c_api_plugin", afl::detail::ResourceType::Plugin)};
    ASSERT_EQ(*created.first, *expected.first);
    ASSERT_EQ(created.second, expected.second);

    created = manager.createToken("12", true);
    ASSERT_EQ(created.first.get(), nullptr);
    ASSERT_EQ(created.second, "");
    delete plugin;
}

GTEST_TEST(TokenManager_Test, createToken_cpp_api)
{
    afl::detail::TokenManager manager;
    apl::Plugin* plugin = apl::Plugin::load("res/plugins/cpp_api/cpp_api_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    manager.addPluginFeatures(plugin);

    std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, std::string> created = manager.createToken("hello", false);
    std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, std::string> expected = {std::make_shared<afl::detail::TokenPtrBundle<std::string>>(std::make_shared<afl::Token<std::string>>("hello", afl::TokenType::Constant, 0, 0, afl::TokenAssociativity::None), std::vector<afl::TokenAliases<std::string>>{}), afl::detail::getFullPathName("res/plugins/cpp_api/cpp_api_plugin", afl::detail::ResourceType::Plugin)};
    ASSERT_NE(created.first, nullptr);
    ASSERT_EQ(*created.first, *expected.first);
    ASSERT_EQ(created.second, expected.second);

    created = manager.createToken("hello", true);
    ASSERT_NE(created.first, nullptr);
    std::pair<std::shared_ptr<afl::detail::TokenPtrBundle<std::string>>, std::string> created2 = manager.createToken("hello");
    ASSERT_NE(created2.first, nullptr);
    ASSERT_EQ(*created.first, *created2.first);
    ASSERT_EQ(created.second, created2.second);

    expected = {std::make_shared<afl::detail::TokenPtrBundle<std::string>>(std::make_shared<afl::Token<std::string>>("hello", afl::TokenType::Constant, 0, 0, afl::TokenAssociativity::None),
                                                                           std::vector<afl::TokenAliases<std::string>>{afl::TokenAliases<std::string>{afl::TokenAliasType::String, {"hello_world"}}}), afl::detail::getFullPathName("res/plugins/cpp_api/cpp_api_plugin", afl::detail::ResourceType::Plugin)};
    ASSERT_EQ(*created.first, *expected.first);
    ASSERT_EQ(created.second, expected.second);

    created = manager.createToken("12", true);
    ASSERT_EQ(created.first.get(), nullptr);
    ASSERT_EQ(created.second, "");

    delete plugin;
}

GTEST_TEST(TokenManager_Test, getToken)
{
    afl::detail::TokenManager manager;
    manager.addToken(allTokens.at("+"), "path/to/operators");
    manager.addToken(allTokens.at("*"), "path/to/operators");
    manager.addToken(allTokens.at("^"), "path/to/operators");
    apl::Plugin* plugin = apl::Plugin::load("res/plugins/c_api/c_api_plugin");
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
    afl::detail::TokenPtrBundle<std::string> tokenBundle(std::make_shared<afl::Token<std::string>>("1", afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None),
                                                         std::vector<afl::TokenAliases<std::string>>{afl::TokenAliases<std::string>{afl::TokenAliasType::String, std::vector<std::string>{"one"}}});
    ASSERT_EQ(*manager.getToken("1", true), tokenBundle);
    ASSERT_EQ(manager.getToken("no_token", true).get(), nullptr);
    delete plugin;
}

GTEST_TEST(TokenManager_Test, getTokens)
{
    afl::detail::TokenManager manager;
    manager.addToken(allTokens.at("+"), "path/to/operators");
    manager.addToken(allTokens.at("*"), "path/to/operators");
    manager.addToken(allTokens.at("^"), "path/to/operators");
    manager.addToken(allTokens.at("abs"), "path/to/functions");

    std::vector<std::shared_ptr<const afl::detail::TokenPtrBundle<std::string>>> expected =
            {allTokens.at("+"), allTokens.at("*"), allTokens.at("^"), allTokens.at("abs") };
    std::sort(expected.begin(), expected.end());
    std::vector<std::shared_ptr<const afl::detail::TokenPtrBundle<std::string>>> tokens = manager.getTokens();
    std::sort(tokens.begin(), tokens.end());
    ASSERT_EQ(expected, tokens);
}

GTEST_TEST(TokenManager_Test, filterTokens)
{
    afl::detail::TokenManager manager;
    manager.addToken(allTokens.at("+"), "path/to/operators");
    manager.addToken(allTokens.at("*"), "path/to/operators");
    manager.addToken(allTokens.at("^"), "path/to/operators");
    manager.addToken(allTokens.at("abs"), "path/to/functions");

    std::vector<std::shared_ptr<const afl::detail::TokenPtrBundle<std::string>>> expected =
            {allTokens.at("+"), allTokens.at("*"), allTokens.at("^") };
    std::sort(expected.begin(), expected.end());
    std::vector<std::shared_ptr<const afl::detail::TokenPtrBundle<std::string>>> tokens =
            manager.filterTokens([](const afl::Token<std::string>* t){ return t->type == afl::TokenType::Operator; });
    std::sort(tokens.begin(), tokens.end());
    ASSERT_EQ(expected, tokens);

    expected = {allTokens.at("abs") };
    tokens = manager.filterTokens([](const afl::Token<std::string>* t){ return t->type == afl::TokenType::Function; });
    ASSERT_EQ(expected, tokens);
}

GTEST_TEST(tokenmanager_h_Test, stringToTokens)
{
    afl::detail::TokenManager tokenManager;
    tokenManager.addToken(allTokens.at("+"), "custom_tokens.atokens");
    tokenManager.addToken(allTokens.at("*"), "custom_tokens.atokens");
    tokenManager.addToken(allTokens.at("abs"), "custom_tokens.atokens");
    auto token3 = std::make_shared<afl::Token<std::string>>("3", afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None);
    auto token5 = std::make_shared<afl::Token<std::string>>("5", afl::TokenType::Number, 0, 0, afl::TokenAssociativity::None);
    auto tokenab = std::make_shared<afl::Token<std::string>>("ab", afl::TokenType::Constant, 0, 0, afl::TokenAssociativity::None);
    auto tokenBracketOpen = std::make_shared<afl::Token<std::string>>("(", afl::TokenType::BracketOpen, 0, 0, afl::TokenAssociativity::None);
    auto tokenBracketClose = std::make_shared<afl::Token<std::string>>(")", afl::TokenType::BracketClose, 0, 0, afl::TokenAssociativity::None);
    tokenManager.addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(token3, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    tokenManager.addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(token5, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    tokenManager.addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(tokenab, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    tokenManager.addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(tokenBracketOpen, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    tokenManager.addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(tokenBracketClose, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");

    std::vector<std::shared_ptr<const afl::Token<std::string>>> tokens =
            {token3, allTokens.at("+")->token, token5, allTokens.at("*")->token, allTokens.at("abs")->token,
             tokenBracketOpen, tokenab, tokenBracketClose};
    ASSERT_EQ(afl::detail::stringToTokens(&tokenManager, "3+5*abs(ab)"), tokens);
}

GTEST_TEST(tokenmanager_h_Test, toFunctionString_with_brackets_and_semicolons)
{
    afl::detail::TokenManager tokenManager;
    auto bracketOpen = std::make_shared<afl::Token<std::string>>("[", afl::TokenType::BracketOpen, 0, 0, afl::TokenAssociativity::None);
    auto bracketClose = std::make_shared<afl::Token<std::string>>("]", afl::TokenType::BracketClose, 0, 0, afl::TokenAssociativity::None);
    auto argumentDelimiter = std::make_shared<afl::Token<std::string>>("#", afl::TokenType::ArgumentDelimiter, 0, 0, afl::TokenAssociativity::None);
    tokenManager.addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(bracketOpen, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    tokenManager.addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(bracketClose, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    tokenManager.addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(argumentDelimiter, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");

    std::default_random_engine rng(std::chrono::system_clock::now().time_since_epoch().count());

    // 3+5*abs(ab)^3*pow(3^5;abs(3+ab))
    std::vector<afl::TokenGroup<std::string>> tokenGroups;
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({0}));
    tokenGroups.emplace_back(afl::test::tokenPlus, std::vector<size_t>({1}));
    tokenGroups.emplace_back(afl::test::token5, std::vector<size_t>({2}));
    tokenGroups.emplace_back(afl::test::tokenMultiply, std::vector<size_t>({3}));
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({4}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({5}));
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({6, 0}));
    tokenGroups.emplace_back(std::vector<size_t>({6, 1}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({7}));
    tokenGroups.emplace_back(afl::test::tokenPower, std::vector<size_t>({8}));
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({9}));
    tokenGroups.emplace_back(afl::test::tokenMultiply, std::vector<size_t>({10}));
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({11}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({12}));
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({13, 0}));
    tokenGroups.emplace_back(afl::test::tokenPower, std::vector<size_t>({13, 1}));
    tokenGroups.emplace_back(afl::test::token5, std::vector<size_t>({13, 2}));
    tokenGroups.emplace_back(std::vector<size_t>({13, 3}));
    tokenGroups.emplace_back(afl::test::tokenSemicolon, std::vector<size_t>({14}));
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({15, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({15, 1}));
    tokenGroups.emplace_back(std::vector<size_t>({15, 2, 0}));
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({15, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenPlus, std::vector<size_t>({15, 2, 1}));
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({15, 2, 2}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({15, 3}));
    tokenGroups.emplace_back(std::vector<size_t>({15, 3, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({16}));

    std::shuffle(tokenGroups.begin(), tokenGroups.end(), rng);
    ASSERT_EQ(afl::detail::toFunctionString(&tokenManager, tokenGroups), "3+5*abs(ab)^3*pow(3^5;abs(3+ab))");

    tokenGroups.clear();
    // pow(abs(3);abs(ab);abs(5))
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({0}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({1}));
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({2, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({2, 1}));
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({2, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({2, 3}));
    tokenGroups.emplace_back(afl::test::tokenSemicolon, std::vector<size_t>({3}));
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({4, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({4, 1}));
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({4, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({4, 3}));
    tokenGroups.emplace_back(afl::test::tokenSemicolon, std::vector<size_t>({5}));
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({6, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({6, 1}));
    tokenGroups.emplace_back(afl::test::token5, std::vector<size_t>({6, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({6, 3}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({7}));

    std::shuffle(tokenGroups.begin(), tokenGroups.end(), rng);
    ASSERT_EQ(afl::detail::toFunctionString(&tokenManager, tokenGroups), "pow(abs(3);abs(ab);abs(5))");

    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(3+5;ab));abs(3+ab))
    tokenGroups.clear();
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({0}));
    tokenGroups.emplace_back(afl::test::tokenPlus, std::vector<size_t>({1}));
    tokenGroups.emplace_back(afl::test::token5, std::vector<size_t>({2}));
    tokenGroups.emplace_back(afl::test::tokenMultiply, std::vector<size_t>({3}));
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({4}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({5}));
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({6, 0}));
    tokenGroups.emplace_back(std::vector<size_t>({6, 1}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({7}));
    tokenGroups.emplace_back(afl::test::tokenPower, std::vector<size_t>({8}));
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({9}));
    tokenGroups.emplace_back(afl::test::tokenMultiply, std::vector<size_t>({10}));

    // 3+5*abs(ab)^3*   pow(pow(pow(abs(3);abs(ab));pow(3+5;ab));abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({11}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({12}));
    // 3+5*abs(ab)^3*pow(   pow(pow(abs(3);abs(ab));pow(3+5;ab))   ;abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({13, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({13, 1}));
    // 3+5*abs(ab)^3*pow(pow(   pow(abs(3);abs(ab));pow(3+5;ab))  ;abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({13, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({13, 2, 1}));
    // 3+5*abs(ab)^3*pow(pow(pow(   abs(3)   ;abs(ab));pow(3+5;ab));abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({13, 2, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({13, 2, 2, 1}));
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({13, 2, 2, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({13, 2, 2, 3}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3)   ;   abs(ab));pow(3+5;ab));abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenSemicolon, std::vector<size_t>({13, 2, 3}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);   abs(ab)   );pow(3+5;ab));abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({13, 2, 4, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({13, 2, 4, 1}));
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({13, 2, 4, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({13, 2, 4, 3}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({13, 2, 5}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab))   ;   pow(3+5;ab));abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenSemicolon, std::vector<size_t>({13, 3}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));   pow(3+5;ab)   );abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({13, 4, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({13, 4, 1}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(   3+5   ;ab));abs(3+ab))
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({13, 4, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenPlus, std::vector<size_t>({13, 4, 2, 1}));
    tokenGroups.emplace_back(afl::test::token5, std::vector<size_t>({13, 4, 2, 2}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(3+5   ;   ab));abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenSemicolon, std::vector<size_t>({13, 4, 3}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(3+5;   ab   ));abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({13, 4, 4, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({13, 4, 5}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({13, 5}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(3+5;ab))   ;   abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenSemicolon, std::vector<size_t>({14}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(3+5;ab));   abs(3+ab)   )
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({15, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketOpen, std::vector<size_t>({15, 1}));
    tokenGroups.emplace_back(std::vector<size_t>({15, 2, 0}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(3+5;ab));abs(   3+ab   ))
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({15, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenPlus, std::vector<size_t>({15, 2, 1}));
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({15, 2, 2}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({15, 3}));
    tokenGroups.emplace_back(std::vector<size_t>({15, 3, 0}));
    tokenGroups.emplace_back(afl::test::tokenBracketClose, std::vector<size_t>({16}));

    std::shuffle(tokenGroups.begin(), tokenGroups.end(), rng);
    ASSERT_EQ(afl::detail::toFunctionString(&tokenManager, tokenGroups), "3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(3+5;ab));abs(3+ab))");
}

GTEST_TEST(tokenmanager_h_Test, toFunctionString_without_brackets_and_semicolons)
{
    afl::detail::TokenManager tokenManager;
    auto bracketOpen = std::make_shared<afl::Token<std::string>>("[", afl::TokenType::BracketOpen, 0, 0, afl::TokenAssociativity::None);
    auto bracketClose = std::make_shared<afl::Token<std::string>>("]", afl::TokenType::BracketClose, 0, 0, afl::TokenAssociativity::None);
    auto argumentDelimiter = std::make_shared<afl::Token<std::string>>("#", afl::TokenType::ArgumentDelimiter, 0, 0, afl::TokenAssociativity::None);
    tokenManager.addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(bracketOpen, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    tokenManager.addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(bracketClose, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    tokenManager.addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(argumentDelimiter, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");

    std::default_random_engine rng(std::chrono::system_clock::now().time_since_epoch().count());

    // 3+5*abs(ab)^3*pow(3^5;abs(3+ab))
    std::vector<afl::TokenGroup<std::string>> tokenGroups;
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({0}));
    tokenGroups.emplace_back(afl::test::tokenPlus, std::vector<size_t>({1}));
    tokenGroups.emplace_back(afl::test::token5, std::vector<size_t>({2}));
    tokenGroups.emplace_back(afl::test::tokenMultiply, std::vector<size_t>({3}));
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({4}));
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({6, 0}));
    tokenGroups.emplace_back(std::vector<size_t>({6, 1}));
    tokenGroups.emplace_back(afl::test::tokenPower, std::vector<size_t>({8}));
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({9}));
    tokenGroups.emplace_back(afl::test::tokenMultiply, std::vector<size_t>({10}));
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({11}));
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({13, 0}));
    tokenGroups.emplace_back(afl::test::tokenPower, std::vector<size_t>({13, 1}));
    tokenGroups.emplace_back(afl::test::token5, std::vector<size_t>({13, 2}));
    tokenGroups.emplace_back(std::vector<size_t>({13, 3}));
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({15, 0}));
    tokenGroups.emplace_back(std::vector<size_t>({15, 2, 0}));
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({15, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenPlus, std::vector<size_t>({15, 2, 1}));
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({15, 2, 2}));
    tokenGroups.emplace_back(std::vector<size_t>({15, 3, 0}));

    std::shuffle(tokenGroups.begin(), tokenGroups.end(), rng);
    ASSERT_EQ(afl::detail::toFunctionString(&tokenManager, tokenGroups), "3+5*abs[ab]^3*pow[3^5#abs[3+ab]]");

    tokenGroups.clear();
    // pow(abs(3);abs(ab);abs(5))
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({0}));
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({2, 0}));
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({2, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({4, 0}));
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({4, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({6, 0}));
    tokenGroups.emplace_back(afl::test::token5, std::vector<size_t>({6, 2, 0}));

    std::shuffle(tokenGroups.begin(), tokenGroups.end(), rng);
    ASSERT_EQ(afl::detail::toFunctionString(&tokenManager, tokenGroups), "pow[abs[3]#abs[ab]#abs[5]]");

    tokenGroups.clear();
    // pow(pow(abs(3);abs(ab));pow(3+5;ab))
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({0}));
    // pow(   pow(abs(3);abs(ab))   ;pow(3+5;ab))
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({2, 0}));
    // pow(pow(   abs(3)   ;abs(ab));pow(3+5;ab))
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({2, 2, 0}));
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({2, 2, 2, 0}));
    // pow(pow(abs(3);   abs(ab)   );pow(3+5;ab))
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({2, 4, 0}));
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({2, 4, 2, 0}));
    // pow(pow(abs(3);abs(ab));   pow(3+5;ab)   )
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({4, 0}));
    // pow(pow(abs(3);abs(ab));pow(   3+5   ;ab))
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({4, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenPlus, std::vector<size_t>({4, 2, 1}));
    tokenGroups.emplace_back(afl::test::token5, std::vector<size_t>({4, 2, 2}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(3+5;   ab   ))
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({4, 4, 0}));

    std::shuffle(tokenGroups.begin(), tokenGroups.end(), rng);
    ASSERT_EQ(afl::detail::toFunctionString(&tokenManager, tokenGroups), "pow[pow[abs[3]#abs[ab]]#pow[3+5#ab]]");

    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(3+5;ab));abs(3+ab))
    tokenGroups.clear();
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({0}));
    tokenGroups.emplace_back(afl::test::tokenPlus, std::vector<size_t>({1}));
    tokenGroups.emplace_back(afl::test::token5, std::vector<size_t>({2}));
    tokenGroups.emplace_back(afl::test::tokenMultiply, std::vector<size_t>({3}));
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({4}));
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({6, 0}));
    tokenGroups.emplace_back(std::vector<size_t>({6, 1}));
    tokenGroups.emplace_back(afl::test::tokenPower, std::vector<size_t>({8}));
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({9}));
    tokenGroups.emplace_back(afl::test::tokenMultiply, std::vector<size_t>({10}));

    // 3+5*abs(ab)^3*   pow(pow(pow(abs(3);abs(ab));pow(3+5;ab));abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({11}));
    // 3+5*abs(ab)^3*pow(   pow(pow(abs(3);abs(ab));pow(3+5;ab))   ;abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({13, 0}));
    // 3+5*abs(ab)^3*pow(pow(   pow(abs(3);abs(ab));pow(3+5;ab))  ;abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({13, 2, 0}));
    // 3+5*abs(ab)^3*pow(pow(pow(   abs(3)   ;abs(ab));pow(3+5;ab));abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({13, 2, 2, 0}));
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({13, 2, 2, 2, 0}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);   abs(ab)   );pow(3+5;ab));abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({13, 2, 4, 0}));
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({13, 2, 4, 2, 0}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));   pow(3+5;ab)   );abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenPow, std::vector<size_t>({13, 4, 0}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(   3+5   ;ab));abs(3+ab))
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({13, 4, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenPlus, std::vector<size_t>({13, 4, 2, 1}));
    tokenGroups.emplace_back(afl::test::token5, std::vector<size_t>({13, 4, 2, 2}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(3+5;   ab   ));abs(3+ab))
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({13, 4, 4, 0}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(3+5;ab));   abs(3+ab)   )
    tokenGroups.emplace_back(afl::test::tokenAbs, std::vector<size_t>({15, 0}));
    tokenGroups.emplace_back(std::vector<size_t>({15, 2, 0}));
    // 3+5*abs(ab)^3*pow(pow(pow(abs(3);abs(ab));pow(3+5;ab));abs(   3+ab   ))
    tokenGroups.emplace_back(afl::test::token3, std::vector<size_t>({15, 2, 0}));
    tokenGroups.emplace_back(afl::test::tokenPlus, std::vector<size_t>({15, 2, 1}));
    tokenGroups.emplace_back(afl::test::tokenAb, std::vector<size_t>({15, 2, 2}));
    tokenGroups.emplace_back(std::vector<size_t>({15, 3, 0}));

    std::shuffle(tokenGroups.begin(), tokenGroups.end(), rng);
    ASSERT_EQ(afl::detail::toFunctionString(&tokenManager, tokenGroups), "3+5*abs[ab]^3*pow[pow[pow[abs[3]#abs[ab]]#pow[3+5#ab]]#abs[3+ab]]");
}

#include "gtest/gtest.h"

#include "../../src/private/functionoptimizer.h"

#include "../../SDK/tests/src/include.h"

GTEST_TEST(Test_FunctionOptimizer, optimizeWithPlugins_c_api_string)
{
    auto resourceManager = std::make_shared<afl::detail::ResourceManager>();
    resourceManager->load("res/plugins/c_api/c_api_plugin", afl::detail::ResourceType::Plugin);
    afl::detail::FunctionOptimizer optimizer(resourceManager);
    resourceManager->load("../extensions/tokens", afl::detail::ResourceType::Extension);
    ASSERT_EQ(optimizer.optimizeWithPlugins("1*1"), "1");
    ASSERT_EQ(optimizer.optimizeWithPlugins("4+2*1*1*1*1"), "4+2*1");
    ASSERT_EQ(optimizer.optimizeWithPlugins("1*1*1*1*13/3+4"), "1*13/3+4");
    ASSERT_EQ(optimizer.optimizeWithPlugins("4+2*1*1*1*1*1/3+4"), "4+2*1/3+4");
    resourceManager->unload("res/plugins/c_api/c_api_plugin", afl::detail::ResourceType::Plugin);
    ASSERT_EQ(optimizer.optimizeWithPlugins("1*1*1*1"), "1*1*1*1");
}
GTEST_TEST(Test_FunctionOptimizer, optimizeWithPlugins_c_api_syntax_tree)
{
    auto resourceManager = std::make_shared<afl::detail::ResourceManager>();
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::token0, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::token3, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::token5, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenAb, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenAbs, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenPlus, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenMultiply, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenBracketOpen, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenBracketClose, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->load("res/plugins/c_api/c_api_plugin", afl::detail::ResourceType::Plugin);
    afl::detail::FunctionOptimizer optimizer(resourceManager);
    resourceManager->load("../extensions/tokens", afl::detail::ResourceType::Extension);
    afl::SyntaxTree<afl::TokenGroup<std::string>> optimizedSyntaxTree = optimizer.optimizeWithPlugins(afl::generateSyntaxTree(afl::detail::toGroupVector(resourceManager->getTokenManager()->stringToTokens("0*3*5*ab"))));
    std::vector<std::shared_ptr<const afl::Token<std::string>>> optimizedTokens = resourceManager->getTokenManager()->tokenGroupsToTokens(optimizedSyntaxTree.root()->subtreeValues());
    ASSERT_EQ(afl::stringify(optimizedTokens.begin(), optimizedTokens.end(), ""), "0");
    optimizedSyntaxTree = optimizer.optimizeWithPlugins(afl::generateSyntaxTree(afl::detail::toGroupVector(resourceManager->getTokenManager()->stringToTokens("5+3*abs(ab)*0*3*5"))));
    optimizedTokens = resourceManager->getTokenManager()->tokenGroupsToTokens(optimizedSyntaxTree.root()->subtreeValues());
    ASSERT_EQ(afl::stringify(optimizedTokens.begin(), optimizedTokens.end(), ""), "5+0");
    optimizedSyntaxTree = optimizer.optimizeWithPlugins(afl::generateSyntaxTree(afl::detail::toGroupVector(resourceManager->getTokenManager()->stringToTokens("3*0*3*5+3+ab"))));
    optimizedTokens = resourceManager->getTokenManager()->tokenGroupsToTokens(optimizedSyntaxTree.root()->subtreeValues());
    ASSERT_EQ(afl::stringify(optimizedTokens.begin(), optimizedTokens.end(), ""), "0+3+ab");
    optimizedSyntaxTree = optimizer.optimizeWithPlugins(afl::generateSyntaxTree(afl::detail::toGroupVector(resourceManager->getTokenManager()->stringToTokens("5+5*3*0*5*3+3+4"))));
    optimizedTokens = resourceManager->getTokenManager()->tokenGroupsToTokens(optimizedSyntaxTree.root()->subtreeValues());
    ASSERT_EQ(afl::stringify(optimizedTokens.begin(), optimizedTokens.end(), ""), "5+0+3+4");
    resourceManager->unload("res/plugins/c_api/c_api_plugin", afl::detail::ResourceType::Plugin);
    optimizedSyntaxTree = optimizer.optimizeWithPlugins(afl::generateSyntaxTree(afl::detail::toGroupVector(resourceManager->getTokenManager()->stringToTokens("0*3*5"))));
    optimizedTokens = resourceManager->getTokenManager()->tokenGroupsToTokens(optimizedSyntaxTree.root()->subtreeValues());
    ASSERT_EQ(afl::stringify(optimizedTokens.begin(), optimizedTokens.end(), ""), "0*3*5");
}

GTEST_TEST(Test_FunctionOptimizer, optimizeWithPlugins_cpp_api_string)
{
    auto resourceManager = std::make_shared<afl::detail::ResourceManager>();
    resourceManager->load("res/plugins/cpp_api/cpp_api_plugin", afl::detail::ResourceType::Plugin);
    afl::detail::FunctionOptimizer optimizer(resourceManager);
    resourceManager->load("../extensions/tokens", afl::detail::ResourceType::Extension);
    ASSERT_EQ(optimizer.optimizeWithPlugins("0+123+0"), "123");
    ASSERT_EQ(optimizer.optimizeWithPlugins("4+0"), "4");
    ASSERT_EQ(optimizer.optimizeWithPlugins("0+4"), "4");
    ASSERT_EQ(optimizer.optimizeWithPlugins("0+4+0+4+0"), "4+4");
    resourceManager->unload("res/plugins/cpp_api/cpp_api_plugin", afl::detail::ResourceType::Plugin);
    ASSERT_EQ(optimizer.optimizeWithPlugins("0+123+0"), "0+123+0");
}
GTEST_TEST(Test_FunctionOptimizer, optimizeWithPlugins_cpp_api_syntax_tree)
{
    auto resourceManager = std::make_shared<afl::detail::ResourceManager>();
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::token0, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::token3, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::token5, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenAb, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenAbs, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenPow, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenPlus, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenMultiply, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenSemicolon, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenBracketOpen, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenBracketClose, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->load("res/plugins/cpp_api/cpp_api_plugin", afl::detail::ResourceType::Plugin);
    afl::detail::FunctionOptimizer optimizer(resourceManager);
    resourceManager->load("../extensions/tokens", afl::detail::ResourceType::Extension);
    afl::SyntaxTree<afl::TokenGroup<std::string>> optimizedSyntaxTree = optimizer.optimizeWithPlugins(afl::generateSyntaxTree(afl::detail::toGroupVector(resourceManager->getTokenManager()->stringToTokens("pow(3;5)"))));
    std::vector<std::shared_ptr<const afl::Token<std::string>>> optimizedTokens = resourceManager->getTokenManager()->tokenGroupsToTokens(optimizedSyntaxTree.root()->subtreeValues());
    ASSERT_EQ(afl::stringify(optimizedTokens.begin(), optimizedTokens.end(), ""), "243");
    optimizedSyntaxTree = optimizer.optimizeWithPlugins(afl::generateSyntaxTree(afl::detail::toGroupVector(resourceManager->getTokenManager()->stringToTokens("3+pow(5;5)"))));
    optimizedTokens = resourceManager->getTokenManager()->tokenGroupsToTokens(optimizedSyntaxTree.root()->subtreeValues());
    ASSERT_EQ(afl::stringify(optimizedTokens.begin(), optimizedTokens.end(), ""), "3+3125");
    optimizedSyntaxTree = optimizer.optimizeWithPlugins(afl::generateSyntaxTree(afl::detail::toGroupVector(resourceManager->getTokenManager()->stringToTokens("pow(3;0)*ab"))));
    optimizedTokens = resourceManager->getTokenManager()->tokenGroupsToTokens(optimizedSyntaxTree.root()->subtreeValues());
    ASSERT_EQ(afl::stringify(optimizedTokens.begin(), optimizedTokens.end(), ""), "1*ab");
    optimizedSyntaxTree = optimizer.optimizeWithPlugins(afl::generateSyntaxTree(afl::detail::toGroupVector(resourceManager->getTokenManager()->stringToTokens("5+pow(3;3)+3"))));
    optimizedTokens = resourceManager->getTokenManager()->tokenGroupsToTokens(optimizedSyntaxTree.root()->subtreeValues());
    ASSERT_EQ(afl::stringify(optimizedTokens.begin(), optimizedTokens.end(), ""), "5+27+3");
    resourceManager->unload("res/plugins/cpp_api/cpp_api_plugin", afl::detail::ResourceType::Plugin);
    optimizedSyntaxTree = optimizer.optimizeWithPlugins(afl::generateSyntaxTree(afl::detail::toGroupVector(resourceManager->getTokenManager()->stringToTokens("pow(3;5)"))));
    optimizedTokens = resourceManager->getTokenManager()->tokenGroupsToTokens(optimizedSyntaxTree.root()->subtreeValues());
    ASSERT_EQ(afl::stringify(optimizedTokens.begin(), optimizedTokens.end(), ""), "pow(3;5)");
}

GTEST_TEST(Test_FunctionOptimizer, optimizeFunction)
{
    auto resourceManager = std::make_shared<afl::detail::ResourceManager>();
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::token0, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::token1, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::token3, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::token5, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenAb, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenAbs, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenPow, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenPlus, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenMultiply, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenSemicolon, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenBracketOpen, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    resourceManager->getTokenManager()->addToken(std::make_shared<afl::detail::TokenPtrBundle<std::string>>(afl::test::tokenBracketClose, std::vector<afl::TokenAliases<std::string>>{}), "custom_tokens.atokens");
    afl::detail::FunctionOptimizer optimizer(resourceManager);
    resourceManager->load("res/plugins/c_api/c_api_plugin", afl::detail::ResourceType::Plugin);
    resourceManager->load("res/plugins/cpp_api/cpp_api_plugin", afl::detail::ResourceType::Plugin);
    resourceManager->load("../extensions/tokens", afl::detail::ResourceType::Extension);
    // test optimizeWithPlugins string
    ASSERT_EQ(optimizer.optimizeFunction("1*1*1"), "1");
    ASSERT_EQ(optimizer.optimizeFunction("1*1*1*3+0"), "1*3");
    ASSERT_EQ(optimizer.optimizeFunction("0+3*1*1*1"), "3*1");
    ASSERT_EQ(optimizer.optimizeFunction("1*1*1+0+1*1*1"), "1+1");
    // test optimizeWithPlugins SyntaxTree
    ASSERT_EQ(optimizer.optimizeFunction("0*pow(3+5;0)*3+3*0"), "0");
    ASSERT_EQ(optimizer.optimizeFunction("5*pow(5;0)+3"), "5*1+3");
    ASSERT_EQ(optimizer.optimizeFunction("pow(0*3*0*5*3;pow(3;3))"), "0");
    ASSERT_EQ(optimizer.optimizeFunction("pow(3;0*5)"), "1");
//    // test all
    ASSERT_EQ(optimizer.optimizeFunction("pow(3;pow(0;5)*3+pow(1*1*1;3+0))"), "3");

    resourceManager->unload("res/plugins/c_api/c_api_plugin", afl::detail::ResourceType::Plugin);
    ASSERT_EQ(optimizer.optimizeFunction("pow(3;pow(0;5)*3+pow(1*1*1;3+0))"), "pow(3;0*3+pow(1*1*1;3))");
    resourceManager->unload("res/plugins/cpp_api/cpp_api_plugin", afl::detail::ResourceType::Plugin);
    ASSERT_EQ(optimizer.optimizeFunction("pow(3;pow(0;5)*3+pow(1*1*1;3+0))"), "pow(3;pow(0;5)*3+pow(1*1*1;3+0))");
}

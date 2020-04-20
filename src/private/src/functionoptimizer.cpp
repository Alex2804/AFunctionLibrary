#include "../functionoptimizer.h"

#include "AFunctionLibrary/implementation/optimizefunctionapi/optimizefunctionapi_definitions.hpp"

afl::detail::FunctionOptimizer::FunctionOptimizer(std::shared_ptr<ResourceManager> resourceManager)
    : m_resourceManager(std::move(resourceManager))
{
    m_resourceManager->getPluginManager()->addObserver(this);
    reloadPluginFunctions();
}
afl::detail::FunctionOptimizer::FunctionOptimizer(const afl::detail::FunctionOptimizer& other)
    : FunctionOptimizer(other.m_resourceManager)
{}
afl::detail::FunctionOptimizer::~FunctionOptimizer()
{
    m_resourceManager->getPluginManager()->removeObserver(this);
}

afl::detail::FunctionOptimizer& afl::detail::FunctionOptimizer::operator=(const afl::detail::FunctionOptimizer& other)
{
    m_resourceManager->getPluginManager()->removeObserver(this);
    m_resourceManager = other.m_resourceManager;
    m_resourceManager->getPluginManager()->addObserver(this);
    reloadPluginFunctions();
    return *this;
}

std::string afl::detail::FunctionOptimizer::optimizeWithPlugins(std::string function)
{
    std::pair<bool, std::string> pair = {false, std::move(function)};
    do {
        pair = invokeCProcessStringPluginFunctions(pair.second, m_cApiPluginOptimizeStringFunctions);
        if(!pair.first)
            pair = invokeCppProcessStringPluginFunctions(std::move(pair.second), m_cppApiPluginOptimizeStringFunctions);
    } while(pair.first);
    return pair.second;
}
afl::SyntaxTree<afl::TokenGroup<std::string>> afl::detail::FunctionOptimizer::optimizeWithPlugins(SyntaxTree<TokenGroup<std::string>> syntaxTree)
{
    std::pair<bool, SyntaxTree<TokenGroup<std::string>>> pair = {false, std::move(syntaxTree)};
    do {
        pair = invokeCProcessSyntaxTreePluginFunctions(pair.second, m_cApiPluginOptimizeSyntaxTreeFunctions);
        if(!pair.first)
            pair = invokeCppProcessSyntaxTreePluginFunctions(std::move(pair.second), m_cppApiPluginOptimizeSyntaxTreeFunctions);
    } while(pair.first);
    return pair.second;
}
std::string afl::detail::FunctionOptimizer::optimizeFunction(std::string function)
{
    std::string tmp, tmp2;
    SyntaxTree<TokenGroup<std::string>> functionTree, tmpTree;
    do {
        tmp = function;
        do {
            tmp2 = function;
            function = optimizeWithPlugins(function);
        } while(tmp2 != function);
        functionTree = generateSyntaxTree(toGroupVector(m_resourceManager->getTokenManager()->stringToTokens(function)));
        do {
            tmpTree = functionTree;
            functionTree = optimizeWithPlugins(functionTree);
        } while(tmpTree != functionTree);
        auto tmpTokens = m_resourceManager->getTokenManager()->tokenGroupsToTokens(functionTree.root()->subtreeValues());
        function = afl::stringify(tmpTokens.begin(), tmpTokens.end(), "");
    } while(function != tmp);
    return function;
}

void afl::detail::FunctionOptimizer::pluginLoaded(apl::PluginManager* pluginManager, apl::Plugin* plugin)
{
    const apl::PluginFeatureInfo* const* featureInfos = plugin->getPluginInfo()->getFeatureInfos();
    for(size_t i = 0; i < plugin->getPluginInfo()->getFeatureCount(); ++i) {
        const apl::PluginFeatureInfo* featureInfo = featureInfos[i];
        if(std::strcmp(featureInfo->featureGroup, k_C_API_OptimizeFunctionStringFeatureGroupName) == 0)
            m_cApiPluginOptimizeStringFunctions.push_back(reinterpret_cast<CApiProcessStringPluginFunction>(featureInfo->functionPointer));
        else if(std::strcmp(featureInfo->featureGroup, k_C_API_OptimizeFunctionSyntaxTreeFeatureGroupName) == 0)
            m_cApiPluginOptimizeSyntaxTreeFunctions.push_back(reinterpret_cast<CApiProcessSyntaxTreePluginFunction>(featureInfo->functionPointer));
        else if(std::strcmp(featureInfo->featureGroup, k_CPP_API_OptimizeFunctionStringFeatureGroupName) == 0)
            m_cppApiPluginOptimizeStringFunctions.push_back(reinterpret_cast<CppApiProcessStringPluginFunction>(featureInfo->functionPointer));
        else if(std::strcmp(featureInfo->featureGroup, k_CPP_API_OptimizeFunctionSyntaxTreeFeatureGroupName) == 0)
            m_cppApiPluginOptimizeSyntaxTreeFunctions.push_back(reinterpret_cast<CppApiProcessSyntaxTreePluginFunction>(featureInfo->functionPointer));
    }
}
void afl::detail::FunctionOptimizer::pluginUnloaded(apl::PluginManager* pluginManager, apl::Plugin* plugin)
{
    const apl::PluginFeatureInfo* const* featureInfos = plugin->getPluginInfo()->getFeatureInfos();
    for(size_t i = 0; i < plugin->getPluginInfo()->getFeatureCount(); ++i) {
        const apl::PluginFeatureInfo* featureInfo = featureInfos[i];
        if(std::strcmp(featureInfo->featureGroup, k_C_API_OptimizeFunctionStringFeatureGroupName) == 0)
            m_cApiPluginOptimizeStringFunctions.erase(std::remove(m_cApiPluginOptimizeStringFunctions.begin(), m_cApiPluginOptimizeStringFunctions.end(), featureInfo->functionPointer), m_cApiPluginOptimizeStringFunctions.end());
        else if(std::strcmp(featureInfo->featureGroup, k_C_API_OptimizeFunctionSyntaxTreeFeatureGroupName) == 0)
            m_cApiPluginOptimizeSyntaxTreeFunctions.erase(std::remove(m_cApiPluginOptimizeSyntaxTreeFunctions.begin(), m_cApiPluginOptimizeSyntaxTreeFunctions.end(), featureInfo->functionPointer), m_cApiPluginOptimizeSyntaxTreeFunctions.end());
        else if(std::strcmp(featureInfo->featureGroup, k_CPP_API_OptimizeFunctionStringFeatureGroupName) == 0)
            m_cppApiPluginOptimizeStringFunctions.erase(std::remove(m_cppApiPluginOptimizeStringFunctions.begin(), m_cppApiPluginOptimizeStringFunctions.end(), featureInfo->functionPointer), m_cppApiPluginOptimizeStringFunctions.end());
        else if(std::strcmp(featureInfo->featureGroup, k_CPP_API_OptimizeFunctionSyntaxTreeFeatureGroupName) == 0)
            m_cppApiPluginOptimizeSyntaxTreeFunctions.erase(std::remove(m_cppApiPluginOptimizeSyntaxTreeFunctions.begin(), m_cppApiPluginOptimizeSyntaxTreeFunctions.end(), featureInfo->functionPointer), m_cppApiPluginOptimizeSyntaxTreeFunctions.end());
    }
}
void afl::detail::FunctionOptimizer::reloadPluginFunctions()
{
    m_cApiPluginOptimizeStringFunctions.clear();
    m_cApiPluginOptimizeSyntaxTreeFunctions.clear();
    m_cppApiPluginOptimizeStringFunctions.clear();
    m_cppApiPluginOptimizeSyntaxTreeFunctions.clear();
    apl::PluginManager* pluginManager = m_resourceManager->getPluginManager();
    for(auto plugin : pluginManager->getLoadedPlugins())
        pluginLoaded(pluginManager, plugin);
}

std::pair<bool, afl::SyntaxTree<afl::TokenGroup<std::string>>> afl::detail::invokeCProcessSyntaxTreePluginFunctions(const SyntaxTree<TokenGroup<std::string>>& syntaxTree, const std::vector<CApiProcessSyntaxTreePluginFunction>& pluginFunctions)
{
    CStringTokenGroupNode* tmpCSyntaxTree;
    CStringTokenGroupNode* formattedCSyntaxTree = convert(*syntaxTree.root());
    bool updatedAnyTime = false, updated;
    do {
        updated = false;
        for(CApiProcessSyntaxTreePluginFunction processFunction : pluginFunctions) {
            do {
                tmpCSyntaxTree = processFunction(formattedCSyntaxTree);
                if(tmpCSyntaxTree != nullptr && !equal(tmpCSyntaxTree, formattedCSyntaxTree)) {
                    free(formattedCSyntaxTree);
                    formattedCSyntaxTree = tmpCSyntaxTree;
                    updatedAnyTime = updated = true;
                } else if(tmpCSyntaxTree != nullptr) {
                    if(tmpCSyntaxTree != formattedCSyntaxTree) // only free returned tree if it is not the same object as passed tree (in case of const_cast)
                        free(tmpCSyntaxTree);
                    break; // breaks if returned syntax tree is equal to passed syntax tree
                }
            } while(tmpCSyntaxTree != nullptr); // breaks if content is equal for two cycles or plugin returns nullptr
        }
    } while(updated);
    return std::make_pair(updatedAnyTime, SyntaxTree<TokenGroup<std::string>>(convert(formattedCSyntaxTree)));
}

std::pair<bool, afl::SyntaxTree<afl::TokenGroup<std::string>>> afl::detail::invokeCppProcessSyntaxTreePluginFunctions(SyntaxTree<TokenGroup<std::string>> syntaxTree, const std::vector<CppApiProcessSyntaxTreePluginFunction>& pluginFunctions)
{
    SyntaxTree<TokenGroup<std::string>> tmp;
    bool updatedAnyTime = false, updated;
    do {
        updated = false;
        for(const CppApiProcessSyntaxTreePluginFunction processFunction : pluginFunctions) {
            do {
                tmp = processFunction(syntaxTree);
                if (tmp.empty() || tmp == syntaxTree) {
                    break;
                } else {
                    syntaxTree = std::move(tmp);
                    updatedAnyTime = updated = true;
                }
            } while(!syntaxTree.empty());
        }
    } while(updated);
    return std::make_pair(updatedAnyTime, std::move(syntaxTree));
}

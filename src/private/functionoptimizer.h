#ifndef AFUNCTIONLIBRARY_FUNCTIONOPTIMIZER_H
#define AFUNCTIONLIBRARY_FUNCTIONOPTIMIZER_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include "AFunctionLibrary/implementation/include/AFunctionLibrary/syntaxtree.h"
#include "AFunctionLibrary/implementation/cstringtokensyntaxtree.hpp"


#include "functionformatter.h"

namespace afl
{
    namespace detail
    {
        typedef CStringTokenGroupNode*(*CApiProcessSyntaxTreePluginFunction)(const CStringTokenGroupNode*);
        typedef SyntaxTree<TokenGroup<std::string>>(*CppApiProcessSyntaxTreePluginFunction)(SyntaxTree<TokenGroup<std::string>>);

        class AFUNCTIONLIBRARY_NO_EXPORT FunctionOptimizer : public apl::PluginManagerObserver
        {
        public:
            explicit FunctionOptimizer(std::shared_ptr<ResourceManager> resourceManager);
            FunctionOptimizer(const FunctionOptimizer& other);
            virtual ~FunctionOptimizer();

            FunctionOptimizer& operator=(const FunctionOptimizer& other);

            std::string optimizeWithPlugins(std::string function);
            SyntaxTree<TokenGroup<std::string>> optimizeWithPlugins(SyntaxTree<TokenGroup<std::string>> syntaxTree);
            std::string optimizeFunction(std::string function);

            void pluginLoaded(apl::PluginManager* pluginManager, apl::Plugin* plugin) override;
            void pluginUnloaded(apl::PluginManager* pluginManager, apl::Plugin* plugin) override;
            void reloadPluginFunctions();

        protected:
            std::shared_ptr<ResourceManager> m_resourceManager;

        private:
            std::vector<CApiProcessStringPluginFunction> m_cApiPluginOptimizeStringFunctions;
            std::vector<CApiProcessSyntaxTreePluginFunction> m_cApiPluginOptimizeSyntaxTreeFunctions;
            std::vector<CppApiProcessStringPluginFunction> m_cppApiPluginOptimizeStringFunctions;
            std::vector<CppApiProcessSyntaxTreePluginFunction> m_cppApiPluginOptimizeSyntaxTreeFunctions;
        };

        AFUNCTIONLIBRARY_NO_EXPORT std::pair<bool, SyntaxTree<TokenGroup<std::string>>> invokeCProcessSyntaxTreePluginFunctions(const SyntaxTree<TokenGroup<std::string>>& syntaxTree, const std::vector<CApiProcessSyntaxTreePluginFunction>& pluginFunctions);
        AFUNCTIONLIBRARY_NO_EXPORT std::pair<bool, SyntaxTree<TokenGroup<std::string>>> invokeCppProcessSyntaxTreePluginFunctions(SyntaxTree<TokenGroup<std::string>> syntaxTree, const std::vector<CppApiProcessSyntaxTreePluginFunction>& pluginFunctions);
    }
}

#endif //AFUNCTIONLIBRARY_FUNCTIONOPTIMIZER_H

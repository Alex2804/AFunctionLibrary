#ifndef AFUNCTIONLIBRARY_FUNCTIONFORMATTER_H
#define AFUNCTIONLIBRARY_FUNCTIONFORMATTER_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include <string>
#include <vector>
#include <regex>
#include <memory>

#include "resourcemanager.h"

namespace afl
{
    namespace detail
    {
        typedef char*(*formatFunctionPluginFunction)(const char*);

        class AFUNCTIONLIBRARY_NO_EXPORT FunctionFormatter
        {
        public:
            explicit FunctionFormatter(std::shared_ptr<ResourceManager> resourceManager);
            FunctionFormatter(const FunctionFormatter& other);
            FunctionFormatter(FunctionFormatter&& other) noexcept;
            ~FunctionFormatter();

            FunctionFormatter& operator=(const FunctionFormatter& other);
            FunctionFormatter& operator=(FunctionFormatter&& other) noexcept;

            void reloadTokens();

            std::string formatFunction(std::string function);

            std::vector<std::string> splitIntoTokens(std::string string);

        private:
            void registerCallbackFunction();

            ResourceManagerCallbackFunction function;
            std::shared_ptr<ResourceManager> m_resourceManager;
            std::vector<std::pair<const apl::Plugin*, std::vector<formatFunctionPluginFunction>>> m_formatFunctions;
            std::vector<std::shared_ptr<const TokenWrapper<std::string>>> m_uniqueTokens, m_notUniqueTokens;
        };
    }
}

#endif //AFUNCTIONLIBRARY_FUNCTIONFORMATTER_H

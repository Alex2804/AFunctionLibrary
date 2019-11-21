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
            FunctionFormatter(const FunctionFormatter& other) = default;
            FunctionFormatter(FunctionFormatter&& other) noexcept = default;
            ~FunctionFormatter() = default;

            FunctionFormatter& operator=(const FunctionFormatter& other) = default;
            FunctionFormatter& operator=(FunctionFormatter&& other) noexcept = default;

            void reloadResources();

            std::string formatFunction(std::string function);

            std::vector<std::string> splitIntoTokens(std::string string);

        private:
            std::shared_ptr<ResourceManager> m_resourceManager;
            std::vector<std::shared_ptr<const TokenWrapper<std::string>>> m_uniqueTokens, m_notUniqueTokens;
        };
    }
}

#endif //AFUNCTIONLIBRARY_FUNCTIONFORMATTER_H

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

            void reloadPluginFormatFunctions();

            std::string formatFunction(std::string function);

            std::vector<std::string> splitIntoTokens(std::string string);

        private:
            std::shared_ptr<ResourceManager> m_resourceManager;
            std::vector<const apl::PluginFeatureInfo*> m_pluginFormatFunctions;
        };
    }
}

#endif //AFUNCTIONLIBRARY_FUNCTIONFORMATTER_H

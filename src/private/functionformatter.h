#ifndef AFUNCTIONLIBRARY_FUNCTIONFORMATTER_H
#define AFUNCTIONLIBRARY_FUNCTIONFORMATTER_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include <string>
#include <vector>
#include <memory>

#include "APluginSDK/pluginapi.h"

#include "resourcemanager.h"

namespace afl
{
    namespace detail
    {
        typedef CString*(*CApiProcessStringPluginFunction)(const char*);
        typedef std::string(*CppApiProcessStringPluginFunction)(std::string);

        class AFUNCTIONLIBRARY_NO_EXPORT FunctionFormatter
        {
        public:
            explicit FunctionFormatter(std::shared_ptr<ResourceManager> resourceManager);
            FunctionFormatter(const FunctionFormatter& other) = default;
            FunctionFormatter(FunctionFormatter&& other) noexcept = default;
            virtual ~FunctionFormatter() = default;

            FunctionFormatter& operator=(const FunctionFormatter& other) = default;
            FunctionFormatter& operator=(FunctionFormatter&& other) noexcept = default;

            void reloadPluginFormatFunctions();

            std::string replaceAliases(std::string function);
            std::string formatWithPlugins(std::string function);
            std::string formatFunction(std::string function);

        protected:
            std::shared_ptr<ResourceManager> m_resourceManager;

        private:
            std::vector<CApiProcessStringPluginFunction> m_cApiPluginFormatStringFunctions;
            std::vector<CppApiProcessStringPluginFunction> m_cppApiPluginFormatStringFunctions;
        };

        AFUNCTIONLIBRARY_NO_EXPORT std::pair<bool, std::string> invokeCProcessStringPluginFunctions(const std::string& function, const std::vector<CApiProcessStringPluginFunction>& pluginFunctions);
        AFUNCTIONLIBRARY_NO_EXPORT std::pair<bool, std::string> invokeCppProcessStringPluginFunctions(std::string function, const std::vector<CppApiProcessStringPluginFunction>& pluginFunctions);
    }
}

#endif //AFUNCTIONLIBRARY_FUNCTIONFORMATTER_H

#ifndef AFUNCTIONLIBRARY_STRINGTOKENMANAGER_H
#define AFUNCTIONLIBRARY_STRINGTOKENMANAGER_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>

#include "APluginLibrary/pluginmanager.h"

#include "AFunctionLibrary/token.h"

namespace afl
{
    namespace detail
    {
        class AFUNCTIONLIBRARY_NO_EXPORT StringTokenManager
        {
        public:
            static std::shared_ptr<Token<std::string>> getToken(std::string value, bool createIfNotExist = false);
            static void removeToken(const std::string& value);
            static void addToken(std::unique_ptr<Token<std::string>> token);

            static std::unordered_map<std::string, std::shared_ptr<Token<std::string>>> getTokens();

        private:
            static std::unordered_map<std::string, std::shared_ptr<Token<std::string>>> tokens;
            static std::mutex mutex;
        };
    }
}


#endif //AFUNCTIONLIBRARY_STRINGTOKENMANAGER_H

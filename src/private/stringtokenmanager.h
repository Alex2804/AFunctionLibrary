#ifndef AFUNCTIONLIBRARY_STRINGTOKENMANAGER_H
#define AFUNCTIONLIBRARY_STRINGTOKENMANAGER_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>
#include <vector>

#include "AFunctionLibrary/token.h"

namespace afl
{
    namespace detail
    {
        class AFUNCTIONLIBRARY_NO_EXPORT StringTokenManager
        {
        public:
            StringTokenManager() = default;
            StringTokenManager(const StringTokenManager& other) = default;
            StringTokenManager(StringTokenManager&& other) = default;
            ~StringTokenManager() = default;

            StringTokenManager& operator=(const StringTokenManager& other) = default;
            StringTokenManager& operator=(StringTokenManager&& other) = default;

            std::shared_ptr<Token<std::string>> getToken(std::string value, bool createIfNotExist = false);
            static std::unique_ptr<Token<std::string>> createToken(std::string value);

            void addToken(std::unique_ptr<Token<std::string>> token);
            void removeToken(const std::string& value);

            std::unordered_map<std::string, std::shared_ptr<Token<std::string>>> getTokens() const;

        private:
            std::unordered_map<std::string, std::shared_ptr<Token<std::string>>> tokens;
        };
    }
}


#endif //AFUNCTIONLIBRARY_STRINGTOKENMANAGER_H

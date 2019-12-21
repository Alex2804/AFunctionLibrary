#ifndef AFUNCTIONLIBRARY_TOKENMANAGER_H
#define AFUNCTIONLIBRARY_TOKENMANAGER_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "APluginLibrary/plugin.h"
#include "APluginSDK/plugininfos.h"

#include "AFunctionLibrary/implementation/cstringtoken.hpp"
#include "tokenextras.h"

namespace afl
{
    namespace detail
    {
        typedef CStringToken*(*cApiCreateTokenPluginFunction)(const char*);
        typedef std::shared_ptr<afl::Token<std::string>>(*cppApiCreateTokenPluginFunction)(const std::string&);
        typedef CStringTokenAliases*(*cApiCreateTokenAliasesPluginFunction)(const char*, afl::TokenAliasType);
        typedef std::vector<TokenAliases<std::string>>(*cppApiCreateTokenAliasesPluginFunction)(const std::string&);

        class AFUNCTIONLIBRARY_NO_EXPORT TokenManager
        {
        public:
            TokenManager() = default;
            TokenManager(const TokenManager& other) = default;
            TokenManager(TokenManager&& other) noexcept = default;
            ~TokenManager() = default;

            TokenManager& operator=(const TokenManager& other) = default;
            TokenManager& operator=(TokenManager&& other) noexcept = default;

            void addPluginFeatures(const apl::Plugin* plugin);
            void removePluginFeatures(const apl::Plugin* plugin);

            bool addToken(std::shared_ptr<TokenPtrBundle < std::string>> tokenBundle, std::string refPath);
            void removeToken(const std::string& value, const std::string& refPath);
            void removeReferences(const std::string& refPath);

            std::pair<std::shared_ptr<TokenPtrBundle<std::string>>, std::string> createToken(const std::string& value, bool createAliases = true) const;
            std::vector<TokenAliases<std::string>> createAliases(const std::string& value) const;

            std::shared_ptr<const TokenPtrBundle<std::string>> getToken(const std::string& value) const;
            std::shared_ptr<const TokenPtrBundle<std::string>> getToken(const std::string& value, bool createIfNotExist);
            std::vector<std::shared_ptr<const TokenPtrBundle<std::string>>> getTokens() const;

            template<typename T> // template to accept function pointer and std::function
            std::vector<std::shared_ptr<const TokenPtrBundle<std::string>>> filterTokens(const T& filterFunction) const;

            static bool isUnique(const afl::Token<std::string>* token);
            static bool isNotUnique(const afl::Token<std::string>* token);

        public:
            std::unordered_map<std::string, std::pair<std::shared_ptr<TokenPtrBundle<std::string>>, size_t>> m_uniqueTokens, m_notUniqueTokens;
            std::unordered_map<std::string, std::vector<std::string>> m_pathTokenValueRefs;
            std::vector<std::tuple<const apl::Plugin*, std::vector<cApiCreateTokenPluginFunction>, std::vector<cApiCreateTokenAliasesPluginFunction>, std::vector<cppApiCreateTokenPluginFunction>, std::vector<cppApiCreateTokenAliasesPluginFunction>>> m_pluginFunctions;
        };

        AFUNCTIONLIBRARY_NO_EXPORT std::vector<std::shared_ptr<const Token<std::string>>> stringToTokens(TokenManager* tokenManager, std::string string);
        AFUNCTIONLIBRARY_NO_EXPORT std::string toFunctionString(TokenManager* tokenManager, std::vector<TokenGroup<std::string>> tokenGroups);
    }
}

#include "implementation/tokenmanager.tpp"

#endif //AFUNCTIONLIBRARY_TOKENMANAGER_H

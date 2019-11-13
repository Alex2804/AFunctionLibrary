#ifndef AFUNCTIONLIBRARY_RESOURCEMANAGER_H
#define AFUNCTIONLIBRARY_RESOURCEMANAGER_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include <set>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "APluginLibrary/pluginmanager.h"

#include "AFunctionLibrary/token.h"

#include "tokenextras.h"
#include "resourceparser.h"

namespace afl
{
    namespace detail
    {
        typedef CStringToken*(*createTokenPluginFunction)(const char*);
        typedef CStringTokenAliases*(*createTokenAliasesPluginFunction)(const char*, afl::TokenAliasType);

        enum class ResourceType
        {
            Plugin,
            Extension
        };

        class AFUNCTIONLIBRARY_NO_EXPORT ResourceManager
        {
        public:
            ResourceManager();
            ResourceManager(const ResourceManager& other);
            ResourceManager(ResourceManager&& other) noexcept;
            ~ResourceManager();

            ResourceManager& operator=(const ResourceManager& other);
            ResourceManager& operator=(ResourceManager&& other) noexcept;

            bool load(ResourceType type, const std::string& path);
            size_t loadDirectory(ResourceType type, const std::string& path, bool recursive);
            void unload(ResourceType type, std::string path);

            apl::PluginManager& getPluginManager();

            std::shared_ptr<const TokenWrapper<std::string>> getToken(const std::string& value, bool createIfNotExist = true);
            std::vector<std::shared_ptr<const TokenWrapper<std::string>>> getTokens();

        protected:
            bool readExtension(std::string path);
            std::shared_ptr<const TokenWrapper<std::string>> createToken(const std::string& value);

            void updatePluginFunctions();

        private:
            std::unordered_map<std::string, std::pair<std::shared_ptr<TokenWrapper<std::string>>, size_t>> tokens;
            std::unordered_map<std::string, std::set<std::string>> pathTokenRefs;
            apl::PluginManager pluginManager;
            std::unordered_map<const apl::Plugin*, std::pair<std::vector<createTokenPluginFunction>, std::vector<createTokenAliasesPluginFunction>>> pluginFunctions;
        };
    }
}


#endif //AFUNCTIONLIBRARY_RESOURCEMANAGER_H

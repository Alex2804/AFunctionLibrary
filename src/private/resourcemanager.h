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
#include "tokenmanager.h"

namespace afl
{
    namespace detail
    {
        enum class ResourceType
        {
            Plugin,
            Extension
        };

        typedef void(*ResourceManagerCallbackFunction)(const std::string& path, ResourceType type);
        enum class ResourceManagerCallbackType
        {
            Load,
            Unload,
            Load_Unload
        };

        AFUNCTIONLIBRARY_NO_EXPORT std::string getFullPathName(std::string path, afl::detail::ResourceType type);
        AFUNCTIONLIBRARY_NO_EXPORT std::vector<std::string> getDirectoryFiles(const std::string& path, const std::string& fileExtension, bool recursive);

        class AFUNCTIONLIBRARY_NO_EXPORT ResourceManager
        {
        public:
            ResourceManager();
            ResourceManager(const ResourceManager& other);
            ResourceManager(ResourceManager&& other) noexcept;
            ~ResourceManager();

            ResourceManager& operator=(const ResourceManager& other);
            ResourceManager& operator=(ResourceManager&& other) noexcept;

            bool load(std::string path, ResourceType type);
            size_t loadDirectory(const std::string& path, ResourceType type, bool recursive);
            void unload(const std::string& path, ResourceType type);

            void addCallbackFunction(ResourceManagerCallbackFunction function, ResourceManagerCallbackType type);
            void removeCallbackFunction(ResourceManagerCallbackFunction function, ResourceManagerCallbackType type);

            const apl::PluginManager* getPluginManager() const;
            TokenManager* getTokenManager();

        private:
            apl::PluginManager* m_pluginManager;
            TokenManager* m_tokenManager;
            std::vector<ResourceManagerCallbackFunction> loadCallbackFunctions, unloadCallbackFunctions;
        };
    }
}


#endif //AFUNCTIONLIBRARY_RESOURCEMANAGER_H

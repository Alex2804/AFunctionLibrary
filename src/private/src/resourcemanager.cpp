#include "../resourcemanager.h"

#include <cstring>
#include <algorithm>

#include "tinydir/tinydir.h"
#include "pugixml.hpp"

namespace
{
    constexpr const char *kExtensionsFileExtension = "xml";

    inline void invokeResourceManagerCallbacks(const std::vector<afl::detail::ResourceManagerCallbackFunction>& functions,
                                        const std::string& path, afl::detail::ResourceType type)
    {
        for(afl::detail::ResourceManagerCallbackFunction function : functions) {
            function(path, type);
        }
    }
}

std::vector<std::string> afl::detail::getDirectoryFiles(const std::string& path, const std::string& fileExtension, bool recursive)
{
    tinydir_dir dir;
    tinydir_file file;
    std::string filePath;
    std::vector<std::string> files, tmpFiles;

    tinydir_open(&dir, path.c_str());
    while (dir.has_next)
    {
        tinydir_readfile(&dir, &file);
        filePath = file.path;
        if(strcmp(file.name, ".") != 0 && strcmp(file.name, "..") != 0) {
            if (file.is_dir && recursive) {
                tmpFiles = getDirectoryFiles(filePath, fileExtension, recursive);
                files.insert(files.end(), tmpFiles.begin(), tmpFiles.end());
            } else if (!file.is_dir && fileExtension == file.extension) {
                files.push_back(filePath);
            }
        }
        tinydir_next(&dir);
    }
    tinydir_close(&dir);

    return files;
}

std::string afl::detail::getFullPathName(std::string path, afl::detail::ResourceType type) {
    if(type == afl::detail::ResourceType::Plugin) {
        return std::move(path).append(".").append(apl::LibraryLoader::libExtension());
    } else if(type == afl::detail::ResourceType::Extension) {
        return std::move(path).append(".").append(kExtensionsFileExtension);
    } else {
        throw std::runtime_error("Unimplemented afl::detail::ResourceType!");
    }
}

afl::detail::ResourceManager::ResourceManager()
    : m_pluginManager(new apl::PluginManager())
    , m_tokenManager(new TokenManager())
{}
afl::detail::ResourceManager::ResourceManager(const afl::detail::ResourceManager &other)
    : m_pluginManager(new apl::PluginManager(*other.m_pluginManager))
    , m_tokenManager(new TokenManager(*other.m_tokenManager))
{}
afl::detail::ResourceManager::ResourceManager(afl::detail::ResourceManager &&other) noexcept
    : m_pluginManager(other.m_pluginManager)
    , m_tokenManager(other.m_tokenManager)
{
    other.m_pluginManager = nullptr;
    other.m_tokenManager = nullptr;
}
afl::detail::ResourceManager::~ResourceManager()
{
    delete m_pluginManager;
    delete m_tokenManager;
}

afl::detail::ResourceManager &afl::detail::ResourceManager::operator=(const afl::detail::ResourceManager &other)
{
    *m_pluginManager = *other.m_pluginManager;
    *m_tokenManager = *other.m_tokenManager;
    return *this;
}
afl::detail::ResourceManager &afl::detail::ResourceManager::operator=(afl::detail::ResourceManager &&other) noexcept
{
    using std::swap;
    swap(m_pluginManager, other.m_pluginManager);
    swap(m_tokenManager, other.m_tokenManager);
    return *this;
}


bool afl::detail::ResourceManager::load(std::string path, ResourceType type)
{
    if(type == ResourceType::Plugin) {
        apl::Plugin* plugin;
        if((plugin = m_pluginManager->load(path)) != nullptr) {
            m_tokenManager->addPluginFeatures(plugin);
            return true;
        }
    } else if(type == ResourceType::Extension) {
        std::string fullPath = getFullPathName(std::move(path), ResourceType::Extension);
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(fullPath.c_str());
        if(!result)
            return false;
        std::vector<std::shared_ptr<TokenWrapper<std::string>>> tokensVector;
        parseExtensionRecursive(doc.root(), tokensVector);

        for(const std::shared_ptr<TokenWrapper<std::string>>& tokenWrapper : tokensVector)
            m_tokenManager->addToken(tokenWrapper, fullPath);
        return true;
    } else {
        throw std::runtime_error("Unimplemented afl::detail::ResourceType!");
    }
    return false;
}
size_t afl::detail::ResourceManager::loadDirectory(const std::string& path, ResourceType type, bool recursive)
{
    size_t count = 0;
    if(type == ResourceType::Plugin) {
        std::vector<apl::Plugin*> plugins = m_pluginManager->loadDirectory(path, recursive);
        count = plugins.size();
        for (apl::Plugin *plugin : plugins)
            m_tokenManager->addPluginFeatures(plugin);
    } else if(type == ResourceType::Extension) {
        size_t extensionSize = std::strlen(kExtensionsFileExtension);
        for(const std::string& p : getDirectoryFiles(path, kExtensionsFileExtension, recursive)) {
            if(load(p.substr(0, p.size() - extensionSize - 1), ResourceType::Extension))
                ++count;
        }
    } else {
        throw std::runtime_error("Unimplemented afl::detail::ResourceType!");
    }
    return count;
}
void afl::detail::ResourceManager::unload(const std::string& path, ResourceType type)
{
    std::string fullPath = getFullPathName(path, type);
    m_tokenManager->removeReferences(fullPath);
    if(type == ResourceType::Plugin) {
        apl::Plugin* plugin = m_pluginManager->getLoadedPlugin(path);
        m_tokenManager->removePluginFeatures(plugin);
        m_pluginManager->unload(plugin);
    } else if(type != ResourceType::Extension) {
        throw std::runtime_error("Unimplemented afl::detail::ResourceType!");
    }
}

const apl::PluginManager* afl::detail::ResourceManager::getPluginManager() const
{
    return m_pluginManager;
}

afl::detail::TokenManager* afl::detail::ResourceManager::getTokenManager()
{
    return m_tokenManager;
}

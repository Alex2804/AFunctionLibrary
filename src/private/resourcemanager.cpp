#include "resourcemanager.h"

#include <cstring>
#include <algorithm>

#include "tinydir/tinydir.h"
#include "pugixml.hpp"

namespace
{
    constexpr const char* kExtensionsFileExtension = "xml";

    std::vector<std::string> getDirectoryFiles(const std::string& path, const std::string& fileExtension, bool recursive)
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

    std::string getFullPathName(std::string path, afl::detail::ResourceType type) {
        if(type == afl::detail::ResourceType::Plugin) {
            return std::move(path).append(".").append(apl::LibraryLoader::libExtension());
        } else if(type == afl::detail::ResourceType::Extension) {
            return std::move(path).append(".").append(kExtensionsFileExtension);
        } else {
            throw std::runtime_error("Unimplemented afl::detail::ResourceType!");
        }
    }
}

afl::detail::ResourceManager::ResourceManager() = default;
afl::detail::ResourceManager::ResourceManager(const ResourceManager& other) = default;
afl::detail::ResourceManager::ResourceManager(ResourceManager&& other) noexcept = default;
afl::detail::ResourceManager::~ResourceManager() = default;

afl::detail::ResourceManager& afl::detail::ResourceManager::operator=(const ResourceManager& other) = default;
afl::detail::ResourceManager& afl::detail::ResourceManager::operator=(ResourceManager&& other) noexcept = default;

bool afl::detail::ResourceManager::load(afl::detail::ResourceType type, const std::string& path)
{
    if(type == ResourceType::Plugin) {
        if(pluginManager.load(path)) {
            updatePluginFunctions();
            return true;
        }
    } else if(type == ResourceType::Extension) {
        return readExtension(path);
    } else {
        throw std::runtime_error("Unimplemented afl::detail::ResourceType!");
    }
    return false;
}

size_t afl::detail::ResourceManager::loadDirectory(afl::detail::ResourceType type, const std::string &path, bool recursive)
{
    size_t count = 0;
    if(type == ResourceType::Plugin) {
        count = pluginManager.loadDirectory(path, recursive);
        if(count > 0)
            updatePluginFunctions();
    } else if(type == ResourceType::Extension) {
        size_t extensionSize = std::strlen(kExtensionsFileExtension);
        for(const std::string& p : getDirectoryFiles(path, kExtensionsFileExtension, recursive)) {
            if(readExtension(p.substr(0, p.size() - extensionSize - 1)))
                ++count;
        }
    } else {
        throw std::runtime_error("Unimplemented afl::detail::ResourceType!");
    }
    return count;
}
void afl::detail::ResourceManager::unload(afl::detail::ResourceType type, std::string path)
{
    if(type == ResourceType::Plugin) {
        pluginManager.unload(path);
    } else if(type == ResourceType::Extension) {

    } else {
        throw std::runtime_error("Unimplemented afl::detail::ResourceType!");
    }
    auto iterator = pathTokenRefs.find(getFullPathName(std::move(path), type));
    if(iterator != pathTokenRefs.end()) {
        for(const std::string& value : iterator->second) {
            auto tokenIterator = tokens.find(value);
            if(tokenIterator != tokens.end() && 0 == --(tokenIterator->second.second)) {
                tokens.erase(tokenIterator);
            }
        }
        pathTokenRefs.erase(iterator);
    }
}

apl::PluginManager& afl::detail::ResourceManager::getPluginManager()
{
    return pluginManager;
}

std::shared_ptr<const afl::detail::TokenWrapper<std::string>> afl::detail::ResourceManager::getToken(const std::string &value, bool createIfNotExist)
{
    auto iterator = tokens.find(value);
    if(iterator != tokens.end()) {
        return iterator->second.first;
    } else if(createIfNotExist) {
        return createToken(value);
    }
    return std::shared_ptr<const TokenWrapper<std::string>>();
}
std::vector<std::shared_ptr<const afl::detail::TokenWrapper<std::string>>> afl::detail::ResourceManager::getTokens()
{
    std::vector<std::shared_ptr<const TokenWrapper<std::string>>> tokenVector;
    tokenVector.reserve(tokens.size());
    for(const auto& tokenPair : tokens) {
        tokenVector.push_back(tokenPair.second.first);
    }
    return tokenVector;
}

bool afl::detail::ResourceManager::readExtension(std::string path)
{
    std::string fullPath = getFullPathName(std::move(path), ResourceType::Extension);
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fullPath.c_str());
    if(!result)
        return false;
    std::vector<std::shared_ptr<TokenWrapper<std::string>>> tokensVector;
    parseExtensionRecursive(doc.root(), tokensVector);

    if(!tokensVector.empty()) {
        auto pathTokenRefsIterator = pathTokenRefs.find(fullPath);
        if (pathTokenRefsIterator == pathTokenRefs.end())
            pathTokenRefsIterator = pathTokenRefs.emplace(fullPath, std::set<std::string>{}).first; // now iterator is guaranteed to point to an valid element

        for (const std::shared_ptr<TokenWrapper<std::string>> &tokenWrapper : tokensVector) {
            if (tokens.find(tokenWrapper->token->value) == tokens.end()) { // only insert if token with this value does not exist already
                tokens.emplace(tokenWrapper->token->value, std::pair<std::shared_ptr<TokenWrapper<std::string>>, size_t>{tokenWrapper, 1});
                pathTokenRefsIterator->second.emplace(tokenWrapper->token->value); // create ref
            } else if(pathTokenRefsIterator->second.emplace(tokenWrapper->token->value).second) {
                tokens.at(tokenWrapper->token->value).second += 1; // wouldn't be in else if key not exists
            }
        }
    }
    return true;
}

std::shared_ptr<const afl::detail::TokenWrapper<std::string>> afl::detail::ResourceManager::createToken(const std::string &value)
{
    std::shared_ptr<Token<std::string>> createdToken;
    std::string createdTokenPluginPath;
    std::vector<TokenAliases<std::string>> createdAliases;

    const char* cValue = value.c_str();

    CStringToken* cToken = nullptr;
    CStringTokenAliases* cTokenAliases = nullptr;
    std::vector<TokenAliasType> aliasTypes = {TokenAliasType::String, TokenAliasType::Regex};
    for(TokenAliasType type : aliasTypes) {
        createdAliases.emplace_back();
        createdAliases.back().type = type;
    }

    const apl::Plugin* plugin;
    std::vector<createTokenPluginFunction> createTokenFunctions;
    std::vector<createTokenAliasesPluginFunction> createTokenAliasesFunctions;
    for(std::pair<const apl::Plugin*, std::pair<std::vector<createTokenPluginFunction>, std::vector<createTokenAliasesPluginFunction>>> pair : pluginFunctions)
    {
        plugin = pair.first;
        if(plugin->isLoaded()) {
            // create token
            createTokenFunctions = pair.second.first;
            if(createdToken == nullptr) { // create token only if none is created by any plugin yet
                for (createTokenPluginFunction function : createTokenFunctions) {
                    cToken = function(cValue);
                    if (cToken != nullptr) {
                        createdTokenPluginPath = getFullPathName(plugin->getPath(), ResourceType::Plugin);
                        createdToken = std::make_shared<Token<std::string>>(value, cToken->type, cToken->precedence,
                                                                         cToken->parameterCount, cToken->associativity);
                        if(!plugin->freeMemory(cToken))
                            throw std::runtime_error("Couldn't free cToken in afl::detail::ResourceManager::createToken!");
                        break;
                    }
                }
            }

            // get aliases to token
            createTokenAliasesFunctions = pair.second.second;
            for(size_t i = 0; i < aliasTypes.size(); i++) {
                for(createTokenAliasesPluginFunction function : createTokenAliasesFunctions) {
                    cTokenAliases = function(cValue, aliasTypes[i]);
                    if(cTokenAliases != nullptr) {
                        std::copy(cTokenAliases->aliases, cTokenAliases->aliases + cTokenAliases->aliasesCount, std::inserter(createdAliases[i].aliases, createdAliases[i].aliases.end()));
                        for(size_t j = 0; j < cTokenAliases->aliasesCount; j++) {
                            if (!plugin->freeMemory(cTokenAliases->aliases[j]))
                                throw std::runtime_error(std::string("Couldn't free cTokenAliases::aliases[") + afl::stringify(j) + "] in afl::detail::ResourceManager::createToken!");
                        }
                        if (!plugin->freeMemory(cTokenAliases->aliases))
                            throw std::runtime_error("Couldn't free cTokenAliases::aliases in afl::detail::ResourceManager::createToken!");
                        if(!plugin->freeMemory(cTokenAliases))
                            throw std::runtime_error("Couldn't free cTokenAliases in afl::detail::ResourceManager::createToken!");
                    }
                }
            }
        }
    }

    if(createdToken != nullptr) {
        // create token wrapper and insert it into the map
        createdAliases.erase(std::remove_if(createdAliases.begin(), createdAliases.end(), [](const TokenAliases<std::string>& a){ return a.aliases.empty(); }), createdAliases.end());
        auto tokenWrapper = std::make_shared<TokenWrapper<std::string>>(createdToken, createdAliases);
        tokens.emplace(tokenWrapper->token->value, std::pair<std::shared_ptr<TokenWrapper<std::string>>, size_t>{tokenWrapper, 1});
        // create reference between plugin and token
        auto iterator = pathTokenRefs.find(createdTokenPluginPath);
        if(iterator == pathTokenRefs.end()) {
            iterator = pathTokenRefs.emplace(createdTokenPluginPath, std::set<std::string>{tokenWrapper->token->value}).first;
        }
        iterator->second.emplace(tokenWrapper->token->value);
        return tokenWrapper;
    }
    return std::shared_ptr<const TokenWrapper<std::string>>(nullptr);
}

void afl::detail::ResourceManager::updatePluginFunctions()
{
    std::vector<createTokenPluginFunction> createTokenFunctions;
    std::vector<createTokenAliasesPluginFunction> createTokenAliasesFunctions;
    const apl::PluginFeatureInfo* info;
    for (const apl::Plugin *plugin : pluginManager.getLoadedPlugins()) {
        auto iterator = pluginFunctions.find(plugin);
        if(plugin->isLoaded() && iterator == pluginFunctions.end()) {
            for(size_t i = 0; i < plugin->getFeatureCount(); i++) {
                info = plugin->getFeatureInfo(i);
                if(strcmp(info->featureGroup, kCreateTokenFeatureGroupName) == 0
                   && strcmp(info->returnType, "afl::CStringToken*") == 0
                   && strcmp(info->parameterTypes, "const char*") == 0
                   && info->functionPointer != nullptr)
                {
                    createTokenFunctions.push_back(reinterpret_cast<createTokenPluginFunction>(info->functionPointer));
                }
                else if(strcmp(info->featureGroup, kCreateTokenAliasesFeatureGroupName) == 0
                        && strcmp(info->returnType, "afl::CStringTokenAliases*") == 0
                        && strcmp(info->parameterTypes, "const char*, afl::TokenAliasType") == 0
                        && info->functionPointer != nullptr)
                {
                    createTokenAliasesFunctions.push_back(reinterpret_cast<createTokenAliasesPluginFunction>(info->functionPointer));
                }
            }
        } else if(!plugin->isLoaded() && iterator != pluginFunctions.end()) {
            pluginFunctions.erase(iterator);
        }
        if(!createTokenFunctions.empty() || !createTokenAliasesFunctions.empty()) {
            std::pair<std::vector<createTokenPluginFunction>, std::vector<createTokenAliasesPluginFunction>> pair = {
                    createTokenFunctions, createTokenAliasesFunctions};
            pluginFunctions.emplace(plugin, pair);
            createTokenFunctions.clear();
            createTokenAliasesFunctions.clear();
        }
    }
}
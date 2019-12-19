#ifndef AFUNCTIONLIBRARY_RESOURCEPARSER_H
#define AFUNCTIONLIBRARY_RESOURCEPARSER_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

#include "pugixml.hpp"

#include "tokenextras.h"

namespace afl
{
    namespace detail
    {

        AFUNCTIONLIBRARY_NO_EXPORT TokenAssociativity stringToTokenAssociativity(const std::string& string, TokenAssociativity defaultValue);
        AFUNCTIONLIBRARY_NO_EXPORT TokenType stringToTokenType(const std::string& string, TokenType defaultValue);

        AFUNCTIONLIBRARY_NO_EXPORT void parseExtensionRecursive(const pugi::xml_node& node, std::vector<std::shared_ptr<TokenPtrBundle<std::string>>>& tokens);

        AFUNCTIONLIBRARY_NO_EXPORT void parseTokenAliases(std::string aliasString, TokenInstanceBundle<std::string> &bundle, TokenAliasType type);
        template<typename T>
        AFUNCTIONLIBRARY_NO_EXPORT void parseTokenField(const pugi::xml_node& node, const std::string& fieldName, const T& fieldValue,
                                                        TokenInstanceBundle<std::string>& bundle,
                                                        std::vector<std::shared_ptr<TokenPtrBundle<std::string>>>& tokens,
                                                        std::unordered_set<std::string>& valueSet);
        AFUNCTIONLIBRARY_NO_EXPORT void parseTokensRecursive(const pugi::xml_node& node, TokenInstanceBundle<std::string> bundle,
                                                             std::vector<std::shared_ptr<TokenPtrBundle<std::string>>>& tokens,
                                                             std::unordered_set<std::string>& valueSet);
    }
}

#include "implementation/resourceparser.tpp"

#endif //AFUNCTIONLIBRARY_RESOURCEPARSER_H

#ifndef AFUNCTIONLIBRARY_RESOURCEPARSER_H
#define AFUNCTIONLIBRARY_RESOURCEPARSER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

#include "pugixml.hpp"

#include "AFunctionLibrary/utility.h"
#include "tokenextras.h"

namespace afl
{
    namespace detail
    {

        TokenAssociativity stringToTokenAssociativity(const std::string& string, TokenAssociativity defaultValue);
        TokenType stringToTokenType(const std::string& string, TokenType defaultValue);

        void parseExtensionRecursive(const pugi::xml_node& node,
                                     std::vector<std::shared_ptr<TokenWrapper<std::string>>>& tokens);

        void parseTokenAliases(std::string aliasString, TokenBundle<std::string> &bundle, TokenAliasType type);
        template<typename T>
        void parseTokenField(const pugi::xml_node& node, const std::string& fieldName, const T& fieldValue,
                             TokenBundle<std::string>& bundle,
                             std::vector<std::shared_ptr<TokenWrapper<std::string>>>& tokens,
                             std::unordered_set<std::string>& valueSet);
        void parseTokensRecursive(const pugi::xml_node& node, TokenBundle<std::string> bundle,
                                  std::vector<std::shared_ptr<TokenWrapper<std::string>>>& tokens,
                                  std::unordered_set<std::string>& valueSet);
    }
}

#include "resourceparser.tpp"

#endif //AFUNCTIONLIBRARY_RESOURCEPARSER_H

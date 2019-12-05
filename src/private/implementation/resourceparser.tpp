#ifndef AFUNCTIONLIBRARY_RESOURCEPARSER_TPP
#define AFUNCTIONLIBRARY_RESOURCEPARSER_TPP

template<typename T>
void afl::detail::parseTokenField(const pugi::xml_node& node, const std::string& fieldName, const T& fieldValue,
                                  TokenInstanceBundle<std::string>& bundle,
                                  std::vector<std::shared_ptr<TokenPtrBundle<std::string>>>& tokens,
                                  std::unordered_set<std::string>& valueSet)
{
    std::string fieldString = afl::trimString(fieldValue.as_string(""));
    if (!fieldValue.empty()) {
        if (fieldName == "type" && !fieldString.empty()) {
            bundle.token.type = stringToTokenType(fieldString, bundle.token.type);
        } else if (fieldName == "precedence") {
            if(fieldString == "-1")
                bundle.token.precedence = -1;
            else
                bundle.token.precedence = static_cast<size_t>(fieldValue.as_uint(bundle.token.precedence));
        } else if (fieldName == "parameter_count") {
            if(fieldString == "-1")
                bundle.token.parameterCount = -1;
            else
                bundle.token.parameterCount = static_cast<size_t>(fieldValue.as_uint(bundle.token.parameterCount));
        } else if (fieldName == "associativity" && !fieldString.empty()) {
            bundle.token.associativity = stringToTokenAssociativity(fieldString, bundle.token.associativity);
        } else if (fieldName == "alias" || fieldName == "aliases") { // alias without extra type is interpreted as string alias
            std::string typeString = afl::trimString(node.attribute("type").as_string(""));
            if(typeString == "string" || typeString.empty()) {
                parseTokenAliases(fieldString, bundle, TokenAliasType::String);
            } else if(typeString == "regex") {
                parseTokenAliases(fieldString, bundle, TokenAliasType::Regex);
            }
        } else if ((fieldName == "value" || fieldName == "values") && !fieldString.empty()) {
            for(std::string s : splitAtSpaces(fieldString)) {
                if(valueSet.find(s) == valueSet.end()) {
                    bundle.token.value = s;
                    tokens.push_back(std::make_shared<TokenPtrBundle<std::string>>(bundle));
                    valueSet.emplace(std::move(s));
                }
            }
        }
    }
}

#endif //AFUNCTIONLIBRARY_RESOURCEPARSER_TPP

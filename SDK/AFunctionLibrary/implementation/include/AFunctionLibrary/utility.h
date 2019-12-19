#ifndef AFUNCTIONLIBRARY_UTILITY_H
#define AFUNCTIONLIBRARY_UTILITY_H

#include "implementation/exports.h"

#include <sstream>
#include <vector>
#include <string>
#include <stack>

#include "implementation/tokenaccessinterface.hpp"

namespace afl
{
    template<typename T>
    inline AFUNCTIONLIBRARY_EXPORT std::string stringify(T t);

    template<typename T>
    AFUNCTIONLIBRARY_EXPORT std::vector<T> shuntingYard(const std::vector<T>& infixTokens);

    inline AFUNCTIONLIBRARY_EXPORT std::vector<std::string> splitAtSpaces(std::string string);
    inline AFUNCTIONLIBRARY_EXPORT std::string trimString(std::string string);

    inline AFUNCTIONLIBRARY_EXPORT std::string replaceString(std::string string, const std::string& toReplace, const std::string& replacement);
    inline AFUNCTIONLIBRARY_EXPORT std::string replaceString(std::string string, const std::vector<std::string>& keys, const std::string& replacement);
    inline AFUNCTIONLIBRARY_EXPORT std::string replaceString(std::string string, const std::vector<std::pair<std::vector<std::string>, std::string>>& keysReplacementPairs);
}

#include "implementation/utility.ipp"

#endif //AFUNCTIONLIBRARY_UTILITY_H

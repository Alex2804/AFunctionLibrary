#ifndef AFUNCTIONLIBRARY_UTILITY_H
#define AFUNCTIONLIBRARY_UTILITY_H

#include <string>
#include <vector>
#include <sstream>

namespace afl
{
    template<typename T>
    inline std::string stringify(T t);

    std::vector<std::string> splitAtSpaces(std::string string);
    std::string trimString(std::string string);

    std::string replaceString(std::string string, const std::string& toReplace, const std::string& replacement);
    std::string replaceString(std::string string, const std::vector<std::string>& keys, const std::string& replacement);
    std::string replaceString(std::string string, const std::vector<std::pair<std::vector<std::string>, std::string>>& keysReplacementPairs);
}

#include "implementation/utility.tpp"

#endif //AFUNCTIONLIBRARY_UTILITY_H

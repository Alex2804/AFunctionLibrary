#ifndef AFUNCTIONLIBRARYSDK_UTILITES_H
#define AFUNCTIONLIBRARYSDK_UTILITES_H

#include "implementation/exports.h"

#include <sstream>
#include <vector>
#include <string>
#include <stack>

#include "implementation/tokenaccessinterface.hpp"

namespace afl
{
    template<typename T>
    inline std::string stringify(T t);
    template<typename Iter>
    inline std::string stringify(Iter begin, Iter end, std::string delimiter);

    inline std::vector<std::string> splitAtSpaces(std::string string);
    inline std::string trimString(std::string string);

    inline std::string replaceString(std::string string, const std::string& toReplace, const std::string& replacement);
    inline std::string replaceString(std::string string, const std::vector<std::string>& keys, const std::string& replacement);
    inline std::string replaceString(std::string string, const std::vector<std::pair<std::vector<std::string>, std::string>>& keysReplacementPairs);
}

#include "implementation/utilities.ipp"

#endif //AFUNCTIONLIBRARYSDK_UTILITES_H

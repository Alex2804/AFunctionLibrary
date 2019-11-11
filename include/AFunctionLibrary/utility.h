#ifndef AFUNCTIONLIBRARY_UTILITY_H
#define AFUNCTIONLIBRARY_UTILITY_H

#include <string>
#include <vector>
#include <sstream>

namespace afl
{
    template<typename T>
    std::string stringify(T t);

    std::vector<std::string> splitAtSpaces(std::string string);
    std::string trimString(std::string string);
}

#include "implementation/utility.tpp"

#endif //AFUNCTIONLIBRARY_UTILITY_H

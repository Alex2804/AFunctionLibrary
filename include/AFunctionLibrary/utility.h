#ifndef AFUNCTIONLIBRARY_UTILITY_H
#define AFUNCTIONLIBRARY_UTILITY_H

#include <string>
#include <sstream>

namespace afl
{
    template<typename T>
    std::string stringify(T t);
}

#include "implementation/utility.tpp"

#endif //AFUNCTIONLIBRARY_UTILITY_H

#ifndef AFUNCTIONLIBRARY_CSTRING_HPP
#define AFUNCTIONLIBRARY_CSTRING_HPP

#include "include/AFunctionLibrary/implementation/exports.h"

#include <cstring>

#include "../APluginSDK/pluginapi.h"

namespace afl
{
    typedef decltype(apl::PluginInfo::allocateMemory) AllocateMemoryFunction;
    typedef decltype(apl::PluginInfo::freeMemory) FreeMemoryFunction;

    struct CString
    {
        const FreeMemoryFunction freeFunction;
        char* string;
    };

    inline AFUNCTIONLIBRARY_EXPORT bool operator==(const CString& s1, const CString& s2);

    inline AFUNCTIONLIBRARY_EXPORT bool operator!=(const CString& s1, const CString& s2);

    inline AFUNCTIONLIBRARY_EXPORT bool equal(const CString* s1, const CString* s2);

    inline AFUNCTIONLIBRARY_NO_EXPORT void free(CString* cString);

    inline AFUNCTIONLIBRARY_NO_EXPORT CString* convert(const std::string& string);
    inline AFUNCTIONLIBRARY_NO_EXPORT std::string convert(CString* cString);
}


bool afl::operator==(const CString& s1, const CString& s2)
{
    return std::strcmp(s1.string, s2.string) == 0;
}

bool afl::operator!=(const CString &s1, const CString &s2)
{
    return !operator==(s1, s2);
}

bool afl::equal(const CString* s1, const CString* s2)
{
    if(s1 == nullptr || s2 == nullptr)
        return s1 == s2;
    return *s1 == *s2;
}

void afl::free(CString* cString)
{
    if(cString == nullptr)
        return;
    cString->freeFunction(cString->string);
    cString->freeFunction(cString);
}

afl::CString* afl::convert(const std::string& string)
{
    CString tmpCString = {apl::freeMemory, static_cast<char*>(apl::allocateMemory(sizeof(char) * (string.size() + 1)))};
    std::strcpy(tmpCString.string, string.c_str());
    auto cString = static_cast<CString*>(apl::allocateMemory(sizeof(CString)));
    memcpy(cString, &tmpCString, sizeof(CString));
    return cString;
}
std::string afl::convert(CString* cString)
{
    std::string string = (cString == nullptr || cString->string == nullptr) ? "" : cString->string;
    free(cString);
    return string;
}

#endif //AFUNCTIONLIBRARY_CSTRING_HPP

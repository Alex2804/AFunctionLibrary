#ifndef AFUNCTIONLIBRARYSDK_CSTRING_HPP
#define AFUNCTIONLIBRARYSDK_CSTRING_HPP

#include "include/AFunctionLibrary/implementation/exports.h"

#include <string>
#include <cstring>

#include "../libs/APluginSDK/pluginapi.h"

namespace afl
{
    typedef void(*FreeMemoryFunction)(void*);

    extern "C"
    {
        struct AFUNCTIONLIBRARY_NO_EXPORT CString
        {
            const FreeMemoryFunction freeFunction;
            char* string;
        };
    }

    inline AFUNCTIONLIBRARY_NO_EXPORT bool operator==(const CString& s1, const CString& s2);

    inline AFUNCTIONLIBRARY_NO_EXPORT bool operator!=(const CString& s1, const CString& s2);

    inline AFUNCTIONLIBRARY_NO_EXPORT bool equal(const CString* s1, const CString* s2);

    inline AFUNCTIONLIBRARY_NO_EXPORT void free(CString* cString);

    inline AFUNCTIONLIBRARY_NO_EXPORT CString* convert(const std::string& string);
    inline AFUNCTIONLIBRARY_NO_EXPORT std::string convert(CString* cString);
    inline AFUNCTIONLIBRARY_NO_EXPORT std::string convert(const CString* cString);
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
    size_t stringLength = string.size() + 1;
    CString tmpCString = {apl::APluginSDK_free, static_cast<char*>(apl::APluginSDK_malloc(sizeof(char) * stringLength))};
    if(tmpCString.string == nullptr)
        return nullptr;
    std::memcpy(tmpCString.string, string.c_str(), stringLength);
    auto cString = static_cast<CString*>(apl::APluginSDK_malloc(sizeof(CString)));
    if(cString == nullptr)
        return nullptr;
    std::memcpy(cString, &tmpCString, sizeof(CString));
    return cString;
}
std::string afl::convert(CString* cString)
{
    std::string string = convert(const_cast<const CString*>(cString));
    free(cString);
    return string;
}
std::string afl::convert(const CString* cString)
{
    return (cString == nullptr || cString->string == nullptr) ? "" : cString->string;
}

#endif //AFUNCTIONLIBRARYSDK_CSTRING_HPP

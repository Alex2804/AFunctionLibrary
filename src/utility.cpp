#include "AFunctionLibrary/utility.h"

namespace
{
    template<typename T>
    inline std::string stringifyFloatingPointNumber(T number)
    {
        std::string str = std::to_string(number);
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        str.erase(str.find_last_not_of('.') + 1, std::string::npos);
        return str;
    }
}
template<>
std::string afl::stringify(char t)
{
    return std::string(1, t);
}
template<>
std::string afl::stringify(float t)
{
    return stringifyFloatingPointNumber(t);
}
template<>
std::string afl::stringify(double t)
{
    return stringifyFloatingPointNumber(t);
}
template<>
std::string afl::stringify(long double t)
{
    return stringifyFloatingPointNumber(t);
}

/**
 * Splits the passed @p string at all spaces (line wraps and tabs also included).
 * @param string The string to split
 * @return The splitted @p string
 */
std::vector<std::string> afl::splitAtSpaces(std::string string)
{
    std::vector<std::string> separated;
    std::string tmp;
    tmp.reserve(30);
    string.push_back(' ');
    for(const char c : string) {
        if(std::isspace(c)) {
            if(!tmp.empty()) {
                separated.push_back(tmp);
                tmp.clear();
            }
        } else {
            tmp.push_back(c);
        }
    }
    return separated;
}

/**
 * Removes all leading and trailing spaces (line wraps and tabs also included).
 * @param string The string to trim
 * @return the trimmed string
 */
std::string afl::trimString(std::string string)
{
    size_t front, end;
    for(front = 0; front < string.size() && isspace(string[front]); front++);
    for(end = string.size() - 1; end > 0 && isspace(string[end]); end--);
    if(end < front)
        return "";
    return string.substr(front, end - front + 1);
}

#ifndef AFUNCTIONLIBRARY_UTILITIES_IPP
#define AFUNCTIONLIBRARY_UTILITIES_IPP

namespace afl
{
    namespace detail
    {
        template<class T>
        using to_string_t = decltype(std::to_string(std::declval<T>()));
        template<class T>
        using has_to_string = can_apply<to_string_t, void, T>;

        template<typename T>
        std::string stringify(T t, std::true_type, std::false_type); // has to_string, is not dereferenceable
        template<typename T>
        std::string stringify(T t, std::true_type, std::true_type); // has to_string, is dereferenceable
        template<typename T>
        std::string stringify(T t, std::false_type, std::false_type); // has no to_string, is not dereferenceable
        template<typename T>
        std::string stringify(T t, std::false_type, std::true_type); // has no to_string, is dereferenceable

        template<typename T>
        inline std::string stringifyFloatingPointNumber(T number);
    }
}

template<typename T>
std::string afl::detail::stringify(T t, std::true_type, std::false_type) // has to_string, is not dereferenceable
{
    return std::to_string(t);
}
template<typename T>
std::string afl::detail::stringify(T t, std::true_type, std::true_type) // has to_string, is dereferenceable
{
    return std::to_string(*t);
}
template<typename T>
std::string afl::detail::stringify(T t, std::false_type, std::false_type) // has no to_string, is not dereferenceable
{
    return static_cast<const std::ostringstream&>(std::ostringstream() << t).str();
}
template<typename T>
std::string afl::detail::stringify(T t, std::false_type, std::true_type) // has no to_string, is dereferenceable
{
    return static_cast<const std::ostringstream&>(std::ostringstream() << *t).str();
}

namespace afl
{
    template<>
    inline std::string stringify<char>(char t)
    {
        return std::string(1, t);
    }
    template<>
    inline std::string stringify<float>(float t)
    {
        return afl::detail::stringifyFloatingPointNumber(t);
    }
    template<>
    inline std::string stringify<double>(double t)
    {
        return afl::detail::stringifyFloatingPointNumber(t);
    }
    template<>
    inline std::string stringify<long double>(long double t)
    {
        return afl::detail::stringifyFloatingPointNumber(t);
    }
}

template<typename T>
inline std::string afl::detail::stringifyFloatingPointNumber(T number)
{
    std::string str = std::to_string(number);
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    str.erase(str.find_last_not_of('.') + 1, std::string::npos);
    return str;
}

/**
 * Converts @p t into an string with either std::to_string if possible or the ostream operator (operator<<)
 * @tparam T The type of @p t
 * @param t The value which should be converted to a string
 * @return @p t converted to a string
 */
template<typename T>
std::string afl::stringify(T t){
    return afl::detail::stringify(t, detail::has_to_string<T>{}, detail::is_dereferenceable<T>{});
}


// ================================================== splitAtSpaces ================================================= //


/**
 * Splits the passed @p string at all spaces (line wraps and tabs also included).
 * @param string The string to split
 * @return The splitted @p string
 */
std::vector<std::string> afl::splitAtSpaces(std::string string)
{
    std::vector<std::string> separated = {};
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


// =================================================== trimString =================================================== //


/**
 * Removes all leading and trailing spaces (line wraps and tabs also included).
 * @param string The string to trim
 * @return the trimmed string
 */
std::string afl::trimString(std::string string)
{
    size_t front, end;
    for(front = 0; front < string.size() && std::isspace(string[front]); front++);
    for(end = string.empty() ? 0 : string.size() - 1; end > 0 && std::isspace(string[end]); end--);
    if(end < front)
        return "";
    return string.substr(front, end - front + 1);
}


// ================================================== replaceString ================================================= //


/**
 * Replaces all occurrences of @p toReplace with @p replacement in @p string.
 * @param string The string to replace in
 * @param toReplace The key to replace in @p string
 * @param replacement The value to replace @p toReplace with in @p string
 * @return the replaced string
 */
std::string afl::replaceString(std::string string, const std::string &toReplace, const std::string &replacement)
{
    size_t index = 0;
    for (index = string.find(toReplace, index); index != std::string::npos; index = string.find(toReplace, index + (replacement.size() - toReplace.size()))) {
        string.replace(index, toReplace.size(), replacement);
    }
    return string;
}
/**
 * Replaces all occurrences of @p keys with @p replacement in @p string.
 * @param string The string to replace in
 * @param keys The keys to replace in @p string
 * @param replacement The value to replace @p keys with in @p string
 * @return the replaced string
 *
 * @see afl::replaceString
 */
std::string afl::replaceString(std::string string, const std::vector<std::string>& keys, const std::string& replacement)
{
    for(const std::string& key : keys) {
        string = replaceString(string, key, replacement);
    }
    return string;
}
/**
 * Replaces all occurrences of @p toReplace with @p replacement in @p string.
 * @param string The string to replace in
 * @param keysReplacementPairs Pairs with the values to replace as first and replacement as second element.
 * @return the replaced string
 *
 * @see afl::replaceString
 */
std::string afl::replaceString(std::string string, const std::vector<std::pair<std::vector<std::string>, std::string>>& keysReplacementPairs)
{
    for(const std::pair<std::vector<std::string>, std::string>& pair : keysReplacementPairs) {
        string = replaceString(string, pair.first, pair.second);
    }
    return string;
}

#endif //AFUNCTIONLIBRARY_UTILITIES_IPP

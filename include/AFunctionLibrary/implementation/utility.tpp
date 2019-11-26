#ifndef AFUNCTIONLIBRARY_UTILITY_TPP
#define AFUNCTIONLIBRARY_UTILITY_TPP

namespace afl
{
    namespace detail
    {
        template<template<class...> class Z, class, class...>
        struct can_apply : std::false_type {};
        template<template<class...> class Z, class...Ts>
        struct can_apply<Z, std::void_t<Z<Ts...>>, Ts...> : std::true_type {};

        template<class T>
        using to_string_t = decltype(std::to_string(std::declval<T>()));
        template<class T>
        using has_to_string = can_apply<to_string_t, void, T>;


        template<typename T>
        std::string stringify(T t, std::true_type);
        template<typename T>
        std::string stringify(T t, std::false_type);
    }
}


template<typename T>
std::string afl::detail::stringify(T t, std::true_type /*can to string*/){
    return std::to_string(t);
}
template<typename T>
std::string afl::detail::stringify(T t, std::false_type /*cannot to string*/){
    return static_cast<std::ostringstream&>(std::ostringstream() << t).str();
}
/**
 * Converts @p t into an string with either std::to_string if possible or the ostream operator (operator<<)
 * @tparam T The type of @p t
 * @param t The value which should be converted to a string
 * @return @p t converted to a string
 */
template<typename T>
std::string afl::stringify(T t){
    return afl::detail::stringify(t, detail::has_to_string<T>{});
}

template<>
std::string afl::stringify<char>(char t);
template<>
std::string afl::stringify<float>(float t);
template<>
std::string afl::stringify<double>(double t);
template<>
std::string afl::stringify<long double>(long double t);

#endif //AFUNCTIONLIBRARY_UTILITY_TPP

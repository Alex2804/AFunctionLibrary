#include "../functionprivate.h"

afl::detail::FunctionPrivate::FunctionPrivate(std::string rawString)
    : rawString(std::move(rawString))
{}

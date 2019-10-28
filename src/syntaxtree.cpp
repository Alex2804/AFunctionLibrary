#include "AFunctionLibrary/syntaxtree.h"

template<>
std::string afl::stringify(char t)
{
    return std::string(1, t);
}
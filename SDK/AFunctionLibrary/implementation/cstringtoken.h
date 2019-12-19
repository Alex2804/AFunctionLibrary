#ifndef AFUNCTIONLIBRARY_CSTRINGTOKEN_H
#define AFUNCTIONLIBRARY_CSTRINGTOKEN_H

#include "definitions.h"
#include "include/AFunctionLibrary/tokenenums.h"

namespace afl
{
    extern "C"
    {
        struct CStringToken
        {
            const char* string;
            TokenType type;
            int precedence;
            int parameterCount;
            TokenAssociativity associativity;
        };

        struct CStringTokenAliases
        {
            TokenAliasType type;
            char** aliases;
            size_t aliasesCount;
        };
    }
}

#endif //AFUNCTIONLIBRARY_CSTRINGTOKEN_H

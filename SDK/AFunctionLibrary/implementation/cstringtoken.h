#ifndef AFUNCTIONLIBRARY_CSTRINGTOKEN_H
#define AFUNCTIONLIBRARY_CSTRINGTOKEN_H

#include "definitions.h"

namespace afl
{
    extern "C"
    {
        enum class TokenType
        {
            Constant,
            Number,
            Operator,
            Function,
            BracketOpen,
            BracketClose,
            ArgumentDelimiter
        };
        enum class TokenAssociativity
        {
            None,
            Left,
            Right
        };
        struct CStringToken
        {
            const char* string;
            TokenType type;
            int precedence;
            int parameterCount;
            TokenAssociativity associativity;
        };

        enum class TokenAliasType
        {
            String,
            Regex
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

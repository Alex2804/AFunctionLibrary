#ifndef AFUNCTIONLIBRARY_TOKENENUMS_H
#define AFUNCTIONLIBRARY_TOKENENUMS_H

namespace afl
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
    enum class TokenAliasType
    {
        String,
        Regex
    };
}

#endif //AFUNCTIONLIBRARY_TOKENENUMS_H

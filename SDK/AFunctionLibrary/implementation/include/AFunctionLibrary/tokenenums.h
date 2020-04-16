#ifndef AFUNCTIONLIBRARYSDK_TOKENENUMS_H
#define AFUNCTIONLIBRARYSDK_TOKENENUMS_H

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

#endif //AFUNCTIONLIBRARYSDK_TOKENENUMS_H

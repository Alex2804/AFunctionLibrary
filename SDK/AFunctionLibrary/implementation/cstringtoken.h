#ifndef AFUNCTIONLIBRARY_CSTRINGTOKEN_H
#define AFUNCTIONLIBRARY_CSTRINGTOKEN_H

namespace afl
{
#define AFL_CREATE_TOKEN_FEATURE_GROUP_NAME afl_create_token
    constexpr const char* kCreateTokenFeatureGroupName = "afl_create_token";

#define AFL_CREATE_TOKEN_DEFAULT_FEATURE_NAME afl_create_token
    constexpr const char* kCreateTokenDefaultFeatureName = "afl_create_token";

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
    }
}

#endif //AFUNCTIONLIBRARY_CSTRINGTOKEN_H

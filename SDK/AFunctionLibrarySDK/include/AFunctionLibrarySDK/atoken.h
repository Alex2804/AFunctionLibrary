#ifndef AFUNCTIONLIBRARYSDK_ATOKEN_H
#define AFUNCTIONLIBRARYSDK_ATOKEN_H

#include "../../libs/ACUtils/include/ACUtils/astring.h"
#include "../../libs/ACUtils/include/ACUtils/types.h"

#include "../../private/macros.h"

PRIVATE_AFUNCTIONLIBRARYSDK_OPEN_EXTERN_C

    enum ATokenType
    {
        CONSTANT,
        NUMBER,
        OPERATOR,
        FUNCTION,
        BRACKET_OPEN,
        BRACKET_CLOSE,
        DELIMITER
    };

    enum ATokenAssociativity
    {
        NONE,
        LEFT,
        RIGHT
    };

    struct ATokenPrivate;
    struct AToken
    {
        struct ATokenPrivate *const dptr;
        struct AString *const value;
        enum ATokenType type;
        enum ATokenAssociativity associativity;
        size_t precedence;
        size_t parameterCount;
    };

    struct AToken* AToken_construct(const char *str, enum ATokenType type, enum ATokenAssociativity associativity, size_t precedence, size_t parameterCount);
    void AToken_destruct(struct AToken *token);

    size_t AToken_refCount(const struct AToken *token);
    size_t AToken_incrementRefCount(const struct AToken *token);
    size_t AToken_decrementRefCount(struct AToken *token);

    bool AToken_equals(const struct AToken *t1, const struct AToken *t2);

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#endif /* AFUNCTIONLIBRARYSDK_ATOKEN_H */

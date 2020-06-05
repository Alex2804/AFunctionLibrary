#ifndef AFUNCTIONLIBRARYSDK_ASTRINGTOKEN_H
#define AFUNCTIONLIBRARYSDK_ASTRINGTOKEN_H

#include "../../libs/ACUtils/include/ACUtils/astring.h"
#include "../../libs/ACUtils/include/ACUtils/types.h"

#include "../../private/dynarrays.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE PRIVATE_AFUNCTIONLIBRARYSDK_OPEN_EXTERN_C

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
        struct ATokenPrivate *const private;
        struct AString *const value;
        enum ATokenType type;
        enum ATokenAssociativity associativity;
        size_t precedence;
        size_t parameterCount;
    };

    struct ATokenGroup
    {
        struct AToken *const token;
        struct ADynSizeTArray *const groupID;
    };

    struct AToken* AToken_construct(const char *str, enum ATokenType type, enum ATokenAssociativity associativity, size_t precedence, size_t parameterCount);
    void AToken_destruct(struct AToken *token);
    size_t AToken_incrementRefCount(struct AToken *token);
    size_t AToken_decrementRefCount(struct AToken *token);
    bool AToken_equals(const struct AToken *t1, const struct AToken *t2);

    struct ATokenGroup* ATokenGroup_construct(struct AToken *token, bool transferOwnership);
    void ATokenGroup_destruct(struct ATokenGroup *tokenGroup);
    bool ATokenGroup_equals(const struct ATokenGroup *g1, const struct ATokenGroup *g2);

PRIVATE_AFUNCTIONLIBRARYSDK_CLOSE_EXTERN_C PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#endif /* AFUNCTIONLIBRARYSDK_ASTRINGTOKEN_H */

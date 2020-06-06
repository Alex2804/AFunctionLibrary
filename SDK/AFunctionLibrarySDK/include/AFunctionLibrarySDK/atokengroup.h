#ifndef AFUNCTIONLIBRARYSDK_ATOKENGROUP_H
#define AFUNCTIONLIBRARYSDK_ATOKENGROUP_H

#include "../../private/dynarrays.h"

#include "atoken.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE PRIVATE_AFUNCTIONLIBRARYSDK_OPEN_EXTERN_C

    struct ATokenGroup
    {
        struct AToken *const token;
        struct ADynSizeTArray *const groupID;
    };

    struct ATokenGroup* ATokenGroup_construct(struct AToken *token, bool transferOwnership);
    void ATokenGroup_destruct(struct ATokenGroup *tokenGroup);

    bool ATokenGroup_equals(const struct ATokenGroup *g1, const struct ATokenGroup *g2);

PRIVATE_AFUNCTIONLIBRARYSDK_CLOSE_EXTERN_C PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#endif /* AFUNCTIONLIBRARYSDK_ATOKENGROUP_H */

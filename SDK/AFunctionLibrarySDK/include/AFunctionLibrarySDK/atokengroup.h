#ifndef AFUNCTIONLIBRARYSDK_ATOKENGROUP_H
#define AFUNCTIONLIBRARYSDK_ATOKENGROUP_H

#include "../../private/dynarrays.h"

#include "atoken.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    struct ATokenGroup
    {
        struct AToken *const token;
        struct ADynSizeTArray *const groupID;
    };

    struct ATokenGroup* ATokenGroup_construct(struct AToken *token, bool transferOwnership);
    void ATokenGroup_destruct(struct ATokenGroup *tokenGroup);

    struct ATokenGroup* ATokenGroup_clone(const struct ATokenGroup *tokenGroup);

    bool ATokenGroup_equals(const struct ATokenGroup *g1, const struct ATokenGroup *g2);

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#endif /* AFUNCTIONLIBRARYSDK_ATOKENGROUP_H */

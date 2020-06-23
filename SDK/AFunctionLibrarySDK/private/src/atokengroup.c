#include "../../include/AFunctionLibrarySDK/atokengroup.h"

#include <string.h>

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    struct ATokenGroup* ATokenGroup_construct(struct AToken *token, bool transferOwnership)
    {
        struct ATokenGroup tmpGroup = {token, ADynArray_construct(struct ADynSizeTArray)};
        struct ATokenGroup *group;
        if(tmpGroup.token == nullptr || tmpGroup.groupID == nullptr) {
            ADynArray_destruct(tmpGroup.groupID);
            return nullptr;
        }
        group = (struct ATokenGroup*) ADynArray_reallocator(tmpGroup.groupID)(nullptr, sizeof(struct ATokenGroup));
        if (group == nullptr) {
            ADynArray_destruct(tmpGroup.groupID);
            return nullptr;
        }
        memcpy(group, &tmpGroup, sizeof(struct ATokenGroup));
        if(!transferOwnership)
            AToken_incrementRefCount(group->token);
        return group;
    }
    void ATokenGroup_destruct(struct ATokenGroup *tokenGroup)
    {
        struct ADynSizeTArray *groupID;
        if(tokenGroup == nullptr)
            return;
        groupID = tokenGroup->groupID;
        AToken_decrementRefCount(tokenGroup->token);
        ADynArray_deallocator(groupID)(tokenGroup);
        ADynArray_destruct(groupID);
    }

    bool ATokenGroup_equals(const struct ATokenGroup *g1, const struct ATokenGroup *g2)
    {
        size_t i, groupIDSize;
        if(g1 == g2 || g1 == nullptr || g2 == nullptr)
            return g1 == g2;
        groupIDSize = ADynArray_size(g1->groupID);
        if(groupIDSize != ADynArray_size(g2->groupID))
            return false;
        for(i = 0; i < groupIDSize; ++i) {
            if(ADynArray_get(g1->groupID, i) != ADynArray_get(g2->groupID, i))
                return false;
        }
        return AToken_equals(g1->token, g2->token);
    }

    struct ATokenGroup *ATokenGroup_clone(const struct ATokenGroup *tokenGroup)
    {
        struct ATokenGroup *clone;
        if(tokenGroup == nullptr)
            return nullptr;
        clone = ATokenGroup_construct(tokenGroup->token, false);
        if(clone == nullptr)
            return nullptr;
        if(!ADynArray_appendADynArray(clone->groupID, tokenGroup->groupID)) {
            ATokenGroup_destruct(clone);
            return nullptr;
        }
        return clone;
    }

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

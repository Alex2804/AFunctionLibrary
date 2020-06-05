#include "../../include/AFunctionLibrarySDK/atoken.h"

#include <string.h>

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    struct ATokenPrivate
    {
        size_t refCount;
    };

    struct AToken* AToken_construct(const char *str, enum ATokenType type, enum ATokenAssociativity associativity, size_t precedence, size_t parameterCount)
    {
        struct AString *string = AString_construct();
        if(string != nullptr && AString_appendCString(string, str, strlen(str))) {
            struct AToken *token = (struct AToken*) AString_reallocator(string)(nullptr, sizeof(struct AToken));
            struct ATokenPrivate *private = (struct ATokenPrivate*) AString_reallocator(string)(nullptr, sizeof(struct ATokenPrivate));
            if(token != nullptr && private != nullptr) {
                private->refCount = 1;
                struct AToken tmpToken = {private, string, type, associativity, precedence, parameterCount};
                memcpy(token, &tmpToken, sizeof(struct AToken));
                return token;
            }
            AString_deallocator(string)(token);
            AString_deallocator(string)(private);
        }
        AString_destruct(string);
        return nullptr;
    }
    void AToken_destruct(struct AToken *token)
    {
        struct AString *string;
        if(token == nullptr)
            return;
        string = token->value;
        AString_deallocator(string)(token->private);
        AString_deallocator(string)(token);
        AString_destruct(string);
    }
    size_t AToken_incrementRefCount(struct AToken *token)
    {
        return ++token->private->refCount;
    }
    size_t AToken_decrementRefCount(struct AToken *token)
    {
        if(--token->private->refCount == 0) {
            AToken_destruct(token);
            return 0;
        } else {
            return token->private->refCount;
        }
    }

    bool AToken_equals(const struct AToken *t1, const struct AToken *t2)
    {
        return t1 == t2 || (t1 != nullptr && t2 != nullptr && AString_equals(t1->value, t2->value)
            && t1->type == t2->type && t1->associativity == t2->associativity && t1->precedence == t2->precedence
            && t1->parameterCount == t2->parameterCount);
    }

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

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

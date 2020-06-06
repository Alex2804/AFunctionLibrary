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
            struct ATokenPrivate *dptr = (struct ATokenPrivate*) AString_reallocator(string)(nullptr, sizeof(struct ATokenPrivate));
            if(token != nullptr && dptr != nullptr) {
                struct AToken tmpToken = {dptr, string, type, associativity, precedence, parameterCount};
                dptr->refCount = 1;
                memcpy(token, &tmpToken, sizeof(struct AToken));
                return token;
            }
            AString_deallocator(string)(token);
            AString_deallocator(string)(dptr);
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
        AString_deallocator(string)(token->dptr);
        AString_deallocator(string)(token);
        AString_destruct(string);
    }

    size_t AToken_refCount(const struct AToken *token)
    {
        return token == nullptr ? 0 : token->dptr->refCount;
    }
    size_t AToken_incrementRefCount(struct AToken *token)
    {
        return token == nullptr ? 0 : ++token->dptr->refCount;
    }
    size_t AToken_decrementRefCount(struct AToken *token)
    {
        if(token == nullptr || --token->dptr->refCount == 0) {
            AToken_destruct(token);
            return 0;
        } else {
            return token->dptr->refCount;
        }
    }

    bool AToken_equals(const struct AToken *t1, const struct AToken *t2)
    {
        return t1 == t2 || (t1 != nullptr && t2 != nullptr && AString_equals(t1->value, t2->value)
            && t1->type == t2->type && t1->associativity == t2->associativity && t1->precedence == t2->precedence
            && t1->parameterCount == t2->parameterCount);
    }

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

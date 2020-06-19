#include "../../include/AFunctionLibrarySDK/astringaliases.h"

#include <string.h>

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    struct AStringAliasesPrivate
    {
        size_t refCount;
    };

    struct AStringAliases *AStringAliases_construct(enum AStringAliasesType type)
    {
        struct ADynStringArray *strings = ADynArray_construct(struct ADynStringArray);
        if(strings != nullptr) {
            struct AStringAliases *aliases = (struct AStringAliases*) ADynArray_reallocator(strings)(nullptr, sizeof(struct AStringAliases));
            struct AStringAliasesPrivate *dptr = (struct AStringAliasesPrivate*) ADynArray_reallocator(strings)(nullptr, sizeof(struct AStringAliasesPrivate));
            if(aliases != nullptr && dptr != nullptr) {
                struct AStringAliases tmpAliases = {dptr, type, strings};
                dptr->refCount = 1;
                memcpy(aliases, &tmpAliases, sizeof(struct AStringAliases));
                return aliases;
            }
            ADynArray_deallocator(strings)(aliases);
            ADynArray_deallocator(strings)(dptr);
        }
        ADynArray_destruct(strings);
        return nullptr;
    }
    void AStringAliases_destruct(struct AStringAliases *aliases)
    {
        struct ADynStringArray *strings;
        size_t i, stringsSize;
        if(aliases == nullptr)
            return;
        strings = aliases->strings;
        ADynArray_deallocator(strings)(aliases->dptr);
        ADynArray_deallocator(strings)(aliases);
        stringsSize = ADynArray_size(strings);
        for(i = 0; i < stringsSize; ++i)
            AString_destruct(ADynArray_get(strings, i));
        ADynArray_destruct(strings);
    }

    size_t AStringAliases_refCount(const struct AStringAliases *aliases)
    {
        return aliases == nullptr ? 0 : aliases->dptr->refCount;
    }
    size_t AStringAliases_incrementRefCount(struct AStringAliases *aliases)
    {
        return aliases == nullptr ? 0 : ++aliases->dptr->refCount;
    }
    size_t AStringAliases_decrementRefCount(struct AStringAliases *aliases)
    {
        if(aliases == nullptr) {
            return 0;
        } else if(--aliases->dptr->refCount == 0) {
            AStringAliases_destruct(aliases);
            return 0;
        } else {
            return aliases->dptr->refCount;
        }
    }

    bool AStringAliases_equals(const struct AStringAliases *a1, const struct AStringAliases *a2)
    {
        size_t i, j, a1Count, a2Count;
        if(a1 == a2 || a1 == nullptr || a2 == nullptr)
            return a1 == a2;
        else if(a1->type != a2->type || (a1Count = ADynArray_size(a1->strings)) != (a2Count = ADynArray_size(a2->strings)))
            return false;
        for(i = 0; i < a1Count; ++i) {
            struct AString *string = ADynArray_get(a1->strings, i);
            for(j = 0; j < a2Count; ++j) {
                if(AString_equals(string, ADynArray_get(a2->strings, j))) {
                    string = nullptr;
                    break;
                }
            }
            if(string != nullptr)
                return false;
        }
        return true;
    }

    bool AStringAliases_appendAString(struct AStringAliases *aliases, struct AString *string, bool transferOwnership)
    {
        size_t i, count;
        if(aliases == nullptr || string == nullptr)
            goto ReturnFalse;
        count = ADynArray_size(aliases->strings);
        for(i = 0; i < count; ++i) {
            if(AString_equals(string, ADynArray_get(aliases->strings, i)))
                goto ReturnFalse;
        }
        if(!transferOwnership)
            string = AString_clone(string);
        return ADynArray_append(aliases->strings, string);
    ReturnFalse:
        if(transferOwnership)
            AString_destruct(string);
        return false;
    }
    void AStringAliases_appendADynStringArray(struct AStringAliases *aliases, struct ADynStringArray *strings, bool transferOwnership)
    {
        size_t i, count = ADynArray_size(strings);
        if(aliases == nullptr) {
            if(transferOwnership) {
                for(i = 0; i < count; ++i)
                    AString_destruct(ADynArray_get(strings, i));
                ADynArray_destruct(strings);
            }
            return;
        }
        for(i = 0; i < count; ++i) {
            struct AString *string = ADynArray_get(strings, i);
            AStringAliases_appendAString(aliases, string, transferOwnership);
        }
        if(transferOwnership)
            ADynArray_destruct(strings);
    }

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

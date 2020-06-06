#ifndef AFUNCTIONLIBRARYSDK_ASTRINGALIASES_H
#define AFUNCTIONLIBRARYSDK_ASTRINGALIASES_H

#include "../../private/dynarrays.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    enum AStringAliasesType
    {
        STRING,
        REGEX
    };

    struct AStringAliasesPrivate;
    struct AStringAliases
    {
        struct AStringAliasesPrivate *const dptr;
        enum AStringAliasesType type;
        struct ADynStringArray *const strings;
    };

    struct AStringAliases* AStringAliases_construct(enum AStringAliasesType type);
    void AStringAliases_destruct(struct AStringAliases *aliases);

    size_t AStringAliases_refCount(const struct AStringAliases *aliases);
    size_t AStringAliases_incrementRefCount(struct AStringAliases *token);
    size_t AStringAliases_decrementRefCount(struct AStringAliases *token);

    bool AStringAliases_equals(const struct AStringAliases *a1, const struct AStringAliases *a2);

    bool AStringAliases_appendAString(struct AStringAliases *aliases, struct AString *string, bool transferOwnership);
    void AStringAliases_appendADynStringArray(struct AStringAliases *aliases, struct ADynStringArray *strings, bool transferOwnership);

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#endif /* AFUNCTIONLIBRARYSDK_ASTRINGALIASES_H */

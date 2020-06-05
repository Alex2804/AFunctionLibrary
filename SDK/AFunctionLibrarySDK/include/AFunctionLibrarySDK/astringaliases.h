#ifndef AFUNCTIONLIBRARYSDK_ASTRINGALIASES_H
#define AFUNCTIONLIBRARYSDK_ASTRINGALIASES_H

#include "../../private/dynarrays.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    enum AStringAliasesType
    {
        STRING,
        REGEX
    };

    struct AStringAliases
    {
        enum AStringAliasesType type;
        struct ADynStringArray *const strings;
    };

    struct AStringAliases* AStringAliases_construct();
    void AStringAliases_destruct(struct AStringAliases *aliases);

    void AStringAliases_appendAString(const struct AString *string);
    void AStringAliases_appendADynStringArray(const struct ADynStringArray *strings);

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#endif /* AFUNCTIONLIBRARYSDK_ASTRINGALIASES_H */

#ifndef AFUNCTIONLIBRARYSDK_DYNARRAYS_H
#define AFUNCTIONLIBRARYSDK_DYNARRAYS_H

#include "../libs/ACUtils/include/ACUtils/adynarray.h"
#include "../libs/ACUtils/include/ACUtils/astring.h"

#include "macros.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE
    A_DYNAMIC_ARRAY_DEFINITION(ADynStringArray, struct AString*);
    A_DYNAMIC_ARRAY_DEFINITION(ADynSizeTArray, size_t);
PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#endif /* AFUNCTIONLIBRARYSDK_DYNARRAYS_H */

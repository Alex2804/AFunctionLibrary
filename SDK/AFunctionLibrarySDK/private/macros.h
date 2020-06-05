#ifndef AFUNCTIONLIBRARYSDK_MACROS_H
#define AFUNCTIONLIBRARYSDK_MACROS_H

#include "../libs/ACUtils/include/ACUtils/macros.h"

#ifdef __cplusplus
#   include "cpp/macros.h"
#else
#   include "c/macros.h"
#endif

#define PRIVATE_AFUNCTIONLIBRARYSDK_OPEN_EXTERN_C ACUTILS_OPEN_EXTERN_C
#define PRIVATE_AFUNCTIONLIBRARYSDK_CLOSE_EXTERN_C ACUTILS_CLOSE_EXTERN_C

#endif /* AFUNCTIONLIBRARYSDK_MACROS_H */

#ifndef AFUNCTIONLIBRARYSDK_EXPORTS_H
#define AFUNCTIONLIBRARYSDK_EXPORTS_H

#ifdef AFUNCTIONLIBRARY_BUILD_LIBRARY
# include "AFunctionLibrary/afunctionlibrary_export.h"
#else
# include "../../../../libs/APluginSDK/implementation/macros.h"
# define AFUNCTIONLIBRARY_EXPORT APLUGINSDK_EXPORT
# define AFUNCTIONLIBRARY_NO_EXPORT APLUGINSDK_NO_EXPORT
#endif

#endif //AFUNCTIONLIBRARYSDK_EXPORTS_H

#ifndef AFUNCTIONLIBRARYSDK_ASYNTAXTREE_H
#define AFUNCTIONLIBRARYSDK_ASYNTAXTREE_H

#include "../../private/dynarrays.h"
#include "atoken.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE PRIVATE_AFUNCTIONLIBRARYSDK_OPEN_EXTERN_C

    struct ATokenNode;
    A_DYNAMIC_ARRAY_DEFINITION(ADynTokenNodeArray, struct ATokenNode*);

    struct ATokenNode
    {
        struct AToken *value;
        struct ADynTokenNodeArray *children;
    };

    struct ATokenSyntaxTree
    {
        struct ATokenNode *root;
    };

PRIVATE_AFUNCTIONLIBRARYSDK_CLOSE_EXTERN_C PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#endif /* AFUNCTIONLIBRARYSDK_ASYNTAXTREE_H */

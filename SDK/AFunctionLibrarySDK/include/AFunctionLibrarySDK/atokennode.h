#ifndef AFUNCTIONLIBRARYSDK_ASYNTAXTREE_H
#define AFUNCTIONLIBRARYSDK_ASYNTAXTREE_H

#include "../../libs/ACUtils/include/ACUtils/adynarray.h"
#include "../../libs/ACUtils/include/ACUtils/astring.h"

#include "atoken.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    struct ATokenNode;
    A_DYNAMIC_ARRAY_DEFINITION(ADynTokenNodeArray, struct ATokenNode*);

    struct ATokenNode
    {
        struct AToken *const token;
        struct ADynTokenNodeArray *const children;
    };

    struct ATokenNode* ATokenNode_construct(struct AToken *token, bool transferOwnership);
    void ATokenNode_destruct(struct ATokenNode *node);

    bool ATokenNode_equals(const struct ATokenNode *n1, const struct ATokenNode *n2);

    struct ATokenNode* ATokenNode_clone(const struct ATokenNode *node);

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#endif /* AFUNCTIONLIBRARYSDK_ASYNTAXTREE_H */

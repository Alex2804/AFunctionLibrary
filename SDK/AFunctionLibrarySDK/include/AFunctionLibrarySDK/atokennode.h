#ifndef AFUNCTIONLIBRARYSDK_ASYNTAXTREE_H
#define AFUNCTIONLIBRARYSDK_ASYNTAXTREE_H

#include "atokengroup.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    struct ATokenNode;
    A_DYNAMIC_ARRAY_DEFINITION(ADynTokenNodeArray, struct ATokenNode*);

    struct ATokenNode
    {
        struct ATokenGroup *const tokenGroup;
        struct ADynTokenNodeArray *const children;
    };

    struct ATokenNode* ATokenNode_construct(struct ATokenGroup *tokenGroup);
    void ATokenNode_destruct(struct ATokenNode *node);

    struct ATokenNode* ATokenNode_clone(const struct ATokenNode *node);

    bool ATokenNode_equals(const struct ATokenNode *n1, const struct ATokenNode *n2);

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#endif /* AFUNCTIONLIBRARYSDK_ASYNTAXTREE_H */

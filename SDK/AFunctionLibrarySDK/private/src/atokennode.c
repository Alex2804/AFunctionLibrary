#include "../../include/AFunctionLibrarySDK/atokennode.h"

#include <string.h>

#include "../dynarrays.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    struct ATokenNode* ATokenNode_construct(struct ATokenGroup *tokenGroup)
    {
        struct ATokenNode tmpNode = {tokenGroup, ADynArray_construct(struct ADynTokenNodeArray)};
        struct ATokenNode *node;
        if(tmpNode.tokenGroup == nullptr || tmpNode.children == nullptr) {
            ATokenGroup_destruct(tokenGroup);
            ADynArray_destruct(tmpNode.children);
            return nullptr;
        }
        node = (struct ATokenNode*) ADynArray_reallocator(tmpNode.children)(nullptr, sizeof(struct ATokenNode));
        if (node == nullptr) {
            ADynArray_destruct(tmpNode.children);
            return nullptr;
        }
        memcpy(node, &tmpNode, sizeof(struct ATokenNode));
        return node;
    }
    void ATokenNode_destruct(struct ATokenNode *node)
    {
        size_t i;
        struct ADynTokenNodeArray *children;
        if(node == nullptr)
            return;
        ATokenGroup_destruct(node->tokenGroup);
        children = node->children;
        for(i = 0; i < ADynArray_size(children); ++i)
            ATokenNode_destruct(ADynArray_get(children, i));
        ADynArray_deallocator(children)(node);
        ADynArray_destruct(children);
    }

    struct ATokenNode* ATokenNode_clone(const struct ATokenNode *node)
    {
        struct ATokenNode *clone;
        struct ADynTokenNodeArray *nodeStack, *cloneStack;
        size_t stackSize;
        if(node == nullptr)
            return nullptr;
        clone = ATokenNode_construct(ATokenGroup_clone(node->tokenGroup));
        nodeStack = ADynArray_construct(struct ADynTokenNodeArray);
        cloneStack = ADynArray_construct(struct ADynTokenNodeArray);
        if(clone == nullptr || nodeStack == nullptr || cloneStack == nullptr || !ADynArray_append(nodeStack, node)  || !ADynArray_append(cloneStack, clone))
            goto RETURN_FAILURE;
        while((stackSize = ADynArray_size(nodeStack)) > 0) {
            size_t i, childrenCount, lastStackIndex = stackSize - 1;
            struct ATokenNode *currentNode = ADynArray_get(nodeStack, lastStackIndex);
            ADynArray_remove(nodeStack, lastStackIndex, 1);
            struct ATokenNode *currentClone = ADynArray_get(cloneStack, lastStackIndex);
            ADynArray_remove(cloneStack, lastStackIndex, 1);
            childrenCount = ADynArray_size(currentNode->children);
            ADynArray_reserve(currentClone->children, childrenCount);
            for(i = 0; i < childrenCount; ++i) {
                struct ATokenNode *nodeChild = ADynArray_get(currentNode->children, i);
                struct ATokenNode *cloneChild = ATokenNode_construct(ATokenGroup_clone(nodeChild->tokenGroup));
                if(cloneChild == nullptr || (ADynArray_size(nodeChild->children) > 0
                                             && (!ADynArray_append(nodeStack, nodeChild) || !ADynArray_append(cloneStack, cloneChild))))
                {
                    ATokenNode_destruct(cloneChild);
                    goto RETURN_FAILURE;
                }
                ADynArray_append(currentClone->children, cloneChild);
            }
        }
        ADynArray_destruct(nodeStack);
        ADynArray_destruct(cloneStack);
        return clone;
    RETURN_FAILURE:
        ATokenNode_destruct(clone);
        ADynArray_destruct(nodeStack);
        ADynArray_destruct(cloneStack);
        return nullptr;
    }

    bool ATokenNode_equals(const struct ATokenNode *n1, const struct ATokenNode *n2)
    {
        size_t stackIndex = 0, childrenCount;
        struct ADynTokenNodeArray *n1Stack, *n2Stack;
        if(n1 == n2 || n1 == nullptr || n2 == nullptr)
            return n1 == n2;
        n1Stack = ADynArray_construct(struct ADynTokenNodeArray);
        ADynArray_append(n1Stack, n1);
        n2Stack = ADynArray_construct(struct ADynTokenNodeArray);
        ADynArray_append(n2Stack, n2);
        while(ADynArray_size(n1Stack) > 0) {
            n1 = ADynArray_get(n1Stack, stackIndex);
            ADynArray_remove(n1Stack, stackIndex, 1);
            n2 = ADynArray_get(n2Stack, stackIndex);
            ADynArray_remove(n2Stack, stackIndex, 1);
            childrenCount = ADynArray_size(n1->children);
            if(!ATokenGroup_equals(n1->tokenGroup, n2->tokenGroup) || childrenCount != ADynArray_size(n2->children)) {
                ADynArray_destruct(n1Stack);
                ADynArray_destruct(n2Stack);
                return false;
            }
            stackIndex += childrenCount - 1;
            ADynArray_appendADynArray(n1Stack, n1->children);
            ADynArray_appendADynArray(n2Stack, n2->children);
        }
        ADynArray_destruct(n1Stack);
        ADynArray_destruct(n2Stack);
        return true;
    }

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE
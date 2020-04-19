#ifndef AFUNCTIONLIBRARYSDK_CSTRINGTOKENSYNTAXTREE_HPP
#define AFUNCTIONLIBRARYSDK_CSTRINGTOKENSYNTAXTREE_HPP

#include "cstringtoken.hpp"
#include "include/AFunctionLibrary/syntaxtree.h"

namespace afl
{
    extern "C"
    {
        struct CStringTokenGroupNode
        {
            const FreeMemoryFunction freeFunction;
            CStringTokenGroup* token;
            size_t childrenCount;
            CStringTokenGroupNode** children;
        };
    }

    inline AFUNCTIONLIBRARY_EXPORT bool operator==(const CStringTokenGroupNode& n1, const CStringTokenGroupNode& n2);

    inline AFUNCTIONLIBRARY_EXPORT bool operator!=(const CStringTokenGroupNode& n1, const CStringTokenGroupNode& n2);

    inline AFUNCTIONLIBRARY_EXPORT bool equal(const CStringTokenGroupNode* n1, const CStringTokenGroupNode* n2);

    inline AFUNCTIONLIBRARY_NO_EXPORT void free(CStringTokenGroupNode* cNode);

    inline AFUNCTIONLIBRARY_NO_EXPORT CStringTokenGroupNode* convert(const Node<TokenGroup<std::string>>& node);
    inline AFUNCTIONLIBRARY_NO_EXPORT Node<TokenGroup<std::string>> convert(CStringTokenGroupNode* cNode, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&)=nullptr);
    inline AFUNCTIONLIBRARY_NO_EXPORT Node<TokenGroup<std::string>> convert(const CStringTokenGroupNode* cNode, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&)=nullptr);
}


bool afl::operator==(const CStringTokenGroupNode& n1, const CStringTokenGroupNode& n2)
{
    if(&n1 == &n2)
        return true;
    else if(!equal(n1.token, n2.token) || n1.childrenCount != n2.childrenCount)
        return false;
    for(size_t i = 0; i < n1.childrenCount; ++i) {
        if(!equal(n1.children[i], n2.children[i]))
            return false;
    }
    return true;
}

bool afl::operator!=(const CStringTokenGroupNode& n1, const CStringTokenGroupNode& n2)
{
    return !operator==(n1, n2);
}

bool afl::equal(const CStringTokenGroupNode* n1, const CStringTokenGroupNode* n2)
{
    if(n1 == nullptr || n2 == nullptr)
        return n1 == n2;
    return *n1 == *n2;
}

void afl::free(CStringTokenGroupNode* cNode)
{
    if(cNode == nullptr)
        return;
    free(cNode->token);
    for(size_t i = 0; i < cNode->childrenCount; ++i)
        free(cNode->children[i]);
    cNode->freeFunction(cNode->children);
    cNode->freeFunction(cNode);
}

afl::CStringTokenGroupNode* afl::convert(const Node<TokenGroup<std::string>>& node)
{
    CStringTokenGroupNode tmpCNode = {apl::freeMemory, convert(node.m_value), node.m_children.size(), nullptr};
    auto cNode = static_cast<CStringTokenGroupNode*>(apl::allocateMemory(sizeof(CStringTokenGroupNode)));
    memcpy(cNode, &tmpCNode, sizeof(CStringTokenGroupNode));
    cNode->children = static_cast<CStringTokenGroupNode**>(apl::allocateMemory(sizeof(void*) * cNode->childrenCount));
    for(size_t i = 0; i < cNode->childrenCount; ++i)
        cNode->children[i] = convert(node.m_children[i]);
    return cNode;
}
afl::Node<afl::TokenGroup<std::string>> afl::convert(CStringTokenGroupNode* cNode, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&))
{
    auto node = convert(const_cast<const CStringTokenGroupNode*>(cNode), getTokenFunction);
    free(cNode);
    return node;
}
afl::Node<afl::TokenGroup<std::string>> afl::convert(const CStringTokenGroupNode* cNode, std::shared_ptr<Token<std::string>>(*getTokenFunction)(const std::string&))
{
    if(cNode == nullptr)
        return Node<TokenGroup<std::string>>(convert(static_cast<const CStringTokenGroup*>(nullptr), getTokenFunction));
    std::vector<Node<TokenGroup<std::string>>> children;
    children.reserve(cNode->childrenCount);
    for(size_t i = 0; i < cNode->childrenCount; ++i)
        children.push_back(std::move(convert(const_cast<const CStringTokenGroupNode*>(cNode->children[i]), getTokenFunction)));
    return afl::Node<afl::TokenGroup<std::string>>(convert(const_cast<const CStringTokenGroup*>(cNode->token), getTokenFunction), children);
}

#endif //AFUNCTIONLIBRARYSDK_CSTRINGTOKENSYNTAXTREE_HPP

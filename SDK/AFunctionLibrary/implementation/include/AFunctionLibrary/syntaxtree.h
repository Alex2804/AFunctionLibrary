#ifndef AFUNCTIONLIBRARYSDK_SYNTAXTREE_H
#define AFUNCTIONLIBRARYSDK_SYNTAXTREE_H

#include "implementation/exports.h"

#include <vector>
#include <memory>
#include <stack>
#include <functional>
#include <forward_list>

#include "utilites.h"
#include "token.h"
#include "implementation/tokenaccessinterface.hpp"

namespace afl
{
    template<typename T>
    class AFUNCTIONLIBRARY_EXPORT Node;

    template<typename T>
    class AFUNCTIONLIBRARY_EXPORT SyntaxTree
    {
    public:
        SyntaxTree();
        explicit SyntaxTree(T value);
        explicit SyntaxTree(Node<T> root);
        explicit SyntaxTree(std::unique_ptr<Node<T>> root);
        SyntaxTree(const SyntaxTree<T>& other);
        SyntaxTree(SyntaxTree<T>&& other) noexcept;
        virtual ~SyntaxTree();

        SyntaxTree<T>& operator=(const SyntaxTree<T>& other);
        SyntaxTree<T>& operator=(SyntaxTree<T>&& other) noexcept;

        bool operator==(const SyntaxTree<T>& other) const;
        bool operator!=(const SyntaxTree<T>& other) const;

        bool empty() const;
        void clear();

        const Node<T>* root() const;
        Node<T>* root();
        void setRoot(std::unique_ptr<Node<T>> root, bool deleteOld=true);

        std::string toString() const;

    private:
        Node<T>* m_root;
    };

    template<typename T>
    AFUNCTIONLIBRARY_EXPORT std::ostream& operator<<(std::ostream& os, const SyntaxTree<T>& tree);

    template<typename T>
    AFUNCTIONLIBRARY_EXPORT std::vector<T> shuntingYard(const std::vector<T>& infixTokens);
    template<typename T>
    AFUNCTIONLIBRARY_EXPORT SyntaxTree<T> generateSyntaxTree(const std::vector<T>& infixTokens);


    template<typename T>
    class AFUNCTIONLIBRARY_EXPORT Node
    {
    public:
        explicit Node(T value);
        Node(T value, std::vector<Node<T>> children);
        Node(const Node<T>& other) = default;
        Node(Node<T>&& other) noexcept = default;
        virtual ~Node() = default;

        Node<T>& operator=(const Node<T>& other) = default;
        Node<T>& operator=(Node<T>&& other) noexcept;

        bool operator==(const Node<T>& other) const;
        bool operator!=(const Node<T>& other) const;

        std::vector<T> subtreeValues() const;

        std::string toString() const;

        T m_value;
        std::vector<Node<T>> m_children;
    };

    template<typename T>
    AFUNCTIONLIBRARY_EXPORT std::ostream& operator<<(std::ostream& os, const Node<T>& node);
}

#include "implementation/syntaxtree.tpp"

#endif //AFUNCTIONLIBRARYSDK_SYNTAXTREE_H

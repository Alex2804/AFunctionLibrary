#ifndef AFUNCTIONLIBRARY_SYNTAXTREE_H
#define AFUNCTIONLIBRARY_SYNTAXTREE_H

#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <sstream>
#include <type_traits>

#include "AFunctionLibrary/afunctionlibrary_export.h"

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
        explicit SyntaxTree(Node<T>* root);
        SyntaxTree(const SyntaxTree<T>& other);
        SyntaxTree(SyntaxTree<T>&& other) noexcept;
        virtual ~SyntaxTree();

        SyntaxTree<T>& operator=(const SyntaxTree<T>& other);
        SyntaxTree<T>& operator=(SyntaxTree<T>&& other) noexcept;

        std::string toString();

        Node<T>* root;
    };


    template<typename T>
    class AFUNCTIONLIBRARY_EXPORT Node
    {
    public:
        explicit Node(T value);
        Node(T value, std::vector<Node<T>*> children);
        Node(const Node<T>& other);
        Node(Node<T>&& other) noexcept;
        virtual ~Node();

        Node<T>& operator=(const Node<T>& other);
        Node<T>& operator=(Node<T>&& other) noexcept;

        std::string toString();

        T value;
        std::vector<Node<T>*> children;
    };


    template<typename T>
    std::string stringify(T t);
    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Node<T>& n);
    template<typename T>
    std::ostream& operator<<(std::ostream& os, const SyntaxTree<T>& s);
}

#include "implementation/syntaxtree.tpp"

#endif //AFUNCTIONLIBRARY_SYNTAXTREE_H

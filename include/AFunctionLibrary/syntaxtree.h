#ifndef AFUNCTIONLIBRARY_SYNTAXTREE_H
#define AFUNCTIONLIBRARY_SYNTAXTREE_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include <vector>
#include <string>

#include "AFunctionLibrary/utility.h"

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

        bool empty() const;
        void clear();

        const Node<T>* root() const;
        void setRoot(Node<T>* root, bool deleteOld=true);

        std::string toString() const;

    private:
        Node<T>* m_root;
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const SyntaxTree<T>& tree);


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

        std::string toString() const;

        T m_value;
        std::vector<Node<T>*> m_children;
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Node<T>& node);
}

#include "implementation/syntaxtree.tpp"

#endif //AFUNCTIONLIBRARY_SYNTAXTREE_H

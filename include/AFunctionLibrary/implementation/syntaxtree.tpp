#ifndef AFUNCTIONLIBRARY_SYNTAXTREE_TPP
#define AFUNCTIONLIBRARY_SYNTAXTREE_TPP

namespace afl
{
    namespace detail
    {
        template<typename T>
        inline std::pair<std::vector<std::string>, size_t> toStringHelper(const Node <T> *node, const char *chars[]);
    }
}


// =================================================== SyntaxTree =================================================== //


template<typename T>
afl::SyntaxTree<T>::SyntaxTree()
    : m_root(nullptr)
{}
template<typename T>
afl::SyntaxTree<T>::SyntaxTree(T value)
    : m_root(new Node<T>(std::move(value)))
{}
template<typename T>
afl::SyntaxTree<T>::SyntaxTree(Node<T>* root)
    : m_root(root)
{}
template<typename T>
afl::SyntaxTree<T>::SyntaxTree(const SyntaxTree<T> &other)
    : m_root(new Node<T>(*other.m_root))
{}
template<typename T>
afl::SyntaxTree<T>::SyntaxTree(SyntaxTree<T> &&other) noexcept
    : m_root(nullptr)
{
    using std::swap;
    swap(m_root, other.m_root);
}
template<typename T>
afl::SyntaxTree<T>::~SyntaxTree()
{
    delete m_root;
}

template<typename T>
afl::SyntaxTree<T>& afl::SyntaxTree<T>::operator=(const SyntaxTree<T> &other)
{
    auto tmpRoot = new Node<T>(*other.m_root);
    delete m_root;
    m_root = tmpRoot;
    return *this;
}
template<typename T>
afl::SyntaxTree<T>& afl::SyntaxTree<T>::operator=(SyntaxTree<T> &&other) noexcept
{
    using std::swap;
    swap(m_root, other.m_root);
    return *this;
}

template<typename T>
bool afl::SyntaxTree<T>::empty() const
{
    return m_root == nullptr;
}
template<typename T>
void afl::SyntaxTree<T>::clear()
{
    delete m_root;
    m_root = nullptr;
}

template<typename T>
const afl::Node<T>* afl::SyntaxTree<T>::root() const
{
    return m_root;
}
template<typename T>
void afl::SyntaxTree<T>::setRoot(afl::Node<T> *root, bool deleteOld)
{
    if(deleteOld)
        clear();
    m_root = root;
}

template<typename T>
std::string afl::SyntaxTree<T>::toString() const
{
    return empty() ? "" : m_root->toString();
}
template<typename T>
std::ostream& afl::operator<<(std::ostream& os, const SyntaxTree<T>& tree)
{
    os << tree.toString();
    return os;
}

// ====================================================== Node ====================================================== //


template<typename T>
afl::Node<T>::Node(T value)
        : m_value(std::move(value))
        , m_children(std::vector<Node < T> * > ())
{}
template<typename T>
afl::Node<T>::Node(T value, std::vector<Node<T>*> children)
    : m_value(std::move(value)), m_children(std::move(children))
{}
template<typename T>
afl::Node<T>::Node(const Node<T> &other)
        : m_value(other.m_value)
{
    m_children.reserve(other.m_children.size());
    for(const Node<T>* child : other.m_children) {
        m_children.push_back(new Node<T>(*child));
    }
}
template<typename T>
afl::Node<T>::Node(Node<T> &&other) noexcept
{
    using std::swap;
    swap(m_value, other.m_value);
    swap(m_children, other.m_children);
}
template<typename T>
afl::Node<T>::~Node()
{
    for(Node<T>* child : m_children) {
        delete child;
    }
}

template<typename T>
afl::Node<T> &afl::Node<T>::operator=(const Node<T> &other)
{
    // copy value
    m_value = other.m_value;
    // copy children
    std::vector<Node<T>*> tmpChildren;
    tmpChildren.reserve(other.m_children.size());
    for(const Node<T>* child : other.m_children) {
        tmpChildren.push_back(new Node<T>(*child));
    }
    using std::swap;
    swap(m_children, tmpChildren);
    for(Node<T>* child : tmpChildren) {
        delete child;
    }
    return *this;
}
template<typename T>
afl::Node<T> &afl::Node<T>::operator=(Node<T> &&other) noexcept
{
    using std::swap;
    swap(m_value, other.m_value);
    swap(m_children, other.m_children);
    return *this;
}

template<typename T>
std::string afl::Node<T>::toString() const
{
    const char* chars[] = {"\u007c", "\u251c", "\u250c", "\u2500", "\u253c", "\u252c", "\u2524", "\u2510", "\u2534"};
    std::vector<std::string> lines = detail::toStringHelper(this, chars).first;
    std::string string;
    if(!lines.empty()) {
        string.reserve(static_cast<int>((lines.size() - 1) * (lines.front().size() + 1)) + (lines.back().size()));
        for(size_t i = 0; i < lines.size() - 1; i++) {
            string.append(lines[i]).push_back('\n');
        }
        string.append(lines.back());
    }
    return string;
}
template<typename T>
std::ostream& afl::operator<<(std::ostream& os, const Node<T>& node)
{
    os << node.toString();
    return os;
}


// ================================================= toStringHelper ================================================= //


template<typename T>
std::pair<std::vector<std::string>, size_t> afl::detail::toStringHelper(const afl::Node<T> *node, const char* chars[])
{
    std::vector<std::string> bodyLines, headLines;
    size_t width = 0, lineCount = 0;
    std::pair<std::vector<std::string>, size_t> pair;
    std::vector<std::pair<std::vector<std::string>, size_t>> pairs;
    std::vector<size_t> widths, positions;
    std::string string, prependString, appendString, spacer(3, ' ');
    std::stringstream stringstream;
    stringstream << node->m_value;
    std::string value = afl::stringify(node->m_value);

    int nodeCount = node->m_children.size();
    if(nodeCount > 0) {
        pairs.reserve(nodeCount);
        widths.reserve(nodeCount);
        for (int i = 0; i < nodeCount; i++) {
            pair = toStringHelper(node->m_children.at(i), chars);
            if (!pair.first.empty() && !pair.first.front().empty()) {
                lineCount = pair.first.size() > lineCount ? pair.first.size() : lineCount; // std::max(lineCount, pair.first.size());
                pairs.push_back(pair);
                widths.push_back(pair.first.front().size());
                positions.push_back(width + pair.second);
                width += widths.back();
                if(i < nodeCount - 1) {
                    width += spacer.size();
                }
            }
        }

        if(width < value.size()) {
            prependString = std::string((value.size() - width) / 2, ' ');
            appendString = std::string(value.size() - (width + prependString.size()), ' ');
            width = value.size();
        }

        if(!pairs.empty()) {
            bodyLines.reserve(lineCount);
            for (unsigned long lineNumber = 0; lineNumber < lineCount; lineNumber++) {
                string = std::string();
                string.reserve(width);
                // fill line with data
                string.append(prependString);
                for (unsigned long i = 0; i < pairs.size(); i++) {
                    if (lineNumber > pairs.at(i).first.size() - 1) {
                        string.append(std::string(widths.at(i), ' '));
                    } else {
                        string.append(pairs.at(i).first.at(lineNumber));
                    }
                    if (i < pairs.size() - 1) {
                        string.append(spacer);
                    }
                }
                string.append(appendString);
                bodyLines.push_back(string);
            }
        }
    }

    size_t valueMiddleIndex, valueStartIndex;
    if(pairs.empty()) {
        headLines.push_back(value);
        return {headLines, value.size() / 2};
    } else if(pairs.size() == 1) {
        valueMiddleIndex = prependString.size() + pairs.front().second;
        valueStartIndex = valueMiddleIndex - (value.size() / 2);
        headLines.reserve(2);
        headLines.push_back(std::string(valueStartIndex, ' ').append(value).append(width - valueStartIndex - value.size(), ' '));
        headLines.push_back(std::string(valueMiddleIndex, ' ').append(chars[0]).append(width - valueMiddleIndex - 1, ' '));
    } else {
        valueMiddleIndex = positions.front() + ((positions.back() - positions.front()) / 2);
        valueStartIndex = valueMiddleIndex - (value.size() / 2);
        string = std::string();
        string.reserve(width);
        string.append(valueStartIndex, ' ').append(value).append(width - valueStartIndex - value.size(), ' ');
        headLines.push_back(string);
        std::vector<const char*> connectionStringVector;
        connectionStringVector.reserve(width);
        connectionStringVector.insert(connectionStringVector.end(), prependString.size() + positions.front(), " ");
        connectionStringVector.push_back((positions.front() == valueMiddleIndex) ? chars[1] : chars[2]);
        bool connectedValue = false;
        for(unsigned long i = 1; i < positions.size()-1; i++) {
            connectionStringVector.insert(connectionStringVector.end(), prependString.size() + positions.at(i) - connectionStringVector.size(), chars[3]);
            connectionStringVector.push_back((positions.at(i) == valueMiddleIndex) ? chars[4] : chars[5]);
            if(positions.at(i) == valueMiddleIndex) {
                connectedValue = true;
            }
        }
        connectionStringVector.insert(connectionStringVector.end(), prependString.size() + positions.back() - connectionStringVector.size(), chars[3]);
        connectionStringVector.push_back((positions.back() == valueMiddleIndex) ? chars[6] : chars[7]);
        connectionStringVector.insert(connectionStringVector.end(), width - connectionStringVector.size(), " ");
        if(!connectedValue) {
            connectionStringVector[valueMiddleIndex] = chars[8];
        }
        std::string connectionString;
        connectionString.reserve(width);
        for(const char* c : connectionStringVector) {
            connectionString.append(c);
        }
        headLines.push_back(connectionString);
    }

    headLines.insert(headLines.end(), bodyLines.begin(), bodyLines.end());
    return {headLines, valueMiddleIndex};
}

#endif //AFUNCTIONLIBRARY_SYNTAXTREE_TPP

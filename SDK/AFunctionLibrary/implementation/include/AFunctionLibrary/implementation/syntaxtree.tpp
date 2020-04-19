#ifndef AFUNCTIONLIBRARYSDK_SYNTAXTREE_TPP
#define AFUNCTIONLIBRARYSDK_SYNTAXTREE_TPP

namespace afl
{
    namespace detail
    {
        template<typename T>
        inline std::pair<std::vector<std::string>, size_t> toStringHelper(const Node<T>& node, const char* connectors[]);
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
afl::SyntaxTree<T>::SyntaxTree(Node<T> root)
    : m_root(new Node<T>(std::move(root)))
{}
template<typename T>
afl::SyntaxTree<T>::SyntaxTree(std::unique_ptr<Node<T>> root)
    : m_root(root.release())
{}
template<typename T>
afl::SyntaxTree<T>::SyntaxTree(const SyntaxTree<T>& other)
    : m_root(new Node<T>(*other.m_root))
{}
template<typename T>
afl::SyntaxTree<T>::SyntaxTree(SyntaxTree<T>&& other) noexcept
    : m_root(other.m_root)
{
    other.m_root = nullptr;
}
template<typename T>
afl::SyntaxTree<T>::~SyntaxTree()
{
    delete m_root;
}

template<typename T>
afl::SyntaxTree<T>& afl::SyntaxTree<T>::operator=(const SyntaxTree<T>& other)
{
    auto tmpRoot = new Node<T>(*other.m_root);
    delete m_root;
    m_root = tmpRoot;
    return *this;
}
template<typename T>
afl::SyntaxTree<T>& afl::SyntaxTree<T>::operator=(SyntaxTree<T>&& other) noexcept
{
    using std::swap;
    swap(m_root, other.m_root);
    return *this;
}

template<typename T>
bool afl::SyntaxTree<T>::operator==(const afl::SyntaxTree<T>& other) const
{
    if(m_root == nullptr || other.m_root == nullptr || m_root == other.m_root)
        return m_root == other.m_root;
    return *m_root == *other.m_root;
}
template<typename T>
bool afl::SyntaxTree<T>::operator!=(const afl::SyntaxTree<T>& other) const
{
    return !operator==(other);
}

template<typename T>
bool afl::SyntaxTree<T>::empty() const
{
    return m_root == nullptr;
}
template<typename T>
void afl::SyntaxTree<T>::clear()
{
    m_root = nullptr;
}

template<typename T>
const afl::Node<T>* afl::SyntaxTree<T>::root() const
{
    return m_root;
}
template<typename T>
afl::Node<T>* afl::SyntaxTree<T>::root()
{
    return m_root;
}
template<typename T>
void afl::SyntaxTree<T>::setRoot(std::unique_ptr<afl::Node<T>> root, bool deleteOld)
{
    if(deleteOld)
        delete m_root;
    m_root = root.release();
}

template<typename T>
std::string afl::SyntaxTree<T>::toString() const
{
    return empty() ? "" : m_root->toString();
}

template<typename T>
std::ostream& afl::operator<<(std::ostream& os, const SyntaxTree<T>& tree)
{
    return os << tree.toString();
}


// ================================================== shunting yard ================================================== //


/**
 * Applies the shunting-yard algorithm to the infixTokens vector.
 *
 * T must implement the methods "afl::TokenType getType() const", "size_t getPrecedence() const" and
 * "afl::TokenAssociativity getAssociativity() const".
 *
 * @tparam T Type of the token
 * @param infixTokens The tokens in infix notation.
 * @return The tokens in postfix notation.
 */
template<typename T>
std::vector<T> afl::shuntingYard(const std::vector<T>& infixTokens)
{
    std::vector<T> output;
    std::stack<T> stack;
    for(const T& token : infixTokens) {
        if(getType(token) == TokenType::Number || getType(token)  == TokenType::Constant) {
            output.push_back(token);
        } else if(getType(token)  == TokenType::Function || getType(token)  == TokenType::BracketOpen) {
            stack.push(token);
        } else if(getType(token)  == TokenType::ArgumentDelimiter) {
            while(getType(stack.top()) != TokenType::BracketOpen) {
                output.push_back(stack.top());
                stack.pop();
                if(stack.empty())
                    throw std::runtime_error("Wrong placed argument delimiter or missing opening for closing bracket");
            }
        } else if(getType(token)  == TokenType::Operator) {
            while(!stack.empty()
                  && getType(stack.top()) == TokenType::Operator
                  && getAssociativity<T>(token) == TokenAssociativity::Left
                  && getPrecedence(token) <= getPrecedence(stack.top()))
            {
                output.push_back(stack.top());
                stack.pop();
            }
            stack.push(token);
        } else if(getType(token)  == TokenType::BracketClose) {
            if(stack.empty())
                throw std::runtime_error("Missing opening for closing bracket!");
            while(getType(stack.top()) != TokenType::BracketOpen) {
                output.push_back(stack.top());
                stack.pop();
                if(stack.empty())
                    throw std::runtime_error("Missing opening for closing bracket!");
            }
            stack.pop();
            if(!stack.empty() && getType(stack.top()) == TokenType::Function) {
                output.push_back(stack.top());
                stack.pop();
            }
        } else {
            throw std::runtime_error("Unknown TokenType");
        }
    }
    while(!stack.empty()) {
        if(getType(stack.top())  == TokenType::BracketOpen)
            throw std::runtime_error("More opening than closing brackets!");
        output.push_back(stack.top());
        stack.pop();
    }
    return output;
}


// =============================================== generateSyntaxTree =============================================== //


template<typename T>
afl::SyntaxTree<T> afl::generateSyntaxTree(const std::vector<T>& infixTokens)
{
    std::vector<T> postfixTokens = afl::shuntingYard(infixTokens);

    std::stack<Node<T>> nodeStack;
    std::vector<Node<T>> tmpNodes;

    for(const auto& token : postfixTokens) {
        if(getType(token) == TokenType::Number || getType(token) == TokenType::Constant) {
            nodeStack.emplace(token);
        } else if(getType(token) == TokenType::Operator || getType(token) == TokenType::Function) {
            size_t count = getType(token) == TokenType ::Operator ? 2 : getParameterCount(token);
            tmpNodes.reserve(count);
            while(count-- > 0) {
                if(nodeStack.empty())
                    throw std::runtime_error("Not enough parameter for function or operator!");
                tmpNodes.emplace_back(std::move(nodeStack.top()));
                nodeStack.pop();
            }
            nodeStack.emplace(token, std::vector<Node<T>>(tmpNodes.rbegin(), tmpNodes.rend()));
        } else {
            throw std::runtime_error("Other TokenType than Number, Constant, Operator or Function in postfix notation (shunting yard doesn't work correctly)!");
        }
        tmpNodes.clear();
    }

    if(nodeStack.empty()) {
        throw std::runtime_error("Node stack mustn't be empty!");
    } else if(nodeStack.size() > 1) {
        throw std::runtime_error("There must be only one node on the node-stack (the root of the tree)!");
    }

    return SyntaxTree<T>(std::move(nodeStack.top()));
}


// ====================================================== Node ====================================================== //


template<typename T>
afl::Node<T>::Node(T value)
    : m_value(std::move(value))
{}
template<typename T>
afl::Node<T>::Node(T value, std::vector<Node<T>> children)
    : m_value(std::move(value)), m_children(std::move(children))
{}

template<typename T>
afl::Node<T>& afl::Node<T>::operator=(Node<T>&& other) noexcept
{
    m_value = std::move(other.m_value);
    using std::swap;
    swap(m_children, other.m_children);
    return *this;
}

template<typename T>
bool afl::Node<T>::operator==(const afl::Node<T>& other) const
{
    return m_value == other.m_value && m_children == other.m_children;
}
template<typename T>
bool afl::Node<T>::operator!=(const afl::Node<T>& other) const
{
    return !operator==(other);
}

template<typename T>
std::vector<T> afl::Node<T>::subtreeValues() const
{
    std::forward_list<T> values;
    std::stack<const afl::Node<T>*> stack;
    stack.push(this);
    while(!stack.empty()) {
        const Node<T>* node = stack.top();
        stack.pop();
        for(const Node<T>& child : node->m_children)
            stack.push(&child);
        values.push_front(node->m_value);
    }
    return std::vector<T>(values.begin(), values.end());
}

template<typename T>
std::string afl::Node<T>::toString() const
{
    const char* connectors[] = {"\u007c", "\342\224\234", "\342\224\214", "\342\224\200", "\342\224\274", "\342\224\254", "\342\224\244", "\342\224\220", "\342\224\264"};
    std::vector<std::string> lines = detail::toStringHelper(*this, connectors).first;
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
    return os << node.toString();
}


// ================================================= toStringHelper ================================================= //


template<typename T>
std::pair<std::vector<std::string>, size_t> afl::detail::toStringHelper(const afl::Node<T>& node, const char* connectors[])
{
    std::vector<std::string> bodyLines, headLines;
    size_t width = 0, lineCount = 0;
    std::pair<std::vector<std::string>, size_t> pair;
    std::vector<std::pair<std::vector<std::string>, size_t>> pairs;
    std::vector<size_t> widths, positions;
    std::string string, prependString, appendString, spacer(3, ' ');
    std::stringstream stringstream;
    stringstream << node.m_value;
    std::string value = afl::stringify(node.m_value);

    size_t nodeCount = node.m_children.size();
    if(nodeCount > 0) {
        pairs.reserve(nodeCount);
        widths.reserve(nodeCount);
        for (size_t i = 0; i < nodeCount; i++) {
            pair = toStringHelper(node.m_children.at(i), connectors);
            if (!pair.first.empty() && !pair.first.front().empty()) {
                lineCount = pair.first.size() > lineCount ? pair.first.size() : lineCount;
                pairs.push_back(pair);
                widths.push_back(pair.first.front().size());
                positions.push_back(width + pair.second);
                width += widths.back();
                if(i < nodeCount - 1)
                    width += spacer.size();
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
        valueStartIndex = (valueMiddleIndex < value.size() / 2) ? 0 : valueMiddleIndex - (value.size() / 2);
        size_t valuePrependSpaceCount = (width < valueStartIndex + value.size()) ? 0 : width - valueStartIndex - value.size();
        headLines.reserve(2);
        headLines.push_back(std::string(valueStartIndex, ' ').append(value).append(valuePrependSpaceCount, ' '));
        headLines.push_back(std::string(valueMiddleIndex, ' ').append(connectors[0]).append(width - valueMiddleIndex - 1, ' '));
    } else {
        valueMiddleIndex = positions.front() + ((positions.back() - positions.front()) / 2);
        valueStartIndex = (valueMiddleIndex < value.size() / 2) ? 0 : valueMiddleIndex - (value.size() / 2);
        string = std::string();
        string.reserve(width);
        string.append(valueStartIndex, ' ').append(value).append(width - valueStartIndex - value.size(), ' ');
        headLines.push_back(string);
        std::vector<std::string> connectionStringVector;
        connectionStringVector.reserve(width);
        connectionStringVector.insert(connectionStringVector.end(), prependString.size() + positions.front(), " ");
        connectionStringVector.emplace_back((positions.front() == valueMiddleIndex) ? connectors[1] : connectors[2]);
        bool connectedValue = false;
        for(unsigned long i = 1; i < positions.size()-1; i++) {
            connectionStringVector.insert(connectionStringVector.end(), prependString.size() + positions.at(i) - connectionStringVector.size(), connectors[3]);
            connectionStringVector.emplace_back((positions.at(i) == valueMiddleIndex) ? connectors[4] : connectors[5]);
            if(positions.at(i) == valueMiddleIndex) {
                connectedValue = true;
            }
        }
        connectionStringVector.insert(connectionStringVector.end(), prependString.size() + positions.back() - connectionStringVector.size(), connectors[3]);
        connectionStringVector.emplace_back((positions.back() == valueMiddleIndex) ? connectors[6] : connectors[7]);
        connectionStringVector.insert(connectionStringVector.end(), width - connectionStringVector.size(), " ");
        if(!connectedValue) {
            connectionStringVector[valueMiddleIndex] = connectors[8];
        }
        std::string connectionString;
        connectionString.reserve(width);
        for(const std::string& s : connectionStringVector) {
            connectionString.append(s);
        }
        headLines.push_back(connectionString);
    }

    headLines.insert(headLines.end(), bodyLines.begin(), bodyLines.end());
    return {headLines, valueMiddleIndex};
}

#endif //AFUNCTIONLIBRARYSDK_SYNTAXTREE_TPP

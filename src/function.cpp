#include "AFunctionLibrary/function.h"
#include "private/functionprivate.h"

afl::Function::Function()
    : d_ptr(new detail::FunctionPrivate())
{}
afl::Function::Function(std::string rawFunction)
    : d_ptr(new detail::FunctionPrivate(std::move(rawFunction)))
{}
afl::Function::Function(const afl::Function &other)
    : d_ptr(new detail::FunctionPrivate(*other.d_ptr))
{}
afl::Function::Function(afl::Function &&other) noexcept
    : d_ptr(other.d_ptr)
{
    other.d_ptr = nullptr;
}
afl::Function::~Function()
{
    delete d_ptr;
}

afl::Function &afl::Function::operator=(const afl::Function &other)
{
    *d_ptr = *other.d_ptr;
    return *this;
}
afl::Function &afl::Function::operator=(afl::Function &&other) noexcept
{
    using std::swap;
    swap(d_ptr, other.d_ptr);
    return *this;
}

void afl::Function::setRawFunctionString(std::string string)
{
    if(string != d_ptr->rawString) {
        d_ptr->rawString = std::move(string);
        d_ptr->formattedString.clear();
        d_ptr->optimizedString.clear();
        d_ptr->formattedTree.clear();
        d_ptr->optimizedTree.clear();
    }
}

bool afl::Function::format(FunctionFormatState formatState)
{
    if(formatState == FunctionFormatState::Raw) {
        return true;
    } else if(formatState == FunctionFormatState::Formatted) {

    }
    return false;
}

std::string afl::Function::getFunctionString(FunctionFormatState formatState)
{
    return std::string();
}

bool afl::Function::createSyntaxTree(SyntaxTreeFormatState formatState)
{
    return false;
}

afl::SyntaxTree<afl::Token<std::string>> afl::Function::getSyntaxTree(SyntaxTreeFormatState formatState)
{
    return afl::SyntaxTree<afl::Token<std::string>>();
}

double afl::Function::calculate(double value)
{
    return 0;
}

double *afl::Function::calculate(double *values, int count, bool createNew)
{
    return nullptr;
}

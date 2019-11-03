#ifndef AFUNCTIONLIBRARY_FUNCTION_H
#define AFUNCTIONLIBRARY_FUNCTION_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include <string>
#include <memory>
#include <vector>

#include "AFunctionLibrary/syntaxtree.h"
#include "AFunctionLibrary/token.h"

namespace afl
{
    AFUNCTIONLIBRARY_EXPORT std::vector<std::shared_ptr<Token<std::string>>> getTokens(const std::vector<std::string>& stringTokens);
    AFUNCTIONLIBRARY_EXPORT std::vector<std::shared_ptr<Token<std::string>>> parseTokens(const std::string& string);

    template<typename T>
    AFUNCTIONLIBRARY_EXPORT std::vector<std::shared_ptr<Token<T>>> shuntingYard(const std::vector<std::shared_ptr<Token<T>>>& tokens);

    template<typename T>
    AFUNCTIONLIBRARY_EXPORT SyntaxTree<std::shared_ptr<Token<T>>> generateSyntaxTree(const std::vector<std::shared_ptr<Token<T>>>& tokens);
    AFUNCTIONLIBRARY_EXPORT SyntaxTree<std::shared_ptr<Token<std::string>>> generateSyntaxTree(const std::string& string);

    enum class FunctionFormatState
    {
        Raw,
        Formatted,
        Optimized
    };

    enum class SyntaxTreeFormatState
    {
        Formatted,
        Optimized
    };

    namespace detail
    {
        struct AFUNCTIONLIBRARY_NO_EXPORT FunctionPrivate;
    }

    class AFUNCTIONLIBRARY_EXPORT Function
    {
    public:
        Function();
        explicit Function(std::string string);
        Function(const Function& other);
        Function(Function&& other) noexcept;
        virtual ~Function();

        Function& operator=(const Function& other);
        Function& operator=(Function&& other) noexcept;

        void setRawFunctionString(std::string string);
        bool format(const FunctionFormatState& formatState);
        std::string getFunctionString(const FunctionFormatState& formatState = FunctionFormatState::Raw);

        bool createSyntaxTree(const SyntaxTreeFormatState& formatState);
        SyntaxTree<Token<std::string>> getSyntaxTree(const SyntaxTreeFormatState& formatState = SyntaxTreeFormatState::Formatted);

        double* calculate(double* values, int count, bool createNew = true);

    private:
        detail::FunctionPrivate* d_ptr;
    };
}


#endif //AFUNCTIONLIBRARY_FUNCTION_H

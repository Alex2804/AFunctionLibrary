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
        class AFUNCTIONLIBRARY_NO_EXPORT FunctionPrivate;
    }

    class AFUNCTIONLIBRARY_EXPORT Function
    {
    public:
        Function();
        explicit Function(std::string rawFunction);
        Function(const Function& other);
        Function(Function&& other) noexcept;
        virtual ~Function();

        Function& operator=(const Function& other);
        Function& operator=(Function&& other) noexcept;

        void setRawFunctionString(std::string string);
        bool format(FunctionFormatState formatState);
        std::string getFunctionString(FunctionFormatState formatState = FunctionFormatState::Raw);

        bool createSyntaxTree(SyntaxTreeFormatState formatState);
        SyntaxTree<Token<std::string>> getSyntaxTree(SyntaxTreeFormatState formatState = SyntaxTreeFormatState::Formatted);

        double calculate(double value);
        double* calculate(double* values, int count, bool createNew = true);

    private:
        detail::FunctionPrivate* d_ptr;
    };
}


#endif //AFUNCTIONLIBRARY_FUNCTION_H

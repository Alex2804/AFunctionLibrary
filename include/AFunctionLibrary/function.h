#ifndef AFUNCTIONLIBRARY_FUNCTION_H
#define AFUNCTIONLIBRARY_FUNCTION_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include <string>
#include <memory>

#include "AFunctionLibrary/syntaxtree.h"

namespace afl
{
    namespace detail
    {
        struct AFUNCTIONLIBRARY_NO_EXPORT FunctionPrivate;
    }

    enum class AFUNCTIONLIBRARY_EXPORT FunctionFormatState
    {
        Raw,
        Formatted,
        Optimized
    };

    enum class AFUNCTIONLIBRARY_EXPORT SyntaxTreeFormatState
    {
        Formatted,
        Optimized
    };

    class AFUNCTIONLIBRARY_EXPORT Function
    {
    public:
        Function();
        explicit Function(std::string s);
        Function(const Function& other);
        Function(Function&& other) noexcept;
        virtual ~Function();

        Function& operator=(const Function& other);
        Function& operator=(Function&& other) noexcept;

        void setRawFunctionString(std::string s);
        bool format(const FunctionFormatState& ffs);
        std::string getFunctionString(const FunctionFormatState& ffs = FunctionFormatState::Raw);

        bool createSyntaxTree(const SyntaxTreeFormatState& stfs);

        double* calculate(double* values, int count, bool createNew = true);

    private:
        std::unique_ptr<detail::FunctionPrivate> d_ptr;
    };
}


#endif //AFUNCTIONLIBRARY_FUNCTION_H

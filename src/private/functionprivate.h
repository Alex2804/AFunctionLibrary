#ifndef AFUNCTIONLIBRARY_FUNCTIONPRIVATE_H
#define AFUNCTIONLIBRARY_FUNCTIONPRIVATE_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include <string>
#include <memory>

#include "AFunctionLibrary/syntaxtree.h"
#include "AFunctionLibrary/token.h"
#include "resourcemanager.h"

namespace afl
{
    namespace detail
    {
        class AFUNCTIONLIBRARY_NO_EXPORT FunctionPrivate
        {
        public:
            FunctionPrivate() = default;
            explicit FunctionPrivate(std::string rawString);
            FunctionPrivate(const FunctionPrivate& other) = default;
            FunctionPrivate(FunctionPrivate&& other) noexcept = default;
            ~FunctionPrivate() = default;

            FunctionPrivate& operator=(const FunctionPrivate& other) = default;
            FunctionPrivate& operator=(FunctionPrivate&& other) noexcept = default;

            std::string rawString;
            std::string formattedString;
            std::string optimizedString;

            SyntaxTree<Token<std::string>> formattedTree;
            SyntaxTree<Token<std::string>> optimizedTree;
        };
    }
}

#endif //AFUNCTIONLIBRARY_FUNCTIONPRIVATE_H

#ifndef AFUNCTIONLIBRARY_SYNTAXTREEHANDLER_H
#define AFUNCTIONLIBRARY_SYNTAXTREEHANDLER_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include <vector>
#include <memory>

#include "AFunctionLibrary/syntaxtree.h"
#include "AFunctionLibrary/token.h"
#include "resourcemanager.h"

namespace afl
{
    namespace detail
    {
        template<typename T>
        AFUNCTIONLIBRARY_NO_EXPORT std::vector<std::shared_ptr<const Token<T>>> shuntingYard(const std::vector<std::shared_ptr<const Token<T>>>& tokens);
        template<typename T>
        AFUNCTIONLIBRARY_NO_EXPORT SyntaxTree<std::shared_ptr<const Token<T>>> generateSyntaxTree(const std::vector<std::shared_ptr<const Token<T>>>& tokens);

        class AFUNCTIONLIBRARY_NO_EXPORT SyntaxTreeHandler
        {
            explicit SyntaxTreeHandler(std::shared_ptr<ResourceManager> resourceManager);
            SyntaxTreeHandler(const SyntaxTreeHandler& other) = default;
            SyntaxTreeHandler(SyntaxTreeHandler&& other) noexcept = default;
            ~SyntaxTreeHandler() = default;

            SyntaxTreeHandler& operator=(const SyntaxTreeHandler& other) = default;
            SyntaxTreeHandler& operator=(SyntaxTreeHandler&& other) noexcept = default;

            std::vector<std::shared_ptr<const Token<std::string>>> parseTokens(std::string string);

            SyntaxTree<std::shared_ptr<const Token<std::string>>> generateSyntaxTree(std::string string);

            std::shared_ptr<ResourceManager> m_resourceManager;
        };
    }
}


#endif //AFUNCTIONLIBRARY_SYNTAXTREEHANDLER_H

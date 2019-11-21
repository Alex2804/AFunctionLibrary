#include "gtest/gtest.h"

#include <memory>

#include "../src/private/functionformatter.h"
#include "../src/private/resourcemanager.h"

GTEST_TEST(function_formatter_Test, formatTokenAliases)
{
    auto resourceManager = std::make_shared<afl::detail::ResourceManager>();
    resourceManager->load(afl::detail::ResourceType::Extension, "../extensions/tokens");
    afl::detail::FunctionFormatter formatter(resourceManager);
    std::string formatted = formatter.formatFunction("4+2*absolute(3)");
    std::cout << formatted;
}
#include "gtest/gtest.h"

#include <memory>

#include "../src/private/functionformatter.h"
#include "../src/private/resourcemanager.h"

GTEST_TEST(FunctionFormatter_Test, formatTokenAliases)
{
    auto resourceManager = std::make_shared<afl::detail::ResourceManager>();
    resourceManager->load("../extensions/tokens", afl::detail::ResourceType::Extension);
    afl::detail::FunctionFormatter formatter(resourceManager);
    std::string formatted = formatter.formatFunction("4+2*absolute(3)");
    std::cout << formatted;
}
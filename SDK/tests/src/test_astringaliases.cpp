#include "../../AFunctionLibrarySDK/libs/ACUtils/tests/include/ACUtilsTest/acheck.h"
#include "../../AFunctionLibrarySDK/libs/ACUtils/include/ACUtils/macros.h"

#include "../../AFunctionLibrarySDK/include/AFunctionLibrarySDK/astringaliases.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    ACUTILS_EXTERN_C Suite* private_AFunctionLibrarySDKTest_AStringAliases_getTestSuite(void)
    {
        Suite *s;
        TCase *test_case_AStringAliases_construct_increment_decrement_destruct;

        s = suite_create("AStringAliases Test Suite");

        test_case_AStringAliases_construct_increment_decrement_destruct = tcase_create("AStringAliases Test Case: AStringAliases_construct / AStringAliases_incrementRefCount / AStringAliases_decrementRefCount / AStringAliases_destruct");
        tcase_add_test(test_case_AStringAliases_construct_increment_decrement_destruct, test_AToken_construct_destruct);
        suite_add_tcase(s, test_case_AStringAliases_construct_increment_decrement_destruct);

        return s;
    }

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

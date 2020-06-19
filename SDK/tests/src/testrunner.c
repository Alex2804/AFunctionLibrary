#include "check.h"

#include "stdlib.h"

#include "../../AFunctionLibrarySDK/libs/ACUtils/include/ACUtils/macros.h"

#include "../../AFunctionLibrarySDK/private/macros.h"

#ifndef __cplusplus
#   undef ACUTILS_EXTERN_C
#   define ACUTILS_EXTERN_C extern
#endif

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    ACUTILS_EXTERN_C Suite* private_AFunctionLibrarySDKTest_AToken_getTestSuite(void);
    ACUTILS_EXTERN_C Suite* private_AFunctionLibrarySDKTest_ATokenGroup_getTestSuite(void);
    ACUTILS_EXTERN_C Suite* private_AFunctionLibrarySDKTest_AStringAliases_getTestSuite(void);
    ACUTILS_EXTERN_C Suite* private_AFunctionLibrarySDKTest_ATokenNode_getTestSuite(void);

    int main(void)
    {
        int numberFailed = 0;
        SRunner *runner;

        runner = srunner_create(private_AFunctionLibrarySDKTest_AToken_getTestSuite());
        srunner_set_fork_status(runner, CK_NOFORK);
        srunner_run_all(runner, CK_NORMAL);
        numberFailed += srunner_ntests_failed(runner);
        srunner_free(runner);

        runner = srunner_create(private_AFunctionLibrarySDKTest_ATokenGroup_getTestSuite());
        srunner_set_fork_status(runner, CK_NOFORK);
        srunner_run_all(runner, CK_NORMAL);
        numberFailed += srunner_ntests_failed(runner);
        srunner_free(runner);

        runner = srunner_create(private_AFunctionLibrarySDKTest_AStringAliases_getTestSuite());
        srunner_set_fork_status(runner, CK_NOFORK);
        srunner_run_all(runner, CK_NORMAL);
        numberFailed += srunner_ntests_failed(runner);
        srunner_free(runner);

        runner = srunner_create(private_AFunctionLibrarySDKTest_ATokenNode_getTestSuite());
        srunner_set_fork_status(runner, CK_NOFORK);
        srunner_run_all(runner, CK_NORMAL);
        numberFailed += srunner_ntests_failed(runner);
        srunner_free(runner);

        return (numberFailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

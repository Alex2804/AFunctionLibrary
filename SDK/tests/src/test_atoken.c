#include "../../AFunctionLibrarySDK/libs/ACUtils/tests/include/ACUtilsTest/acheck.h"

#include "../../AFunctionLibrarySDK/include/AFunctionLibrarySDK/atoken.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    START_TEST(test_AToken_construct_destruct)
    {
        struct AToken *token = AToken_construct("token value", DELIMITER, RIGHT, 666, 42);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token->dptr);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(token), 1);
        ACUTILSTEST_ASSERT(AString_equalsCString(token->value, "token value"));
        ACUTILSTEST_ASSERT_INT_EQ(token->type, DELIMITER);
        ACUTILSTEST_ASSERT_INT_EQ(token->associativity, RIGHT);
        ACUTILSTEST_ASSERT_UINT_EQ(token->precedence, 666);
        ACUTILSTEST_ASSERT_UINT_EQ(token->parameterCount, 42);
        AToken_destruct(token);
    }
    END_TEST
    START_TEST(test_AToken_construct_increment_decrementAndDestruct)
    {
        struct AToken *token = AToken_construct("token value", DELIMITER, RIGHT, 666, 42);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token->dptr);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(token), 1);
        ACUTILSTEST_ASSERT(AString_equalsCString(token->value, "token value"));
        ACUTILSTEST_ASSERT_INT_EQ(token->type, DELIMITER);
        ACUTILSTEST_ASSERT_INT_EQ(token->associativity, RIGHT);
        ACUTILSTEST_ASSERT_UINT_EQ(token->precedence, 666);
        ACUTILSTEST_ASSERT_UINT_EQ(token->parameterCount, 42);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_incrementRefCount(token), 2);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token->dptr);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(token), 2);
        ACUTILSTEST_ASSERT(AString_equalsCString(token->value, "token value"));
        ACUTILSTEST_ASSERT_INT_EQ(token->type, DELIMITER);
        ACUTILSTEST_ASSERT_INT_EQ(token->associativity, RIGHT);
        ACUTILSTEST_ASSERT_UINT_EQ(token->precedence, 666);
        ACUTILSTEST_ASSERT_UINT_EQ(token->parameterCount, 42);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_decrementRefCount(token), 1);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token->dptr);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(token), 1);
        ACUTILSTEST_ASSERT(AString_equalsCString(token->value, "token value"));
        ACUTILSTEST_ASSERT_INT_EQ(token->type, DELIMITER);
        ACUTILSTEST_ASSERT_INT_EQ(token->associativity, RIGHT);
        ACUTILSTEST_ASSERT_UINT_EQ(token->precedence, 666);
        ACUTILSTEST_ASSERT_UINT_EQ(token->parameterCount, 42);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_decrementRefCount(token), 0);
    }
    END_TEST

    START_TEST(test_AToken_equals)
    {
        struct AToken *t1 = AToken_construct("token value", DELIMITER, RIGHT, 666, 42);
        struct AToken *t2 = AToken_construct("token value", DELIMITER, RIGHT, 666, 42);
        ACUTILSTEST_ASSERT(AToken_equals(nullptr, nullptr));
        ACUTILSTEST_ASSERT(!AToken_equals(nullptr, t2));
        ACUTILSTEST_ASSERT(!AToken_equals(t1, nullptr));
        ACUTILSTEST_ASSERT(AToken_equals(t1, t1));
        ACUTILSTEST_ASSERT(AToken_equals(t2, t2));
        ACUTILSTEST_ASSERT(AToken_equals(t1, t2));
        AString_set(t1->value, 0, 'T');
        ACUTILSTEST_ASSERT(!AToken_equals(t1, t2));
        AString_set(t1->value, 0, 't');
        t1->type = CONSTANT;
        ACUTILSTEST_ASSERT(!AToken_equals(t1, t2));
        t1->type = DELIMITER;
        t1->associativity = NONE;
        ACUTILSTEST_ASSERT(!AToken_equals(t1, t2));
        t1->associativity = RIGHT;
        t1->precedence += 1;
        ACUTILSTEST_ASSERT(!AToken_equals(t1, t2));
        t1->precedence -= 1;
        t1->parameterCount += 1;
        ACUTILSTEST_ASSERT(!AToken_equals(t1, t2));
        AToken_destruct(t1);
        AToken_destruct(t2);
    }
    END_TEST

    ACUTILS_EXTERN_C Suite* private_AFunctionLibrarySDKTest_AToken_getTestSuite(void)
    {
        Suite *s;
        TCase *test_case_AToken_construct_increment_decrement_destruct, *test_case_AToken_equals;

        s = suite_create("AToken Test Suite");

        test_case_AToken_construct_increment_decrement_destruct = tcase_create("AToken Test Case: AToken_construct / AToken_incrementRefCount / AToken_decrementRefCount / AToken_destruct");
        tcase_add_test(test_case_AToken_construct_increment_decrement_destruct, test_AToken_construct_destruct);
        tcase_add_test(test_case_AToken_construct_increment_decrement_destruct, test_AToken_construct_increment_decrementAndDestruct);
        suite_add_tcase(s, test_case_AToken_construct_increment_decrement_destruct);

        test_case_AToken_equals = tcase_create("AToken Test Case: AToken_equals");
        tcase_add_test(test_case_AToken_equals, test_AToken_equals);
        suite_add_tcase(s, test_case_AToken_equals);

        return s;
    }

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

#include "../../AFunctionLibrarySDK/libs/ACUtils/tests/include/ACUtilsTest/acheck.h"

#include "../../AFunctionLibrarySDK/include/AFunctionLibrarySDK/atokengroup.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    START_TEST(test_ATokenGroup_construct_destruct)
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
        struct ATokenGroup *tokenGroup = ATokenGroup_construct(token, false);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token->dptr);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(token), 2);
        ACUTILSTEST_ASSERT(AString_equalsCString(token->value, "token value"));
        ACUTILSTEST_ASSERT_INT_EQ(token->type, DELIMITER);
        ACUTILSTEST_ASSERT_INT_EQ(token->associativity, RIGHT);
        ACUTILSTEST_ASSERT_UINT_EQ(token->precedence, 666);
        ACUTILSTEST_ASSERT_UINT_EQ(token->parameterCount, 42);
        ATokenGroup_destruct(tokenGroup);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token->dptr);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(token), 1);
        ACUTILSTEST_ASSERT(AString_equalsCString(token->value, "token value"));
        ACUTILSTEST_ASSERT_INT_EQ(token->type, DELIMITER);
        ACUTILSTEST_ASSERT_INT_EQ(token->associativity, RIGHT);
        ACUTILSTEST_ASSERT_UINT_EQ(token->precedence, 666);
        ACUTILSTEST_ASSERT_UINT_EQ(token->parameterCount, 42);
        tokenGroup = ATokenGroup_construct(token, true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token);
        ACUTILSTEST_ASSERT_PTR_NONNULL(token->dptr);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(token), 1);
        ACUTILSTEST_ASSERT(AString_equalsCString(token->value, "token value"));
        ACUTILSTEST_ASSERT_INT_EQ(token->type, DELIMITER);
        ACUTILSTEST_ASSERT_INT_EQ(token->associativity, RIGHT);
        ACUTILSTEST_ASSERT_UINT_EQ(token->precedence, 666);
        ACUTILSTEST_ASSERT_UINT_EQ(token->parameterCount, 42);
        ATokenGroup_destruct(tokenGroup);
        ATokenGroup_destruct(nullptr);
    }
    END_TEST
    START_TEST(test_ATokenGroup_construct_destruct_nullptr)
    {
        ACUTILSTEST_ASSERT_PTR_NULL(ATokenGroup_construct(nullptr, true));
        ACUTILSTEST_ASSERT_PTR_NULL(ATokenGroup_construct(nullptr, false));
        ATokenGroup_destruct(nullptr);
    }
    END_TEST

    START_TEST(test_ATokenGroup_equals)
    {
        size_t a1[] = {3, 4};
        size_t v1, v2;
        struct ATokenGroup *t1 = ATokenGroup_construct(AToken_construct("token value", DELIMITER, RIGHT, 666, 42), true);
        struct ATokenGroup *t2 = ATokenGroup_construct(AToken_construct("token value", DELIMITER, RIGHT, 666, 42), true);
        struct ATokenGroup *t3 = ATokenGroup_construct(AToken_construct("token value", DELIMITER, NONE, 666, 42), true);
        ACUTILSTEST_ASSERT(ATokenGroup_equals(nullptr, nullptr));
        ACUTILSTEST_ASSERT(!ATokenGroup_equals(nullptr, t2));
        ACUTILSTEST_ASSERT(!ATokenGroup_equals(t1, nullptr));
        ACUTILSTEST_ASSERT(ATokenGroup_equals(t1, t2));
        ACUTILSTEST_ASSERT(!ATokenGroup_equals(t1, t3));
        ACUTILSTEST_ASSERT(!ATokenGroup_equals(t2, t3));
        ACUTILSTEST_ASSERT(ATokenGroup_equals(t1, t1));
        ACUTILSTEST_ASSERT(ATokenGroup_equals(t2, t2));
        ACUTILSTEST_ASSERT(ATokenGroup_equals(t3, t3));
        ADynArray_appendArray(t1->groupID, &a1, 2);
        ACUTILSTEST_ASSERT(!ATokenGroup_equals(t1, t2));
        ACUTILSTEST_ASSERT(!ATokenGroup_equals(t2, t3));
        ACUTILSTEST_ASSERT(!ATokenGroup_equals(t1, t3));
        ADynArray_appendArray(t2->groupID, &a1, 2);
        ACUTILSTEST_ASSERT(ATokenGroup_equals(t1, t2));
        ACUTILSTEST_ASSERT(!ATokenGroup_equals(t2, t3));
        ACUTILSTEST_ASSERT(!ATokenGroup_equals(t1, t3));
        ADynArray_appendArray(t3->groupID, &a1, 2);
        ACUTILSTEST_ASSERT(ATokenGroup_equals(t1, t2));
        ACUTILSTEST_ASSERT(!ATokenGroup_equals(t2, t3));
        ACUTILSTEST_ASSERT(!ATokenGroup_equals(t1, t3));
        v1 = 0;
        ADynArray_append(t1->groupID, v1);
        ADynArray_append(t2->groupID, v1);
        v2 = 1;
        ADynArray_append(t3->groupID, v2);
        ACUTILSTEST_ASSERT(ATokenGroup_equals(t1, t2));
        ACUTILSTEST_ASSERT(!ATokenGroup_equals(t2, t3));
        ACUTILSTEST_ASSERT(!ATokenGroup_equals(t1, t3));
        ATokenGroup_destruct(t1);
        ATokenGroup_destruct(t2);
        ATokenGroup_destruct(t3);
    }
    END_TEST

    START_TEST(test_ATokenGroup_clone)
    {
        size_t a1[] = {3, 4};
        struct ATokenGroup *clone, *tokenGroup = ATokenGroup_construct(AToken_construct("token value", DELIMITER, RIGHT, 666, 42), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(tokenGroup);
        ACUTILSTEST_ASSERT(ADynArray_appendArray(tokenGroup->groupID, &a1, 2));
        clone = ATokenGroup_clone(tokenGroup);
        ACUTILSTEST_ASSERT_PTR_NONNULL(clone);
        ACUTILSTEST_ASSERT(AToken_equals(tokenGroup->token, clone->token));
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(tokenGroup->token), 2);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(clone->token), 2);
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(clone->groupID), 2);
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_get(clone->groupID, 0), 3);
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_get(clone->groupID, 1), 4);
        ATokenGroup_destruct(tokenGroup);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(clone->token), 1);
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_get(clone->groupID, 0), 3);
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_get(clone->groupID, 1), 4);
        ATokenGroup_destruct(clone);
    }
    END_TEST
    START_TEST(test_ATokenGroup_clone_nullptr)
    {
        ACUTILSTEST_ASSERT_PTR_NULL(ATokenGroup_clone(nullptr));
    }
    END_TEST

    ACUTILS_EXTERN_C Suite* private_AFunctionLibrarySDKTest_ATokenGroup_getTestSuite(void)
    {
        Suite *s;
        TCase *test_case_ATokenGroup_construct_destruct, *test_case_ATokenGroup_equals, *test_case_ATokenGroup_clone;

        s = suite_create("ATokenGroup Test Suite");

        test_case_ATokenGroup_construct_destruct = tcase_create("ATokenGroup Test Case: ATokenGroup_construct / ATokenGroup_destruct");
        tcase_add_test(test_case_ATokenGroup_construct_destruct, test_ATokenGroup_construct_destruct);
        tcase_add_test(test_case_ATokenGroup_construct_destruct, test_ATokenGroup_construct_destruct_nullptr);
        suite_add_tcase(s, test_case_ATokenGroup_construct_destruct);

        test_case_ATokenGroup_equals = tcase_create("ATokenGroup Test Case: ATokenGroup_equals");
        tcase_add_test(test_case_ATokenGroup_equals, test_ATokenGroup_equals);
        suite_add_tcase(s, test_case_ATokenGroup_equals);

        test_case_ATokenGroup_clone = tcase_create("ATokenGroup Test Case: ATokenGroup_clone");
        tcase_add_test(test_case_ATokenGroup_clone, test_ATokenGroup_clone);
        tcase_add_test(test_case_ATokenGroup_clone, test_ATokenGroup_clone_nullptr);
        suite_add_tcase(s, test_case_ATokenGroup_clone);

        return s;
    }

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

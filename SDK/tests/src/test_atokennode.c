#include "../../AFunctionLibrarySDK/libs/ACUtils/tests/include/ACUtilsTest/acheck.h"

#include "../../AFunctionLibrarySDK/include/AFunctionLibrarySDK/atokennode.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    START_TEST(test_ATokenNode_construct_destruct)
    {
        struct ATokenNode *plusNode, *mulNode, *oneNode, *twoNode, *threeNode;
        struct AToken *threeToken;
        plusNode = ATokenNode_construct(AToken_construct("+", OPERATOR, LEFT, 1, 2), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(plusNode);
        mulNode = ATokenNode_construct(AToken_construct("*", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(mulNode);
        oneNode = ATokenNode_construct(AToken_construct("1", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(oneNode);
        twoNode = ATokenNode_construct(AToken_construct("2", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(twoNode);
        threeToken = AToken_construct("3", NUMBER, NONE, 0, 0);
        ACUTILSTEST_ASSERT_PTR_NONNULL(threeToken);
        threeNode = ATokenNode_construct(threeToken, false);
        ACUTILSTEST_ASSERT_PTR_NONNULL(threeNode);
        ACUTILSTEST_ASSERT_PTR_NONNULL(plusNode->children);
        ACUTILSTEST_ASSERT_PTR_NONNULL(mulNode->children);
        ACUTILSTEST_ASSERT_PTR_NONNULL(oneNode->children);
        ACUTILSTEST_ASSERT_PTR_NONNULL(twoNode->children);
        ACUTILSTEST_ASSERT_PTR_NONNULL(threeNode->children);
        ACUTILSTEST_ASSERT(ADynArray_append(plusNode->children, mulNode));
        ACUTILSTEST_ASSERT(ADynArray_append(mulNode->children, oneNode));
        ACUTILSTEST_ASSERT(ADynArray_append(mulNode->children, twoNode));
        ACUTILSTEST_ASSERT(ADynArray_append(plusNode->children, threeNode));
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(plusNode->children), 2);
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(mulNode->children), 2);
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(oneNode->children), 0);
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(twoNode->children), 0);
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(threeNode->children), 0);
        ACUTILSTEST_ASSERT_PTR_NONNULL(plusNode->token);
        ACUTILSTEST_ASSERT_PTR_NONNULL(mulNode->token);
        ACUTILSTEST_ASSERT_PTR_NONNULL(oneNode->token);
        ACUTILSTEST_ASSERT_PTR_NONNULL(twoNode->token);
        ACUTILSTEST_ASSERT_PTR_NONNULL(threeNode->token);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(plusNode->token), 1);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(mulNode->token), 1);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(oneNode->token), 1);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(twoNode->token), 1);
        ACUTILSTEST_ASSERT_UINT_EQ(AToken_refCount(threeNode->token), 2);
        AToken_decrementRefCount(threeToken);
        ATokenNode_destruct(plusNode);
    }
    END_TEST
    START_TEST(test_ATokenNode_construct_destruct_nullptr)
    {
        ACUTILSTEST_ASSERT_PTR_NULL(ATokenNode_construct(nullptr, true));
        ACUTILSTEST_ASSERT_PTR_NULL(ATokenNode_construct(nullptr, false));
        ATokenNode_destruct(nullptr);
    }
    END_TEST

    START_TEST(test_ATokenNode_equals)
    {
        struct ATokenNode *plusNode1, *mulNode1, *oneNode1, *twoNode1, *threeNode1;
        struct ATokenNode *plusNode2, *mulNode2, *oneNode2, *twoNode2, *threeNode2;
        plusNode1 = ATokenNode_construct(AToken_construct("+", OPERATOR, LEFT, 1, 2), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(plusNode1);
        mulNode1 = ATokenNode_construct(AToken_construct("*", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(mulNode1);
        oneNode1 = ATokenNode_construct(AToken_construct("1", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(oneNode1);
        twoNode1 = ATokenNode_construct(AToken_construct("2", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(twoNode1);
        threeNode1 = ATokenNode_construct(AToken_construct("3", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(threeNode1);
        ACUTILSTEST_ASSERT(ADynArray_append(plusNode1->children, mulNode1));
        ACUTILSTEST_ASSERT(ADynArray_append(mulNode1->children, oneNode1));
        ACUTILSTEST_ASSERT(ADynArray_append(mulNode1->children, twoNode1));
        ACUTILSTEST_ASSERT(ADynArray_append(plusNode1->children, threeNode1));
        plusNode2 = ATokenNode_construct(AToken_construct("+", OPERATOR, LEFT, 1, 2), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(plusNode1);
        mulNode2 = ATokenNode_construct(AToken_construct("*", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(mulNode1);
        oneNode2 = ATokenNode_construct(AToken_construct("1", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(oneNode1);
        twoNode2 = ATokenNode_construct(AToken_construct("2", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(twoNode1);
        threeNode2 = ATokenNode_construct(AToken_construct("3", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(threeNode1);
        ACUTILSTEST_ASSERT(ATokenNode_equals(nullptr, nullptr));
        ACUTILSTEST_ASSERT(!ATokenNode_equals(nullptr, plusNode2));
        ACUTILSTEST_ASSERT(!ATokenNode_equals(plusNode1, nullptr));
        ACUTILSTEST_ASSERT(ATokenNode_equals(plusNode1, plusNode1));
        ACUTILSTEST_ASSERT(ATokenNode_equals(plusNode2, plusNode2));
        ACUTILSTEST_ASSERT(ADynArray_append(plusNode2->children, mulNode2));
        ACUTILSTEST_ASSERT(ADynArray_append(mulNode2->children, oneNode2));
        ACUTILSTEST_ASSERT(ADynArray_append(mulNode2->children, twoNode2));
        ACUTILSTEST_ASSERT(!ATokenNode_equals(plusNode1, plusNode2));
        ACUTILSTEST_ASSERT(ADynArray_append(plusNode2->children, threeNode2));
        ACUTILSTEST_ASSERT(ATokenNode_equals(plusNode1, plusNode2));
        AString_clear(oneNode1->token->value);
        ACUTILSTEST_ASSERT(!ATokenNode_equals(plusNode1, plusNode2));
        AString_appendCString(oneNode1->token->value, "1", 1);
        ACUTILSTEST_ASSERT(ATokenNode_equals(plusNode1, plusNode2));
        ATokenNode_destruct(plusNode1);
        ATokenNode_destruct(plusNode2);
    }
    END_TEST

    START_TEST(test_ATokenNode_clone)
    {
        struct ATokenNode *clonedNode, *plusNode, *mulNode, *oneNode, *twoNode, *threeNode;
        plusNode = ATokenNode_construct(AToken_construct("+", OPERATOR, LEFT, 1, 2), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(plusNode);
        mulNode = ATokenNode_construct(AToken_construct("*", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(mulNode);
        oneNode = ATokenNode_construct(AToken_construct("1", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(oneNode);
        twoNode = ATokenNode_construct(AToken_construct("2", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(twoNode);
        threeNode = ATokenNode_construct(AToken_construct("3", NUMBER, NONE, 0, 0), true);
        ACUTILSTEST_ASSERT_PTR_NONNULL(threeNode);
        ACUTILSTEST_ASSERT(ADynArray_append(plusNode->children, mulNode));
        ACUTILSTEST_ASSERT(ADynArray_append(mulNode->children, oneNode));
        ACUTILSTEST_ASSERT(ADynArray_append(mulNode->children, twoNode));
        ACUTILSTEST_ASSERT(ADynArray_append(plusNode->children, threeNode));
        clonedNode = ATokenNode_clone(plusNode);
        ACUTILSTEST_ASSERT_PTR_NONNULL(clonedNode);
        ACUTILSTEST_ASSERT_STR_EQ(AString_buffer(clonedNode->token->value), "+");
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(clonedNode->children), 2);
        ACUTILSTEST_ASSERT_STR_EQ(AString_buffer(ADynArray_get(clonedNode->children, 0)->token->value), "*");
        ACUTILSTEST_ASSERT_STR_EQ(AString_buffer(ADynArray_get(clonedNode->children, 1)->token->value), "3");
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(ADynArray_get(clonedNode->children, 0)->children), 2);
        ACUTILSTEST_ASSERT_STR_EQ(AString_buffer(ADynArray_get(ADynArray_get(clonedNode->children, 0)->children, 0)->token->value), "1");
        ACUTILSTEST_ASSERT_STR_EQ(AString_buffer(ADynArray_get(ADynArray_get(clonedNode->children, 0)->children, 1)->token->value), "2");
        ATokenNode_destruct(plusNode);
        ATokenNode_destruct(clonedNode);
    }
    END_TEST
    START_TEST(test_ATokenNode_clone_nullptr)
    {
        ACUTILSTEST_ASSERT_PTR_NULL(ATokenNode_clone(nullptr));
    }
    END_TEST

    ACUTILS_EXTERN_C Suite* private_AFunctionLibrarySDKTest_ATokenNode_getTestSuite(void)
    {
        Suite *s;
        TCase *test_case_ATokenNode_construct_destruct, *test_case_ATokenNode_equals, *test_case_ATokenNode_clone;

        s = suite_create("ATokenNode Test Suite");

        test_case_ATokenNode_construct_destruct = tcase_create("ATokenNode Test Case: ATokenNode_construct / ATokenNode_destruct");
        tcase_add_test(test_case_ATokenNode_construct_destruct, test_ATokenNode_construct_destruct);
        tcase_add_test(test_case_ATokenNode_construct_destruct, test_ATokenNode_construct_destruct_nullptr);
        suite_add_tcase(s, test_case_ATokenNode_construct_destruct);

        test_case_ATokenNode_equals = tcase_create("ATokenNode Test Case: ATokenNode_equals");
        tcase_add_test(test_case_ATokenNode_equals, test_ATokenNode_equals);
        suite_add_tcase(s, test_case_ATokenNode_equals);

        test_case_ATokenNode_clone = tcase_create("ATokenNode Test Case: ATokenNode_clone");
        tcase_add_test(test_case_ATokenNode_clone, test_ATokenNode_clone);
        tcase_add_test(test_case_ATokenNode_clone, test_ATokenNode_clone_nullptr);
        suite_add_tcase(s, test_case_ATokenNode_clone);

        return s;
    }

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

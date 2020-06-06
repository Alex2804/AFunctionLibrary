#include "../../AFunctionLibrarySDK/libs/ACUtils/tests/include/ACUtilsTest/acheck.h"

#include "../../AFunctionLibrarySDK/include/AFunctionLibrarySDK/astringaliases.h"

PRIVATE_AFUNCTIONLIBRARY_OPEN_NAMESPACE

    START_TEST(test_AStringAliases_construct_destruct)
    {
        struct AStringAliases *aliases = AStringAliases_construct(REGEX);
        struct AString *string = AString_construct();
        ACUTILSTEST_ASSERT(AString_appendCString(string, "test", 4));
        ACUTILSTEST_ASSERT_PTR_NONNULL(aliases);
        ACUTILSTEST_ASSERT_PTR_NONNULL(aliases->dptr);
        ACUTILSTEST_ASSERT_UINT_EQ(AStringAliases_refCount(aliases), 1);
        ACUTILSTEST_ASSERT_INT_EQ(aliases->type, REGEX);
        ACUTILSTEST_ASSERT_PTR_NONNULL(aliases->strings);
        ACUTILSTEST_ASSERT(ADynArray_append(aliases->strings, string));
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(aliases->strings), 1);
        AStringAliases_destruct(aliases);
    }
    END_TEST
    START_TEST(test_AStringAliases_construct_increment_decrementAndDestruct)
    {
        struct AStringAliases *aliases = AStringAliases_construct(REGEX);
        struct AString *string = AString_construct();
        ACUTILSTEST_ASSERT(AString_appendCString(string, "test", 4));
        ACUTILSTEST_ASSERT_PTR_NONNULL(aliases);
        ACUTILSTEST_ASSERT_PTR_NONNULL(aliases->dptr);
        ACUTILSTEST_ASSERT_UINT_EQ(AStringAliases_refCount(aliases), 1);
        ACUTILSTEST_ASSERT_INT_EQ(aliases->type, REGEX);
        ACUTILSTEST_ASSERT_PTR_NONNULL(aliases->strings);
        ACUTILSTEST_ASSERT(ADynArray_append(aliases->strings, string));
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(aliases->strings), 1);
        ACUTILSTEST_ASSERT_UINT_EQ(AStringAliases_incrementRefCount(aliases), 2);
        ACUTILSTEST_ASSERT_PTR_NONNULL(aliases);
        ACUTILSTEST_ASSERT_PTR_NONNULL(aliases->dptr);
        ACUTILSTEST_ASSERT_UINT_EQ(AStringAliases_refCount(aliases), 2);
        ACUTILSTEST_ASSERT_INT_EQ(aliases->type, REGEX);
        ACUTILSTEST_ASSERT_PTR_NONNULL(aliases->strings);
        ACUTILSTEST_ASSERT_UINT_EQ(AStringAliases_decrementRefCount(aliases), 1);
        ACUTILSTEST_ASSERT_PTR_NONNULL(aliases);
        ACUTILSTEST_ASSERT_PTR_NONNULL(aliases->dptr);
        ACUTILSTEST_ASSERT_UINT_EQ(AStringAliases_refCount(aliases), 1);
        ACUTILSTEST_ASSERT_INT_EQ(aliases->type, REGEX);
        ACUTILSTEST_ASSERT_PTR_NONNULL(aliases->strings);
        ACUTILSTEST_ASSERT_UINT_EQ(AStringAliases_decrementRefCount(aliases), 0);
    }
    END_TEST

    START_TEST(test_AStringAliases_equals)
    {
        struct AStringAliases *a1 = AStringAliases_construct(REGEX);
        struct AStringAliases *a2 = AStringAliases_construct(REGEX);
        struct AString *str = AString_construct();
        ACUTILSTEST_ASSERT(AString_appendCString(str, "test", 4));
        ACUTILSTEST_ASSERT(AStringAliases_equals(nullptr, nullptr));
        ACUTILSTEST_ASSERT(!AStringAliases_equals(nullptr, a2));
        ACUTILSTEST_ASSERT(!AStringAliases_equals(a1, nullptr));
        ACUTILSTEST_ASSERT(AStringAliases_equals(a1, a1));
        ACUTILSTEST_ASSERT(AStringAliases_equals(a2, a2));
        ACUTILSTEST_ASSERT(AStringAliases_equals(a1, a2));
        a1->type = STRING;
        ACUTILSTEST_ASSERT(!AStringAliases_equals(a1, a2));
        a1->type = REGEX;
        ACUTILSTEST_ASSERT(ADynArray_append(a1->strings, str));
        ACUTILSTEST_ASSERT(!AStringAliases_equals(a1, a2));
        ACUTILSTEST_ASSERT(ADynArray_append(a2->strings, str));
        ACUTILSTEST_ASSERT(AStringAliases_equals(a1, a2));
        ADynArray_clear(a1->strings);
        ADynArray_clear(a2->strings);
        ACUTILSTEST_ASSERT(AStringAliases_equals(a1, a2));
        AStringAliases_destruct(a1);
        AStringAliases_destruct(a2);
        AString_destruct(str);
    }
    END_TEST

    START_TEST(test_AStringAliases_appendAString)
    {
        struct AStringAliases *aliases = AStringAliases_construct(STRING);
        struct AString *str = AString_construct();
        ACUTILSTEST_ASSERT(AString_appendCString(str, "test1", 5));
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(aliases->strings), 0);
        ACUTILSTEST_ASSERT(!AStringAliases_appendAString(nullptr, str, true));
        str = AString_construct();
        ACUTILSTEST_ASSERT(AString_appendCString(str, "test1", 5));
        ACUTILSTEST_ASSERT(!AStringAliases_appendAString(nullptr, str, false));
        ACUTILSTEST_ASSERT(!AStringAliases_appendAString(aliases, nullptr, true));
        ACUTILSTEST_ASSERT(!AStringAliases_appendAString(aliases, nullptr, false));
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(aliases->strings), 0);
        ACUTILSTEST_ASSERT(AStringAliases_appendAString(aliases, str, false));
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(aliases->strings), 1);
        ACUTILSTEST_ASSERT(!AStringAliases_appendAString(aliases, str, true));
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(aliases->strings), 1);
        str = AString_construct();
        ACUTILSTEST_ASSERT(AString_appendCString(str, "test2", 5));
        ACUTILSTEST_ASSERT(AStringAliases_appendAString(aliases, str, true));
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(aliases->strings), 2);
        AStringAliases_destruct(aliases);
    }
    END_TEST

    START_TEST(test_AStringAliases_appendADynStringArray)
    {
        struct AStringAliases *aliases = AStringAliases_construct(STRING);
        struct ADynStringArray *dynArray = ADynArray_construct(struct ADynStringArray);
        struct AString *str = AString_construct();
        ACUTILSTEST_ASSERT(AString_appendCString(str, "test1", 5));
        ACUTILSTEST_ASSERT(ADynArray_append(dynArray, str));
        str = AString_construct();
        ACUTILSTEST_ASSERT(AString_appendCString(str, "test2", 5));
        ACUTILSTEST_ASSERT(ADynArray_append(dynArray, str));
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(aliases->strings), 0);
        AStringAliases_appendADynStringArray(nullptr, dynArray, true);
        dynArray = ADynArray_construct(struct ADynStringArray);
        str = AString_construct();
        ACUTILSTEST_ASSERT(AString_appendCString(str, "test1", 5));
        ACUTILSTEST_ASSERT(ADynArray_append(dynArray, str));
        str = AString_construct();
        ACUTILSTEST_ASSERT(AString_appendCString(str, "test2", 5));
        ACUTILSTEST_ASSERT(ADynArray_append(dynArray, str));
        AStringAliases_appendADynStringArray(nullptr, dynArray, false);
        AStringAliases_appendADynStringArray(aliases, nullptr, true);
        AStringAliases_appendADynStringArray(aliases, nullptr, false);
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(aliases->strings), 0);
        AStringAliases_appendADynStringArray(aliases, dynArray, false);
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(aliases->strings), 2);
        AStringAliases_appendADynStringArray(aliases, dynArray, true);
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(aliases->strings), 2);
        dynArray = ADynArray_construct(struct ADynStringArray);
        str = AString_construct();
        ACUTILSTEST_ASSERT(AString_appendCString(str, "test2", 5));
        ACUTILSTEST_ASSERT(ADynArray_append(dynArray, str));
        str = AString_construct();
        ACUTILSTEST_ASSERT(AString_appendCString(str, "test3", 5));
        ACUTILSTEST_ASSERT(ADynArray_append(dynArray, str));
        AStringAliases_appendADynStringArray(aliases, dynArray, true);
        ACUTILSTEST_ASSERT_UINT_EQ(ADynArray_size(aliases->strings), 3);
        AStringAliases_destruct(aliases);
    }
    END_TEST

    ACUTILS_EXTERN_C Suite* private_AFunctionLibrarySDKTest_AStringAliases_getTestSuite(void)
    {
        Suite *s;
        TCase *test_case_AStringAliases_construct_increment_decrement_destruct, *test_case_AStringAliases_equals,
              *test_case_AStringAliases_appendAString, *test_case_AStringAliases_appendADynStringArray;

        s = suite_create("AStringAliases Test Suite");

        test_case_AStringAliases_construct_increment_decrement_destruct = tcase_create("AStringAliases Test Case: AStringAliases_construct / AStringAliases_incrementRefCount / AStringAliases_decrementRefCount / AStringAliases_destruct");
        tcase_add_test(test_case_AStringAliases_construct_increment_decrement_destruct, test_AStringAliases_construct_destruct);
        tcase_add_test(test_case_AStringAliases_construct_increment_decrement_destruct, test_AStringAliases_construct_increment_decrementAndDestruct);
        suite_add_tcase(s, test_case_AStringAliases_construct_increment_decrement_destruct);

        test_case_AStringAliases_equals = tcase_create("AStringAliases Test Case: AStringAliases_equals");
        tcase_add_test(test_case_AStringAliases_equals, test_AStringAliases_equals);
        suite_add_tcase(s, test_case_AStringAliases_equals);

        test_case_AStringAliases_appendAString = tcase_create("AStringAliases Test Case: AStringAliases_appendAString");
        tcase_add_test(test_case_AStringAliases_appendAString, test_AStringAliases_appendAString);
        suite_add_tcase(s, test_case_AStringAliases_appendAString);

        test_case_AStringAliases_appendADynStringArray = tcase_create("AStringAliases Test Case: AStringAliases_appendADynStringArray");
        tcase_add_test(test_case_AStringAliases_appendADynStringArray, test_AStringAliases_appendAString);
        suite_add_tcase(s, test_case_AStringAliases_appendADynStringArray);

        return s;
    }

PRIVATE_AFUNCTIONLIBRARY_CLOSE_NAMESPACE

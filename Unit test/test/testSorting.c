#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "generate.h"
#include "sorting.h"
#include "parallel.h"

typedef void (*SortFunction)(int*, int, int);

typedef struct testArrays
{
    int* arrNull;
    int arrOne[1];
    int arrSorted[10];
    int arrReversed[10];
    int arrSame[10];
    int arrBigDiff[10];
} testArrays;

typedef struct testCase
{
    SortFunction func;
    int* arr;
    int* expected;
} testCase;

static void test_sort(void** state)
{
    testCase* tc = (testCase*)(*state);
    SortFunction sorting = tc->func;
    if (tc->arr == NULL)
    {
        sorting(NULL, 0, 0);
        return;
    }
    int* arr = tc->arr;
    int* expected = tc->expected;
    int max = tc->arr[0];
    int size = sizeof(tc->arr) / sizeof(tc->arr[0]);

    sorting(arr, size, max);
    assert_int_equal(tc->arr[0], expected[0]);
}

int main(void)
{
    testArrays unsortedArrays =
    {
        .arrNull = NULL,
        .arrOne = { 42 },
        .arrSorted = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },
        .arrReversed = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        .arrSame = { 10, 10, 10, 10, 10, 10, 10, 10, },
        .arrBigDiff = { 1, 10000, 2, 99, 3, 9800, 4, 9700, 5, 9600 }
    };

    testArrays expectedArrays =
    {
        .arrNull = NULL,
        .arrOne = { 42 },
        .arrSorted = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },
        .arrReversed = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        .arrSame = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
        .arrBigDiff = { 10000, 9800, 9700, 9600, 5, 4, 3, 2, 1 }
    };

    testCase tc = 
    {
        .func = max_sort_wrapper,
        .arr = unsortedArrays.arrNull,
        .expected = expectedArrays.arrNull
    };

    const struct CMUnitTest tests[] = 
    {
        cmocka_unit_test_prestate(test_sort, &tc),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
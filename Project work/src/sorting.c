#include "sorting.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(unsigned long* a, unsigned long* b)
{
    unsigned long temp = *a;
    *a = *b;
    *b = temp;
}

void quick_sort(unsigned long* arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

int partition(unsigned long* arr, int low, int high)
{
    unsigned long pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] > pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void max_sort(unsigned long* arr, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (arr[j] > arr[i])
            {
                swap(&arr[j], &arr[i]);
            }
        }
    }
}

void quick_sort_by_digit(unsigned long* arr, int low, int high, unsigned long exp) 
{
    if (low < high) 
    {
        int homogeneous = 1;
        unsigned long d0 = (arr[low] / exp) % 10;
        for (int k = low + 1; k <= high; k++) 
        {
            if ((arr[k] / exp) % 10 != d0) 
            {
                homogeneous = 0;
                break;
            }
        }
        if (homogeneous)
            return;

        int pi = partition_by_digit(arr, low, high, exp);
        quick_sort_by_digit(arr, low, pi - 1, exp);
        quick_sort_by_digit(arr, pi + 1, high, exp);
    }
}


int partition_by_digit(unsigned long* arr, int low, int high, unsigned long exp) 
{
    unsigned long pivot_digit = (arr[high] / exp) % 10;
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        unsigned long current_digit = (arr[j] / exp) % 10;
        if (current_digit < pivot_digit) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void radix_sort_quick(unsigned long* arr, int size, unsigned long max) 
{
    for (unsigned long exp = 1; max / exp > 0; exp *= 10) 
    {
        quick_sort_by_digit(arr, 0, size - 1, exp);
    }
}
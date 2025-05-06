#include "sorting.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void max_sort(int* arr, int size)
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

void quick_sort(int* arr, int low, int high)
{
    if (low < high)
    {
        int pivot = partition(arr, low, high);
        quick_sort(arr, low, pivot - 1);
        quick_sort(arr, pivot + 1, high);
    }
}

int partition(int* arr, int low, int high)
{
    int pivot = arr[high];
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

void radix_quicksort(int* arr, int size, int max) 
{
    for (int exp = 1; max / exp > 0; exp *= 10) 
    {
        quick_sort_by_digit(arr, 0, size - 1, exp);
    }
}

int partition_by_digit(int* arr, int low, int high, int exp) 
{
    int pivot = arr[high];
    int pivot_digit = (pivot / exp) % 10;
    int i = low - 1;
    
    for (int j = low; j <= high - 1; j++) 
    {
        int current_digit = (arr[j] / exp) % 10;
        if (current_digit > pivot_digit || (current_digit == pivot_digit && arr[j] > pivot))
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quick_sort_by_digit(int* arr, int low, int high, int exp) 
{
    if (low < high) 
    {
        int pi = partition_by_digit(arr, low, high, exp);
        quick_sort_by_digit(arr, low, pi - 1, exp);
        quick_sort_by_digit(arr, pi + 1, high, exp);
    }
}

void max_sort_wrapper(int* arr, int size, int unused)
{
    max_sort(arr, size);
}

void quick_sort_wrapper(int* arr, int size, int unused)
{
    quick_sort(arr, 0, size - 1);
}

void radix_quicksort_wrapper(int* arr, int size, int max)
{
    radix_quicksort(arr, size, max);
}
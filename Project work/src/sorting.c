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

void radix_sort(unsigned long* arr, int size, unsigned long max)
{
    unsigned long* output = (unsigned long*)malloc(size * sizeof(unsigned long));
    if (output == NULL) 
    {
        printf("Memory allocation failed in radix_sort\n");
        return;
    }

    for (unsigned long exp = 1; max / exp > 0; exp *= 10)
    {
        int count[10] = {0};

        for (int i = 0; i < size; i++) count[(arr[i] / exp) % 10]++;

        for (int i = 8; i >= 0; i--) count[i] += count[i + 1];

        for (int i = size - 1; i >= 0; i--)
        {
            int digit = (arr[i] / exp) % 10;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
        }

        for (int i = 0; i < size; i++) arr[i] = output[i];
    }

    free(output);
}
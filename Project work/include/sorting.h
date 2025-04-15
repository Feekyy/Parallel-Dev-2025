#ifndef SORTING_H
#define SORTING_H

void swap(int* a, int* b);

void max_sort(int* arr, int size);

void quick_sort(int* arr, int low, int high);

int partition(int* arr, int low, int high);

void radix_sort_quick(int* arr, int size, int max);

void quick_sort_by_digit(int* arr, int low, int high, int exp);

int partition_by_digit(int* arr, int low, int high, int exp); 

#endif
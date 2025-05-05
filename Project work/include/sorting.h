#ifndef SORTING_H
#define SORTING_H

// Function to swap two integers
void swap(int* a, int* b);

// Function to perform max sort
void max_sort(int* arr, int size);

// Function to perform quick sort
void quick_sort(int* arr, int low, int high);

// Function to perform partition operation in quick sort
int partition(int* arr, int low, int high);

// Function to perform radix sort using quick sort
void radix_quicksort(int* arr, int size, int max);

// Function to perform quick sort by digit in radix sort
void quick_sort_by_digit(int* arr, int low, int high, int exp);

// Function to perform partition operation in quick sort by digit in radix sort
int partition_by_digit(int* arr, int low, int high, int exp); 

#endif // SORTING_H
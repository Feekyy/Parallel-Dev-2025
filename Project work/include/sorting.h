#ifndef SORTING_H
#define SORTING_H

void swap(unsigned long* a, unsigned long* b);

void max_sort(unsigned long* arr, int size);

void quick_sort(unsigned long* arr, int low, int high);

int partition(unsigned long* arr, int low, int high);

void radix_sort(unsigned long* arr, int size, unsigned long max);

#endif
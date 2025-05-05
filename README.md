Repository for Parallel Devices Programming lesson at University of Miskolc. Editor: Vitkolczi Dániel (Neptun: XPGMCH)

Project work: Comparing several sorting algorithms and analyzing how they react when the size and/or quantity of numbers increases. And do the similar algorithm but using parallel devices.
Algorithm: maximal sorting, quicksort, radix quicksort.
Language: basic C, and openCL library for parallel code.

Comment: The final result depends heavily on the system it is running on. Especially the system memory and vram, and to a lesser extent the cpu. I tested on Intel-10400F CPU, 32 DDR4 RAM, and NVIDIA-1660 GPU (wich has 6 GB VRAM).

Conclusions based on the RW statistics file:

Maximum sort: the sort is independent or only minimally affected by the increase in the maximum of random values, and the only thing that increases the running time is how many elements it contains. Based on the results, due to the cost of threading, it would only be worthwhile for a very large number of elements (about 10^8).

Quick sort: this is the only algorithm that reacts negatively to the increase in our maximum size, as it strongly affects the partition algorithm. Parallelization is not really worth it for this algorithm, as the thread handling and the VRAM fullfilling due to recursion slow down the operation a lot.

Radix sort: In this algorithm, it is not the maximum value that matters, but how many digits it consists of. Parallelization was not invented for this algorithm.

Summary: these algorithms are explicitly sequential measurements, so when you run them in parallel, the thread management and other costs (memory management, memory shortage, etc.) will be much slower than sequential. In short: it's not worth it.
These results and conclusions are based on the fact that the arrays contained only ints, interesting results can be obtained if you need to sort a structure according to given criteria.

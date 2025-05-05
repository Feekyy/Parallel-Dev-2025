Repository for Parallel Devices Programming lesson at University of Miskolc. Editor: Vitkolczi Dániel (Neptun: XPGMCH)

Project work: Comparing several sorting algorithms and analyzing how they react when the size and/or quantity of numbers increases. And do the similar algorithm but using parallel devices.
Algorithm: maximal sorting, quicksort, radix quicksort.
Language: basic C, and openCL library for parallel code.

Comment: The final result depends heavily on the system it is running on. Especially the system memory and vram, and to a lesser extent the cpu. I tested on Intel-10400F CPU, 32 DDR4 RAM, and NVIDIA-1660 GPU (wich has 6 GB VRAM).

Conclusions based on the RW statistics file:

Max sort: The sorting time is independent of, or only minimally affected by, the increase in the maximum value of random elements. The running time depends primarily on the number of elements. According to the results, due to the overhead of threading, parallelization is only worthwhile for extremely large datasets (around 10^8 elements).

Quick sort: This is the only algorithm that is negatively affected by an increase in the maximum value, as it significantly impacts the partitioning step. Parallelization is generally not worthwhile here, because thread management and memory operations (especially VRAM pressure caused by recursion) introduce substantial overhead.

Radix sort: For this algorithm, the maximum value is irrelevant; instead, performance is influenced by the number of digits per value. Parallelization does not benefit this algorithm significantly, as it was not designed with parallel execution in mind.

Summary: These algorithms were tested in inherently sequential implementations, and when executed in parallel, the overhead from thread management and memory handling (e.g. fragmentation, memory limits) often outweighs the benefits. In short: it’s not worth parallelizing them in these cases. These results and conclusions are based on arrays containing only integers. More interesting results might emerge when sorting complex structures based on specific criteria.

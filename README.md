# Cache-Aware and Cache-Oblivious Merge Sort Performance Analysis

## Overview
This project compares the performance of different merge sort implementations—an iterative standard merge sort, a cache-aware merge sort, and a recursive cache-oblivious merge sort. By leveraging data locality and cache hierarchy, the cache-aware implementation minimizes cache misses, resulting in better performance on modern processors. The goal is to observe how each algorithm handles cache lines, memory access, and overall performance on various cache levels.

## Implementations
- **Iterative Standard Merge Sort**: A traditional merge sort algorithm.
- **Iterative Cache-Aware Merge Sort**: Merges data chunk by chunk, aligned with the cache line and L1 cache size.
- **Recursive Cache-Oblivious Merge Sort**: Implements a recursive approach without explicit cache tuning, relying on contiguous data access for better cache utilization.

## Build & Run

### Prerequisites
Ensure you have CMake installed along with a C++ compiler (such as GCC or MSVC).

### Build Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/AniDashyan/cache_aware_oblivious_merge_sort.git
   ```
2. Navigate to the project directory:
   ```bash
   cd /cache_aware_oblivious_merge_sort
   ```
3. Build the project:
   ```bash
   cmake -S . -B build
   cmake --build build --config Release
   ```
4. Run the executable:
   ```bash
   ./build/merge
   ```

### Optional Command Line Argument
You can specify the size of the input array using the `--size` option:
```bash
./build/merge --size 3000
```
If not specified, the default array size of 1000 will be used.

## Example Output
Here’s an example output of the program, including cache information and the time taken by each merge sort implementation:

```
==================== Cache Information ====================                                                                                  
L1D Cache Size:          48 KB
Cache Line Size:         64 bytes
Chunk Size:              3072 elements (12 KB)
==========================================================

================== Performance Results ==================
Iterative Merge Sort Time:         451.2 ns
Cache-Aware Merge Sort Time:       438.5 ns
Cache-Oblivious Merge Sort Time:     870.9 ns
==========================================================
```

In this example:
- The L1D cache size is 32 KB, and the cache line size is 64 bytes.
- The cache-aware merge sort outperforms both the iterative standard and cache-oblivious merge sorts, demonstrating the benefits of cache alignment.

## Performance Comparison
The program measures and displays the time taken by each implementation:
- **Standard Merge Sort Time**: Time taken by the iterative standard merge sort.
- **Cache-Aware Merge Sort Time**: Time taken by the cache-aware implementation.
- **Cache-Oblivious Merge Sort Time**: Time taken by the recursive cache-oblivious implementation.

## Additional Features
- **Cache Information**: The program automatically retrieves cache details such as L1 data cache size and cache line size, ensuring proper alignment for the cache-aware merge sort.
- **Random Data Generation**: Input arrays are filled with random values before sorting.

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "kaizen.h"
#include "cache_info.h"

int n = 1000;

// Iterative Standard Merge Sort
void iterative_standard_merge_sort(std::vector<int>& arr) {
    int n = arr.size();
    std::vector<int> temp(n);
    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = std::min(left + size - 1, n - 1);
            int right = std::min(left + 2 * size - 1, n - 1);
            int i = left, j = mid + 1, k = left;
            while (i <= mid && j <= right) {
                if (arr[i] <= arr[j]) temp[k++] = arr[i++];
                else temp[k++] = arr[j++];
            }
            while (i <= mid) temp[k++] = arr[i++];
            while (j <= right) temp[k++] = arr[j++];
            for (int p = left; p <= right; p++) arr[p] = temp[p];
        }
    }
}

// Iterative Cache-Aware Merge Sort
void iterative_cache_aware_merge_sort(std::vector<int>& arr, int chunk_size, int line_size) {
    int n = arr.size();
    chunk_size = (chunk_size / line_size) * line_size;
    if (chunk_size < line_size) chunk_size = line_size;
    std::vector<int> temp(chunk_size);

    // Phase 1: Sort initial chunks
    for (int left = 0; left < n; left += chunk_size) {
        int right = std::min(left + chunk_size - 1, n - 1);
        int chunk_len = right - left + 1;
        for (int size = 1; size < chunk_len; size *= 2) {
            for (int start = left; start < right; start += 2 * size) {
                int mid = std::min(start + size - 1, right);
                int end = std::min(start + 2 * size - 1, right);
                int i = start, j = mid + 1, k = 0;
                while (i <= mid && j <= end) {
                    if (arr[i] <= arr[j]) temp[k++] = arr[i++];
                    else temp[k++] = arr[j++];
                }
                while (i <= mid) temp[k++] = arr[i++];
                while (j <= end) temp[k++] = arr[j++];
                for (int p = 0; p < k; p++) arr[start + p] = temp[p];
            }
        }
    }

    // Phase 2: Merge chunks
    for (int size = chunk_size; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = std::min(left + size - 1, n - 1);
            int right = std::min(left + 2 * size - 1, n - 1);
            int merge_size = right - left + 1;
            if (temp.size() < merge_size) temp.resize(merge_size);
            int i = left, j = mid + 1, k = 0;
            while (i <= mid && j <= right) {
                if (arr[i] <= arr[j]) temp[k++] = arr[i++];
                else temp[k++] = arr[j++];
            }
            while (i <= mid) temp[k++] = arr[i++];
            while (j <= right) temp[k++] = arr[j++];
            for (int p = 0; p < k; p++) arr[left + p] = temp[p];
        }
    }
}

// Recursive Merge Sort
void merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (int p = 0; p < k; p++) arr[left + p] = temp[p];
}

void recursive_cache_oblivious_merge_sort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        recursive_cache_oblivious_merge_sort(arr, left, mid);
        recursive_cache_oblivious_merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}


int main(int argc, char** argv) {
    zen::cmd_args args(argv, argc);
    if (!args.is_present("--size")) {
        zen::log(zen::color::yellow("--size option is not provided. Using the default value: " + std::to_string(n)));
    } else {
        n = std::stoi(args.get_options("--size")[0]);
    }

    CacheInfo cache = get_cache_info();
    long l1d_size = (cache.l1d_size != -1) ? cache.l1d_size : 32 * 1024;
    long line_size = (cache.line_size != -1) ? cache.line_size : 64;
    int chunk_size = l1d_size / (4 * sizeof(int));
    chunk_size = (chunk_size * sizeof(int) / line_size) * line_size / sizeof(int);
    
    std::cout << "==================== Cache Information ====================\n";
    std::cout << std::left << std::setw(25) << "L1D Cache Size:" << l1d_size / 1024 << " KB\n";
    std::cout << std::left << std::setw(25) << "Cache Line Size:" << line_size << " bytes\n";
    std::cout << std::left << std::setw(25) << "Chunk Size:" << chunk_size << " elements (" << chunk_size * 4 / 1024 << " KB)\n";
    std::cout << "==========================================================\n\n";


    std::vector<int> arr_std(n), arr_cache(n), arr_oblivious(n);
    zen::generate_random(arr_std, n);
    arr_cache = arr_std;               
    arr_oblivious = arr_std;   


    zen::timer t;

    t.start();
    iterative_standard_merge_sort(arr_std);
    t.stop();
    auto std_duration = t.duration<zen::timer::nsec>();


    t.start();
    iterative_cache_aware_merge_sort(arr_cache, chunk_size, static_cast<int>(line_size));
    t.stop();
    auto cache_aware_duration = t.duration<zen::timer::nsec>();

    t.start();
    recursive_cache_oblivious_merge_sort(arr_oblivious, 0, n - 1);
    t.stop();
    auto oblivious_duration = t.duration<zen::timer::nsec>();

    std::cout << "================== Performance Results ==================\n";
    std::cout << std::fixed << std::setprecision(1); // 1 deqcimal place for readability
    std::cout << std::left << std::setw(30) << "Iterative Merge Sort Time:" 
              << std::right << std::setw(10) << std_duration.count() / 1000.0 << " ns\n";
    std::cout << std::left << std::setw(30) << "Cache-Aware Merge Sort Time:" 
              << std::right << std::setw(10) << cache_aware_duration.count() / 1000.0 << " ns\n";
    std::cout << std::left << std::setw(30) << "Cache-Oblivious Merge Sort Time:" 
              << std::right << std::setw(10) << oblivious_duration.count() / 1000.0 << " ns\n";
    std::cout << "==========================================================\n";

    return 0;
}
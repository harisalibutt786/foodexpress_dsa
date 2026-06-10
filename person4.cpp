// ================================================================
//  person4.cpp  —  PERSON 4
//  Module 7: Performance Analysis & Benchmarking
//  Module 8: Scalability Simulation
// ================================================================

#include "person4.h"

// ------------------------------------------------
//  PerformanceAnalyzer – Helper bubble sort
// ------------------------------------------------
void PerformanceAnalyzer::bubbleSort(int* arr, int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1])
                std::swap(arr[j], arr[j + 1]);
        }
    }
}

// ------------------------------------------------
//  runBenchmark – theoretical & experimental report
// ------------------------------------------------
void PerformanceAnalyzer::runBenchmark() {
    std::cout << "\n  === PERFORMANCE ANALYSIS REPORT ===\n";

    // ---- Theoretical Complexity ----
    std::cout << "\n  -- Theoretical Complexity --\n";
    std::cout << "  Max-Heap Insert/Extract   : O(log n)\n";
    std::cout << "  Linked Queue Enqueue/Deq  : O(1)\n";
    std::cout << "  BST Insert/Search (avg)   : O(log n)\n";
    std::cout << "  BST Search (worst case)   : O(n)  [unbalanced]\n";
    std::cout << "  Dijkstra Routing          : O(V^2) [adj. list + linear scan]\n";
    std::cout << "  Dynamic Array Resize      : O(n) amortized O(1) per insert\n";
    std::cout << "  Linked Stack Push/Pop     : O(1)\n";

    // ---- Experimental: heap insert timing ----
    int sizes[] = {100, 1000, 5000, 10000};
    std::cout << "\n  -- Experimental Heap Insert (microseconds) --\n";
    for (int sz : sizes) {
        Order* testHeap = new Order[sz];
        int heapSize = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < sz; ++i) {
            testHeap[heapSize] = {i, "Test", (rand() % 10) + 1, "Placed", 5, 0, "Regular"};
            // Simple heapify-up for timing (reuse OrderScheduler logic)
            int idx = heapSize;
            while (idx > 0 && testHeap[(idx - 1) / 2].priority < testHeap[idx].priority) {
                std::swap(testHeap[idx], testHeap[(idx - 1) / 2]);
                idx = (idx - 1) / 2;
            }
            ++heapSize;
        }
        auto finish = std::chrono::high_resolution_clock::now();
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
        std::cout << "    n=" << sz << " -> " << micros << " µs\n";
        delete[] testHeap;
    }

    // ---- Comparison of sorting algorithms ----
    const int arrSize = 2000;
    int *arr1 = new int[arrSize];
    int *arr2 = new int[arrSize];
    for (int i = 0; i < arrSize; ++i) {
        arr1[i] = arr2[i] = rand() % 10000;
    }
    // Bubble sort timing
    auto t1 = std::chrono::high_resolution_clock::now();
    bubbleSort(arr1, arrSize);
    auto t2 = std::chrono::high_resolution_clock::now();
    // std::sort (quick sort / introsort) timing
    auto t3 = std::chrono::high_resolution_clock::now();
    std::sort(arr2, arr2 + arrSize);
    auto t4 = std::chrono::high_resolution_clock::now();
    std::cout << "\n  -- Sorting Comparison (microseconds) --\n";
    std::cout << "    Bubble sort: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " µs\n";
    std::cout << "    std::sort   : " << std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count() << " µs\n";

    delete[] arr1; delete[] arr2;
    std::cout << "\n  [NOTE] All timings are approximate and run on the current hardware.\n";
}

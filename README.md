# QuickSelect Algorithm with Memory and Performance Analysis

This project implements the QuickSelect algorithm for finding the k-th smallest element in an unsorted array, along with comprehensive memory usage and performance analysis tools.

# LINK - https://www.youtube.com/watch?v=Hb-HSah_aBU

## Algorithm Overview

QuickSelect is an efficient selection algorithm that uses principles similar to QuickSort. It has:
- Average case time complexity: O(n)
- Worst case time complexity: O(n²)
- Space complexity: O(log n) due to recursive calls

## Features

- Implementation of QuickSelect algorithm to find the k-th smallest element
- Memory usage tracking across different platforms (Windows, macOS, Linux)
- Performance measurement for both time and memory usage
- Data export to CSV for further analysis
- Support for different test cases (random arrays, sorted arrays)

## Requirements

- C++ compiler supporting C++11 standard
- Platform-specific headers for memory measurement:
  - Windows: `windows.h`, `psapi.h`
  - macOS: `mach/mach.h`
  - Linux: `/proc/self/statm`

## How to Compile

```bash
g++ -o quickselect QuickSelect.cpp
```

## How to Run

Run the compiled executable:

```bash
./quickselect
```

## Program Options

The program provides four main options:

1. **Find kth smallest element**
   - Manual input: Enter array size, values, and k
   - Auto-generated input: Enter array size and k, values are randomly generated

2. **Measure Running Time**
   - Measures average execution time across multiple iterations
   - Reports results for different array sizes

3. **Analyze Memory Usage**
   - Measures peak memory usage for different array sizes
   - Exports data to `memory_analysis.csv`

4. **Measure both Time and Memory Performance**
   - Combined analysis of time and memory metrics
   - Exports data to `performance_results.csv`

## How to Reproduce Results

To reproduce the memory usage analysis:

1. Run the program: `./quickselect`
2. Select option 3 or 4
3. Enter the maximum size (e.g., 50000)
4. Select test type (1 for random arrays, 2 for sorted arrays)
5. The program will run the analysis and generate a CSV file with the results
6. Import the CSV file into your preferred spreadsheet software for visualization

## Code Structure

- `main()`: Program entry point and option selection
- `kthSmallest()`: Main function to find k-th smallest element
- `quickselect()`: Recursive implementation of QuickSelect algorithm
- `partition()`: Partitioning function similar to QuickSort
- Memory tracking functions:
  - `getCurrentMemoryUsage()`: Platform-specific memory measurement
  - `updatePeakMemory()`: Updates peak memory tracking
  - `resetMemoryTracking()`: Resets memory tracking variables
- Analysis functions:
  - `measurePerformance()`: Time performance analysis
  - `analyzeMemoryUsage()`: Memory usage analysis
  - `measureTimeAndMemory()`: Combined analysis

## Modifying the Code

To modify the analysis parameters:
- For memory analysis, adjust the constants in `analyzeMemoryUsage()`:
  ```cpp
  const int maxSize = 100000; // Maximum array size
  const int step = 5000;      // Size increment
  const int iterations = 5;   // Number of iterations per size
  ```

- For different k values, modify the k selection in the test functions

#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <string>

// Platform-specific memory measurement
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>
#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach.h>
#endif
#endif

using namespace std;

// Memory tracking structure
struct MemoryUsage {
    size_t peakBytes;
    size_t currentBytes;
};

// Get current memory usage in bytes
size_t getCurrentMemoryUsage() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    return 0;
#elif defined(__APPLE__) && defined(__MACH__)
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
        return info.resident_size;
    }
    return 0;
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    long rss = 0L;
    FILE* fp = fopen("/proc/self/statm", "r");
    if (fp != NULL) {
        if (fscanf(fp, "%*s%ld", &rss) == 1) {
            rss *= (size_t)sysconf(_SC_PAGESIZE);
        }
        fclose(fp);
    }
    return rss;
#else
    return 0; // Unsupported platform
#endif
}

// Global memory tracking variable
MemoryUsage memoryUsage = {0, 0};

// Function to update peak memory usage
void updatePeakMemory() {
    memoryUsage.currentBytes = getCurrentMemoryUsage();
    memoryUsage.peakBytes = max(memoryUsage.peakBytes, memoryUsage.currentBytes);
}

// Reset memory tracking
void resetMemoryTracking() {
    memoryUsage.peakBytes = 0;
    memoryUsage.currentBytes = 0;
}

int partition(vector<int>& arr, int low, int high) {
    updatePeakMemory(); // Track memory at each function call
    
    int pivot = arr[high];  // Pick the last element as the pivot
    int i = low; // index for the smaller element
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            swap(arr[i], arr[j]); // move smaller element to the left
            i++;
        }
    }
    swap(arr[i], arr[high]); // put pivot in the right position
    return i; //return pivot index
}

// Finds and returns the kth smallest element in arr
int quickselect(vector<int>& arr, int low, int high, int k) {
    updatePeakMemory(); // Track memory at each function call
    
    if (low == high) { // only one element left
        return arr[low];
    }
    int pivotIndex = partition(arr, low, high);
    if (pivotIndex == k) { // pivot is the kth element
        return arr[pivotIndex];
    } 
    else if (pivotIndex > k) { // kth element is in the left part
        return quickselect(arr, low, pivotIndex - 1, k);
    } 
    else { // kth element is in the right part
        return quickselect(arr, pivotIndex + 1, high, k);
    }
}

// Find kth smallest number
int kthSmallest(vector<int> arr, int k) {
    updatePeakMemory(); // Track memory before function execution
    int result = quickselect(arr, 0, arr.size() - 1, k - 1);
    updatePeakMemory(); // Track memory after function execution
    return result;
}

// Generate sorted array for worst case time
vector<int> worstTimeTest(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    return arr;
}

// Original measurePerformance function (kept for compatibility)
void measurePerformance() {
    int maxSize, testType;
    const int iterations = 10; // use 10 runs to find avg
    cout << "Max size(n): " << endl;
    cin >> maxSize;
    cout << "Test type: " << endl;
    cout << "1: Regular" << endl;
    cout << "2: Worst (sorted)" << endl;
    cout << "Choice: " << endl;
    cin >> testType;
    
    cout << "Perf: " << endl;
    cout << "n, Avg(ms)" << endl;
    srand(time(NULL));
    double sumTime = 0.0;
    
    for (int size = 1; size <= maxSize; size++) {
        vector<vector<int>> testInputs(iterations, vector<int>(size));

        if (testType == 1) { // Regular random generated array
            for (int iter = 0; iter < iterations; iter++) {
                for (int j = 0; j < size; j++) {
                    testInputs[iter][j] = rand() % 1000;
                }
            }
        } 
        //doesn't work???? running time still the same O(n)??
        else if (testType == 2) { // Worst-case use sorted array
            vector<int> sortedArr = worstTimeTest(size);
            for (int iter = 0; iter < iterations; iter++) {
                testInputs[iter] = sortedArr;
            }
        }
        
        // Measure avg time to find kth smallest (k=1)
        double totalTime = 0.0;
        // create test array for measure the time
        for (int iter = 0; iter < iterations; iter++) {
            vector<int> arrCopy = testInputs[iter];
            auto start = chrono::high_resolution_clock::now();
            int kth = kthSmallest(arrCopy, 1); // select the k = 1 for testing and measuring
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> dur = end - start;
            totalTime += dur.count();
        }
        double avgTime = totalTime / iterations; // find avg time in ms
        cout << "n=" << size << ", " << avgTime << "ms" << endl;
        sumTime = sumTime + avgTime;
    }
    cout << "Total Runtime: " << sumTime << "ms" << endl;
    cout << "Average for n: " << sumTime/maxSize << "ms" << endl;
}

// Run memory usage analysis with different input sizes
void analyzeMemoryUsage() {
    ofstream outFile("memory_analysis.csv");
    outFile << "Size,PeakMemoryBytes,PeakMemoryKB,PeakMemoryMB" << endl;
    
    cout << "Running Memory Analysis..." << endl;
    cout << "n, Peak Memory (KB)" << endl;
    
    const int maxSize = 100000; // Adjust as needed
    const int step = 5000;   // Step size for measurements
    const int iterations = 5; // Average over iterations
    
    for (int size = 1000; size <= maxSize; size += step) {
        double totalPeakMemory = 0;
        
        for (int iter = 0; iter < iterations; iter++) {
            // Generate data
            vector<int> arr(size);
            for (int j = 0; j < size; j++) {
                arr[j] = rand() % 1000000;
            }
            
            // Reset memory tracking
            resetMemoryTracking();
            
            // Run the algorithm
            int k = size / 2; // Find median as a representative test
            int result = kthSmallest(arr, k);
            
            // Add to total
            totalPeakMemory += memoryUsage.peakBytes;
        }
        
        // Calculate average peak memory
        double avgPeakMemory = totalPeakMemory / iterations;
        double peakMemoryKB = avgPeakMemory / 1024.0;
        double peakMemoryMB = peakMemoryKB / 1024.0;
        
        cout << size << ", " << peakMemoryKB << " KB" << endl;
        outFile << size << "," << avgPeakMemory << "," << peakMemoryKB << "," << peakMemoryMB << endl;
    }
    
    outFile.close();
    cout << "Memory analysis completed. Results saved to memory_analysis.csv" << endl;
}

// Print array helper function
void printArray(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

// Measure both time and memory performance
void measureTimeAndMemory() {
    int maxSize, testType;
    const int iterations = 10; // use 10 runs to find avg
    cout << "Max size(n): " << endl;
    cin >> maxSize;
    cout << "Test type: " << endl;
    cout << "1: Regular (random)" << endl;
    cout << "2: Worst (sorted)" << endl;
    cout << "Choice: " << endl;
    cin >> testType;
    
    cout << "Performance Results: " << endl;
    cout << "n, Avg Time(ms), Peak Memory(KB)" << endl;
    
    ofstream outFile("performance_results.csv");
    outFile << "Size,AvgTime(ms),PeakMemory(bytes),PeakMemory(KB)" << endl;
    
    srand(time(NULL));
    double sumTime = 0.0;
    
    for (int size = 1000; size <= maxSize; size += 1000) {
        vector<vector<int>> testInputs(iterations, vector<int>(size));

        if (testType == 1) { // Regular random generated array
            for (int iter = 0; iter < iterations; iter++) {
                for (int j = 0; j < size; j++) {
                    testInputs[iter][j] = rand() % 1000;
                }
            }
        } 
        else if (testType == 2) { // Worst-case use sorted array
            vector<int> sortedArr = worstTimeTest(size);
            for (int iter = 0; iter < iterations; iter++) {
                testInputs[iter] = sortedArr;
            }
        }
        
        // Measure avg time and peak memory to find kth smallest (k=size/2)
        double totalTime = 0.0;
        size_t totalPeakMemory = 0;
        
        for (int iter = 0; iter < iterations; iter++) {
            vector<int> arrCopy = testInputs[iter];
            int k = size / 2; // Use median as k
            
            resetMemoryTracking();
            
            auto start = chrono::high_resolution_clock::now();
            int kth = kthSmallest(arrCopy, k);
            auto end = chrono::high_resolution_clock::now();
            
            chrono::duration<double, milli> dur = end - start;
            totalTime += dur.count();
            totalPeakMemory += memoryUsage.peakBytes;
        }
        
        double avgTime = totalTime / iterations;
        double avgPeakMemory = static_cast<double>(totalPeakMemory) / iterations;
        double avgPeakMemoryKB = avgPeakMemory / 1024.0;
        
        cout << "n=" << size << ", " << avgTime << "ms, " << avgPeakMemoryKB << "KB" << endl;
        outFile << size << "," << avgTime << "," << avgPeakMemory << "," << avgPeakMemoryKB << endl;
        
        sumTime = sumTime + avgTime;
    }
    
    outFile.close();
    cout << "Total Runtime: " << sumTime << "ms" << endl;
    cout << "Average Time per n: " << sumTime/(maxSize/1000) << "ms" << endl;
    cout << "Results saved to performance_results.csv" << endl;
}

int main() {
    int option;
    cout << "Options: " << endl;
    cout << "1: Find kth smallest" << endl;
    cout << "2: Measure Running Time" << endl;
    cout << "3: Analyze Memory Usage" << endl;
    cout << "4: Measure Time and Memory" << endl;
    cout << "Choice: " << endl;
    cin >> option;
    
    if (option == 1) {
        int subOption;
        cout << "Input: " << endl;
        cout << "1: Manual" << endl;
        cout << "2: Auto" << endl;
        cout << "Choice: " << endl;
        cin >> subOption;
        
        vector<int> arr;
        int n, k;
        if (subOption == 1) {
            cout << "Enter Size of the Array: " << endl;
            cin >> n;
            arr.resize(n);
            cout << "Enter Values(no duplicate and no bigger than size of the array) separate by space: " << endl;
            for (int i = 0; i < n; i++) {
                cin >> arr[i];
            }
        } 
        else if (subOption == 2) {
            cout << "Size: " << endl;
            cin >> n;
            arr.resize(n);
            srand(time(NULL));
            for (int i = 0; i < n; i++) {
                arr[i] = rand() % 1000;
            }
            cout << "Arr: " << endl;
            printArray(arr);
        } 
        else {
            cout << "Invalid option" << endl;
            return 1;
        }
        
        cout << "Choose k (1-indexed): " << endl;
        cin >> k;
        
        resetMemoryTracking();
        int kth = kthSmallest(arr, k);
        
        cout << "kth smallest: " << kth << endl;
        cout << "Peak memory usage: " << (memoryUsage.peakBytes / 1024.0) << " KB" << endl;
    } 
    else if (option == 2) {
        measurePerformance();
    } 
    else if (option == 3) {
        analyzeMemoryUsage();
    }
    else if (option == 4) {
        measureTimeAndMemory();
    }
    else {
        cout << "Invalid Option" << endl;
    }
    
    return 0;
}

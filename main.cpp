#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

using namespace std;

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];  // Pick the lat element as the pivot
    int i = low; // index fo the smaller element
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            swap(arr[i], arr[j]); // move smaller element tp the left
            i++;
        }
    }
    swap(arr[i], arr[high]); // put pivot in the right position
    return i; //return pivot index
}

// Finds and returns the kth smallest element in arr
int quickselect(vector<int>& arr, int low, int high, int k) {
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

//find kth smallest number
int kthSmallest(vector<int> arr, int k) {
    return quickselect(arr, 0, arr.size() - 1, k - 1);
}

//sorted array to measure worst case time
//might need to remove this, it doesn't seem to be working
//maybe n is too small, only tested to n = 100
vector<int> worstTimeTest(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    return arr;
}

//measures avg time (10 iter) for kthSmallest
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
    }
}

void printArray(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    // Choose btw 1.the actual program to find kth smallest number and Measure the running time
    // Might need to comment the Measure Running time out in the Final submission.
    int option;
    cout << "Options: " << endl;
    cout << "1: Find kth smallest" << endl;
    cout << "2: Measure Running Time" << endl;
    cout << "Choice: " << endl;
    cin >> option;
    
    // if choose 1(the program), it will ask to choose 1. put your own value, 2.Auto genarate the value, it will be the same value evertime thou.
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
            for (int i = 0; i < n; i++) {
                arr[i] = rand() % 1000; // random number, not really.
            }
            cout << "Arr: " << endl;
            printArray(arr);
        } 
        else {
            cout << "Inv opt" << endl;
            return 1;
        }
        cout << "Chose k (1-indexed): " << endl;
        cin >> k;
        int kth = kthSmallest(arr, k);
        cout << "kth: " << kth << endl;
    } 
    else if (option == 2) {
        measurePerformance();
    } 
    else {
        cout << "Invalid Option" << endl;
    }
    
    return 0;
}
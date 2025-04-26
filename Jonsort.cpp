#include <iostream>
using namespace std;

//Testing a short data set
int testData[] = { 50, 21, 90, 1, 45, 60, 100, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
// Try do with 2D array and copy meeekail's code

// Define the dimensions for our 2D array
const int ROWS = 9;
const int MAX_COLS = 10; // Maximum number of columns in any row

// Complex 2D array with varying row lengths
int test2D[ROWS][MAX_COLS] = {
    {50, 21, 90, 42, 17, 63, 88, 0, 0, 0},                    // Row with 7 elements
    {1, 45, 60, 33, 76, 99, 0, 0, 0, 0},                      // Row with 6 elements
    {100, 2, 3, 500, 250, 175, 333, 444, 0, 0},               // Row with 8 elements
    {4, 5, 6, 412, 534, 621, 789, 832, 901, 0},               // Row with 9 elements
    {7, 8, 9, 11, 13, 15, 17, 19, 21, 23},                    // Row with 10 elements
    {10, 20, 30, 40, 50, 0, 0, 0, 0, 0},                      // Row with 5 elements
    {-5, -10, -15, -20, -25, -30, 0, 0, 0, 0},                // Row with negative numbers
    {1000, 2000, 3000, 4000, 0, 0, 0, 0, 0, 0},               // Row with 4 large numbers
    {123, 456, 789, 321, 654, 987, 135, 246, 579, 0}          // Row with 9 elements
};

// Store the actual length of each row
int rowLengths[ROWS] = {7, 6, 8, 9, 10, 5, 6, 4, 9};

int selectionSort() {
    int i, j, minIndex, temp;
    int n = sizeof(testData) / sizeof(testData[0]);
    for (i = 0; i < n - 1; i++) {
        minIndex = i;
        for (j = i + 1; j < n; j++) {
            if (testData[j] < testData[minIndex]) {
                minIndex = j;
            }
        }
        temp = testData[i];
        testData[i] = testData[minIndex];
        testData[minIndex] = temp;
    }
    cout << "Sorted array: ";
    for (i = 0; i < n; i++) {
        cout << testData[i] << " ";
    }

    cout << "\nNow let us do it with 2D array!" << endl;

    // Calculate total number of elements in 2D array
    int totalElements = 0;
    for (i = 0; i < ROWS; i++) {
        totalElements += rowLengths[i];
    }

    // Flatten the 2D array into a 1D array
    int flatArray[100]; // Assuming the total number of elements won't exceed 100
    int flatIndex = 0;
    
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < rowLengths[i]; j++) {
            flatArray[flatIndex++] = test2D[i][j];
        }
    }

    // Sort the flattened array
    for (i = 0; i < totalElements - 1; i++) {
        minIndex = i;
        for (j = i + 1; j < totalElements; j++) {
            if (flatArray[j] < flatArray[minIndex]) {
                minIndex = j;
            }
        }
        temp = flatArray[i];
        flatArray[i] = flatArray[minIndex];
        flatArray[minIndex] = temp;
    }

    // Print the sorted 1D array
    cout << "Sorted 2D array (flattened): ";
    for (i = 0; i < totalElements; i++) {
        cout << flatArray[i] << " ";
    }
    cout << endl;
}

int linearSearch() {
    int i, n = sizeof(testData) / sizeof(testData[0]);
    int target = 90; // Element to search for
    for (i = 0; i < n; i++) {
        if (testData[i] == target) {
            cout << "Element " << target << " found at index " << i << endl;
            return i;
        }
    }
    cout << "Element " << target << " not found in the array." << endl;
    return -1;
}

int main() {
    cout << "Testing 1, 2, 3"<< endl;
    selectionSort();
    cout << endl;
    linearSearch();
    cout << endl;
}
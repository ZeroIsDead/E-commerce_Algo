#include <iostream>
using namespace std;

//Testing a short data set
int testData[] = { 50, 21, 90, 1, 45, 60, 100, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
// Try do with 2D array and copy meeekail's code
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
}

int main() {
    cout << "Testing 1, 2, 3"<< endl;
    selectionSort();
    cout << endl;
}
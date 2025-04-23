#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>

using namespace std;

struct DataContainer2d {
    // All members are public
    int error; // 1 - error | 0 - fine
    string* fields; // an array containing the fields (headers)
    string** data; // a 2D array containing each line of data
    int y; // y - number of rows 
    int x; // x - number of columns
};

class Timer {
    private:
        chrono::steady_clock::time_point startTime;
        chrono::steady_clock::time_point endTime;
    
    public:
        void start() {
            startTime = chrono::steady_clock::now();
        }

        void end() {
            endTime = chrono::steady_clock::now();
            auto duration = chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    
            cout << "Time taken: " << duration.count() << " microseconds" << endl;
        }
};

class Functions {
public:
    /*
        Function to Count the number of colums in one single line, (in CSV file).
        Returns the number of columns as integer
    */
    int countColumns(const string& line) {
        int count = 1; // Start with 1 for the first field
        
        for (size_t i = 0; i < line.length(); i++) {
            if (line[i] == ',') {
                count++;
            }
        }
        
        return count;
    }
    
    /*
        Function to Count the number of rows in in CSV file. 
        Returns the number of rows as integer
    */
    int countRows(const string& filename) {
        ifstream file(filename);
        int count = 0;
        string line;
        
        if (!file.is_open()) {
            return 0;
        }
        
        while (getline(file, line)) {
            count++;
        }
        
        file.close();
        return count;
    }
    
    // Helper function to split a line by comma
    void splitLine(const string& line, string* result, int maxCols) {
        int col = 0;
        string current = "";
        
        for (size_t i = 0; i <= line.length(); i++) {
            if (i == line.length() || line[i] == ',') {
                if (col < maxCols) {
                    result[col] = current;
                    col++;
                }
                current = "";
            } else {
                current += line[i];
            }
        }
    }
    
    /*
        DataContainer2d Stuct that take the CSV filename as argument.
        FIELD names are stored in FIELDS, 1D array
        DATA is sotries in data, 2D array
        INT X stores the number of columns
        INT Y stores the number of ROWS
        INT ERROR stores 1 or 0 depending on if the struct has been created properly or not
    */
    DataContainer2d getdata(const string& filename) {
        DataContainer2d container;
        string path = "../data/"+filename;

        
        // Count rows and prepare to read the file
        int totalRows = countRows(path);
        if (totalRows <= 1) { // Need at least header row and one data row
            container.error = 1;
            return container;
        }
        
        ifstream file(path);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << path << endl;
            container.error = 1;
            return container;
        }
        
        // Read the header row to determine number of columns
        string headerLine;
        if (!getline(file, headerLine)) {
            container.error = 1;
            file.close();
            return container;
        }
        
        // Count columns and allocate memory for fields
        int numCols = countColumns(headerLine);
        container.x = numCols;
        container.fields = new string[numCols];
        
        // Parse header row
        splitLine(headerLine, container.fields, numCols);
        
        // Set dimensions and allocate memory for data
        container.y = totalRows - 1; // Subtract 1 for header row
        container.data = new string*[container.y];
        
        // Read and parse data rows
        string dataLine;
        int rowIndex = 0;
        
        while (getline(file, dataLine) && rowIndex < container.y) {
            container.data[rowIndex] = new string[numCols];
            splitLine(dataLine, container.data[rowIndex], numCols);
            rowIndex++;
        }
        
        file.close();
        container.error = 0;
        return container;
    }

    /*
        Displays the data from struct as a table
    */
    void displayTabulatedData(const DataContainer2d& container) {
        if (container.error == 1 || container.fields == nullptr || container.data == nullptr) {
            cout << "Error: Invalid data container" << endl;
            return;
        }
        
        // First, determine the maximum width needed for each column
        int* columnWidths = new int[container.x];
        for (int j = 0; j < container.x; j++) {
            // Initialize with header width
            columnWidths[j] = container.fields[j].length();
            
            // Check all data rows for this column
            for (int i = 0; i < container.y; i++) {
                if (container.data[i][j].length() > columnWidths[j]) {
                    columnWidths[j] = container.data[i][j].length();
                }
            }
            
            // Add some padding
            columnWidths[j] += 2;
        }
        
        // Print the table header with column names
        cout << "+";
        for (int j = 0; j < container.x; j++) {
            for (int k = 0; k < columnWidths[j] + 2; k++) {
                cout << "-";
            }
            cout << "+";
        }
        cout << endl;
        
        cout << "|";
        for (int j = 0; j < container.x; j++) {
            cout << " " << container.fields[j];
            // Add padding spaces
            for (int k = 0; k < columnWidths[j] - container.fields[j].length(); k++) {
                cout << " ";
            }
            cout << " |";
        }
        cout << endl;
        
        // Print separator line
        cout << "+";
        for (int j = 0; j < container.x; j++) {
            for (int k = 0; k < columnWidths[j] + 2; k++) {
                cout << "-";
            }
            cout << "+";
        }
        cout << endl;
        
        // Print data rows
        for (int i = 0; i < container.y; i++) {
            cout << "|";
            for (int j = 0; j < container.x; j++) {
                cout << " " << container.data[i][j];
                // Add padding spaces
                for (int k = 0; k < columnWidths[j] - container.data[i][j].length(); k++) {
                    cout << " ";
                }
                cout << " |";
            }
            cout << endl;
        }
        
        // Print bottom border
        cout << "+";
        for (int j = 0; j < container.x; j++) {
            for (int k = 0; k < columnWidths[j] + 2; k++) {
                cout << "-";
            }
            cout << "+";
        }
        cout << endl;
        
        // Clean up
        delete[] columnWidths;
    }

    //Sort Algorithum
    //Search Algorithum


    /*
    Helper function to parse date string in format "dd/mm/yyyy" and convert to comparable value
    Returns a long integer representing the date (yyyymmdd) for easy comparison
    */
    long parseDateString(const string& dateStr) {
        
        // Expected format: "dd/mm/yyyy"
        if (dateStr.length() < 10){
            return 0; // Invalid date format
        }  
        
        int day = stoi(dateStr.substr(0, 2));
        int month = stoi(dateStr.substr(3, 2));
        int year = stoi(dateStr.substr(6, 4));
        
        // Convert to format yyyymmdd for proper comparison
        return year * 10000 + month * 100 + day;
    }

    /*
        Helper function for heapSort that maintains the heap property with special handling for date strings
        Rearranges the heap rooted at index i to maintain max heap property
    */
    void heapify(string** data, int n, int i, int column, bool isDateColumn) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        
        // Compare based on whether it's a date column or regular string
        if (isDateColumn) {
            // Date comparison
            if (left < n && parseDateString(data[left][column]) > parseDateString(data[largest][column]))
                largest = left;
            
            if (right < n && parseDateString(data[right][column]) > parseDateString(data[largest][column]))
                largest = right;
        } else {
            // Regular string comparison
            if (left < n && data[left][column] > data[largest][column])
                largest = left;
            
            if (right < n && data[right][column] > data[largest][column])
                largest = right;
        }
        
        // If largest is not root
        if (largest != i) {
            // Swap entire rows to maintain row relationships
            string* temp = data[i];
            data[i] = data[largest];
            data[largest] = temp;
            
            // Recursively heapify the affected sub-tree
            heapify(data, n, largest, column, isDateColumn);
        }
    }

    /*
        Helper function to reverse the array to convert from descending to ascending order
    */
    void reverseArray(string** data, int size) {
        for (int i = 0; i < size / 2; i++) {
            string* temp = data[i];
            data[i] = data[size - 1 - i];
            data[size - 1 - i] = temp;
        }
    }

    /*
        Sorts the 2D data array in ascending order based on values in the specified column
        Has special handling for date columns in format "dd/mm/yyyy"
        Returns the same DataContainer2d structure with data sorted
    */
    DataContainer2d heapSort(DataContainer2d data, int column) {
        // Check for valid input
        if (data.error == 1 || column < 0 || column >= data.x) {
            cerr << "Error: Invalid data container or column index" << endl;
            // Set error flag and return
            data.error = 1;
            return data;
        }
        
        // Detect if this is a date column by checking format of first value
        bool isDateColumn = false;
        if (data.y > 0) {
            string val = data.data[0][column];
            // Simple check for date format "dd/mm/yyyy"
            if (val.length() >= 10 && val[2] == '/' && val[5] == '/') {
                isDateColumn = true;
            }
        }
        
        // Sort the original data directly
        // Build heap (rearrange array)
        for (int i = data.y / 2 - 1; i >= 0; i--)
            heapify(data.data, data.y, i, column, isDateColumn);
        
        // Extract elements from heap one by one
        for (int i = data.y - 1; i > 0; i--) {
            // Move current root to end
            string* temp = data.data[0];
            data.data[0] = data.data[i];
            data.data[i] = temp;
            
            // Call heapify on the reduced heap
            heapify(data.data, i, 0, column, isDateColumn);
        }
        
        // Reverse the array to get ascending order (since heap sort produces descending)
        reverseArray(data.data, data.y);
        
        // Return the original container with sorted data
        return data;
    }

    /*
    Finds unique values in the specified column and counts their occurrences
    Assumes input data is already sorted by the specified column
    Returns a DataContainer2d with two columns: the unique values and their counts
    */

    DataContainer2d repeatingItem(DataContainer2d& data, int column) {
        // Check for valid input
        if (data.error == 1 || column < 0 || column >= data.x) {
            cerr << "Error: Invalid data container or column index" << endl;
            DataContainer2d errorContainer;
            errorContainer.error = 1;
            return errorContainer;
        }
        
        // Count unique values and their occurrences
        // This is more efficient since the data is already sorted
        int uniqueCount = 0;
        
        // First pass: count unique values
        if (data.y > 0) {
            uniqueCount = 1; // At least one unique value if there's data
            
            for (int i = 1; i < data.y; i++) {
                if (data.data[i][column] != data.data[i-1][column]) {
                    uniqueCount++;
                }
            }
        }
        
        // Create result container
        DataContainer2d result;
        result.error = 0;
        result.x = 2; // Two columns: value and count
        result.y = uniqueCount; // Number of unique values
        
        // Set up field names
        result.fields = new string[2];
        result.fields[0] = "Value_" + data.fields[column]; // Use original column name
        result.fields[1] = "Count";
        
        // Allocate memory for data
        result.data = new string*[result.y];
        
        // Fill data with unique values and their counts
        if (data.y > 0) {
            int resultRow = 0;
            string currentValue = data.data[0][column];
            int currentCount = 1;
            
            result.data[0] = new string[2];
            
            // Process all rows
            for (int i = 1; i < data.y; i++) {
                if (data.data[i][column] == currentValue) {
                    // Same value, increment count
                    currentCount++;
                } else {
                    // New value found, store the previous value and its count
                    result.data[resultRow][0] = currentValue;
                    result.data[resultRow][1] = to_string(currentCount);
                    resultRow++;
                    
                    // Start counting the new value
                    currentValue = data.data[i][column];
                    currentCount = 1;
                    
                    // Allocate memory for the new row
                    result.data[resultRow] = new string[2];
                }
            }
            
            // Store the last value and its count
            result.data[resultRow][0] = currentValue;
            result.data[resultRow][1] = to_string(currentCount);
        }
        
        return result;
    }
    
    string** getSubArray(string** arr,int start, int end) {
        int length = end - start;

        string** subArray = new string*[length];

        for (int i = 0; i < length; i++) {
            subArray[i] = arr[start + i];
        }

        return subArray;
    }


    string** mergeSort(string** arr, int length, int fieldIndex) {
        if (length <= 1) {
            return arr;
        }

        int middlePoint = length / 2;
        int leftSize = middlePoint, rightSize = length - middlePoint;
        string** leftArr = getSubArray(arr, 0, middlePoint);
        string** rightArr = getSubArray(arr, middlePoint, length);

        string** sortedLeft = mergeSort(leftArr, leftSize, fieldIndex);
        string** sortedRight = mergeSort(rightArr, rightSize, fieldIndex);

        string** sortedArr = new string*[length];
        int leftIndex = 0, rightIndex = 0;

        for (int i=0; i < length; i++) {
            if (leftIndex >= leftSize) {
                sortedArr[i] = sortedRight[rightIndex];
                rightIndex++;
                continue;
            }

            if (rightIndex >= rightSize) {
                sortedArr[i] = sortedLeft[leftIndex];
                leftIndex++;
                continue;
            }

            if (sortedLeft[leftIndex][fieldIndex] > sortedRight[rightIndex][fieldIndex]) {
                sortedArr[i] = sortedRight[rightIndex];
                rightIndex++;
            } else {
                sortedArr[i] = sortedLeft[leftIndex];
                leftIndex++;
            }
        }

        // Clean Shallow Copies
        delete[] sortedLeft;
        delete[] sortedRight;

        return sortedArr;
    }


    int fibMonaccianSearch(string** arr, int n, const string& item, const int fieldIndex)
    {
        /* Initialize fibonacci numbers */
        int fibMMm2 = 0; // (m-2)'th Fibonacci No.
        int fibMMm1 = 1; // (m-1)'th Fibonacci No.
        int fibM = fibMMm2 + fibMMm1; // m'th Fibonacci
     
        /* fibM is going to store the smallest Fibonacci
           Number greater than or equal to n */
        while (fibM < n) {
            fibMMm2 = fibMMm1;
            fibMMm1 = fibM;
            fibM = fibMMm2 + fibMMm1;
        }
     
        // Marks the eliminated range from front
        int offset = -1;
     
        /* while there are elements to be inspected. Note that
           we compare arr[fibMm2] with x. When fibM becomes 1,
           fibMm2 becomes 0 */
        while (fibM > 1) {
            // Check if fibMm2 is a valid location
            int i = min(offset + fibMMm2, n - 1);
     
            /* If x is greater than the value at index fibMm2,
               cut the subarray array from offset to i */
            if (arr[i][fieldIndex] < item) {
                fibM = fibMMm1;
                fibMMm1 = fibMMm2;
                fibMMm2 = fibM - fibMMm1;
                offset = i;
            }
     
            /* If x is greater than the value at index fibMm2,
               cut the subarray after i+1  */
            else if (arr[i][fieldIndex] > item) {
                fibM = fibMMm2;
                fibMMm1 = fibMMm1 - fibMMm2;
                fibMMm2 = fibM - fibMMm1;
            }
     
            /* element found. return index */
            else
                return i;
        }
     
        /* comparing the last element with x */
        if (fibMMm1 && arr[offset + 1][fieldIndex] == item)
            return offset + 1;
     
        /*element not found. return -1 */
        return -1;
    }



};

#endif // DATACONTAINER_H
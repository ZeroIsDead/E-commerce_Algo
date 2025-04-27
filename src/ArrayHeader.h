#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>

using namespace std;

struct DataContainer2d {
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
        string path = "../data/"+filename+".csv";

        
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

    /*
        Function to count the number of trancation from repreating data set
    */
    int NumberofTransactions (DataContainer2d data){
        int sum = 0;
        for (int i = 0; i < data.y; i++){
            sum = sum + stoi(data.data[i][1]);
        }
        return sum;
    }


    /*
        Helper function to reverse the array 
    */
   void reverseArray(string** data, int size) {
        for (int i = 0; i < size / 2; i++) {
            string* temp = data[i];
            data[i] = data[size - 1 - i];
            data[size - 1 - i] = temp;
        }
    }


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
        Checks if a string contains only numeric characters (integer)
        Returns true if the string represents a number, false otherwise
    */
    bool isNumeric(const string& str) {
        
        try {
            stoi(str);
            return true;
        } catch (const std::invalid_argument&) {
            return false;
        }

    }

    /*
        Checks if a string is a decimal number (contains a dot and can be converted to double)
        Returns true if the string represents a decimal number, false otherwise
    */
   bool isDecimal(const string& str) {
        try {
            // Check if the string contains a decimal point
            if (str.find('.') != string::npos) {
                stod(str); // Try converting to verify it's a valid decimal
                return true;
            }
            return false;

        } 
        catch (const std::invalid_argument&) {
            return false;
        } 
        catch (const std::out_of_range&) {
            return false;
        }
    }

     /*
        Array partition function for QuickSort
    */
    int partition(string** data, int low, int high, int column, bool isDateColumn, bool isNumericColumn, bool isDecimalColumn) {
        // Choose the rightmost element as pivot
        string pivot = data[high][column];
        int i = low - 1;  // Index of smaller element
        
        for (int j = low; j < high; j++) {
            bool shouldSwap = false;
            
            if (isDateColumn) {
                // Date comparison
                if (parseDateString(data[j][column]) <= parseDateString(pivot)) {
                    shouldSwap = true;
                }
            } 
            else if (isNumericColumn) {
                // Numeric comparison
                if (stoi(data[j][column]) <  stoi(pivot)) {
                    shouldSwap = true;
                }
            }
            else if (isDecimalColumn) {

                if (stod(data[j][column]) < stod(pivot)){
                    shouldSwap = true;
                }
            }
            else {
                // Regular string comparison
                if (data[j][column] <= pivot) {
                    shouldSwap = true;
                }
            }
            
            if (shouldSwap) {
                i++;
                // Swap entire rows to maintain row relationships
                string* temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
        
        // Swap pivot element with element at i+1
        string* temp = data[i + 1];
        data[i + 1] = data[high];
        data[high] = temp;
        
        return i + 1;
    }

    /*
        Recursive QuickSort helper function with numeric and date handling
    */
    void quickSortHelper(string** data, int low, int high, int column, bool isDateColumn, bool isNumericColumn, bool isDecimalColumn) {
        if (low < high) {
            // pi is the partitioning index, data[pi] is now at right place
            int pi = partition(data, low, high, column, isDateColumn, isNumericColumn, isDecimalColumn);
            
            // Recursively sort elements before and after partition
            quickSortHelper(data, low, pi - 1, column, isDateColumn, isNumericColumn, isDecimalColumn);
            quickSortHelper(data, pi + 1, high, column, isDateColumn, isNumericColumn, isDecimalColumn);
        }
    }


    /*
         QuickSort that handles numeric columns, date column and normal strings
    */
    void quickSort(DataContainer2d data, int column) {
        
        if (data.error != 0){
            cout << "Data entered is empty of invalid! " << endl;
        }
        else{

            // Detect column types
            bool isDateColumn = false;
            bool isColumnNumeric = false;
            bool isColumnDecimal = false;
            
            // Check for date format
            string val = data.data[0][column];
            if (val.length() >= 10 && val[2] == '/' && val[5] == '/') {
                isDateColumn = true;
            } 
            else{
                // If not a date column, check if it's numeric
                isColumnNumeric = isNumeric(data.data[0][column]);
                isColumnDecimal = isDecimal(data.data[0][column]);
            }
            
            // Sort the data using QuickSort
            quickSortHelper(data.data, 0, data.y - 1, column, isDateColumn, isColumnNumeric, isColumnDecimal);
            
        }
        
    }
   
    /*
        Selection Sort that handles numeric columns (integers and decimals), date column and normal strings
    */
   void selectionSort(DataContainer2d data, int column) {
        if (data.error != 0) {
            cout << "Data entered is empty or invalid!" << endl;
            return;
        }
        
        // Detect column types
        bool isDateColumn = false;
        bool isColumnNumeric = false;
        bool isColumnDecimal = false;
        
        // Check for date format
        string val = data.data[0][column];
        if (val.length() >= 10 && val[2] == '/' && val[5] == '/') {
            isDateColumn = true;
        } 
        else {
            // If not a date column, check if it's numeric or decimal
            isColumnNumeric = isNumeric(data.data[0][column]);
            isColumnDecimal = isDecimal(data.data[0][column]);
        }
        
        // Selection Sort algorithm
        for (int i = 0; i < data.y - 1; i++) {
            // Find the minimum element in unsorted array
            int min_idx = i;
            
            for (int j = i + 1; j < data.y; j++) {
                bool shouldUpdate = false;
                
                if (isDateColumn) {
                    // Date comparison
                    if (parseDateString(data.data[j][column]) < parseDateString(data.data[min_idx][column])) {
                        shouldUpdate = true;
                    }
                } else if (isColumnDecimal) {
                    // Decimal comparison
                    if (stod(data.data[j][column]) < stod(data.data[min_idx][column])) {
                        shouldUpdate = true;
                    }
                } else if (isColumnNumeric) {
                    // Integer comparison
                    if (stoi(data.data[j][column]) < stoi(data.data[min_idx][column]) ) {
                        shouldUpdate = true;
                    }
                } else {
                    // Regular string comparison
                    if (data.data[j][column] < data.data[min_idx][column]) {
                        shouldUpdate = true;
                    }
                }
                
                if (shouldUpdate) {
                    min_idx = j;
                }
            }
            
            // Swap the found minimum element with the first element
            if (min_idx != i) {
                string* temp = data.data[i];
                data.data[i] = data.data[min_idx];
                data.data[min_idx] = temp;
            }
        }
    }
   
    /*
        Performs interpolation search on a sorted string array
        Returns the index of the target string if found, otherwise -1
        
        For string interpolation, we use a simple heuristic based on first character ASCII value
        This is a simplified approach since true string interpolation is more complex
    */
    int interpolationSearch(string** data, int size, int column, const string& target) {
        // Check if array is empty
        if (size <= 0) return -1;
        
        int low = 0;
        int high = size - 1;
        
        // Continue until we find the element or narrow down to an empty subarray
        while (low <= high && target >= data[low][column] && target <= data[high][column]) {
            // No division by zero check
            if (low == high) {
                if (data[low][column] == target) return low;
                return -1;
            }
            
            // Calculate the approximated position using interpolation formula
            // For strings, we use a simple ASCII value based heuristic
            int targetChar = target.empty() ? 0 : target[0];
            int lowChar = data[low][column].empty() ? 0 : data[low][column][0];
            int highChar = data[high][column].empty() ? 0 : data[high][column][0];
            
            // Avoid division by zero
            if (highChar == lowChar) {
                // Sequential search in this case
                for (int i = low; i <= high; i++) {
                    if (data[i][column] == target) return i;
                }
                return -1;
            }
            
            // Calculate probe position
            int pos = low + ((targetChar - lowChar) * (high - low)) / (highChar - lowChar);
            
            // Bound the position to prevent array out of bounds
            if (pos < low) pos = low;
            if (pos > high) pos = high;
            
            // If found, return the position
            if (data[pos][column] == target) return pos;
            
            // If the value at pos is less than target, search in right sub-array
            if (data[pos][column] < target) low = pos + 1;
            
            // If the value at pos is greater than target, search in left sub-array
            else high = pos - 1;
        }
        
        // Element not found
        return -1;
    }


    /*
        Finds unique values in the specified column and counts their occurrences
        Assumes data is already sorted by the specified column
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
        
        // First pass to count unique values
        int uniqueCount = 0;
        if (data.y > 0) {
            uniqueCount = 1; // At least one unique value if there's data
            
            for (int i = 1; i < data.y; i++) {
                if (data.data[i][column] != data.data[i-1][column]) {
                    uniqueCount++;
                }
            }
        }
        
        // Create arrays for unique values and their counts
        string* uniqueValues = new string[uniqueCount];
        int* counts = new int[uniqueCount];
        
        // Second pass to populate the arrays
        if (data.y > 0) {
            int uniqueIndex = 0;
            uniqueValues[uniqueIndex] = data.data[0][column];
            counts[uniqueIndex] = 1;
            
            for (int i = 1; i < data.y; i++) {
                if (data.data[i][column] == uniqueValues[uniqueIndex]) {
                    counts[uniqueIndex]++;
                } else {
                    uniqueIndex++;
                    uniqueValues[uniqueIndex] = data.data[i][column];
                    counts[uniqueIndex] = 1;
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
        result.fields[0] = data.fields[column]; 
        result.fields[1] = "Count";
        
        // Allocate memory for data and populate it
        result.data = new string*[result.y];
        for (int i = 0; i < result.y; i++) {
            result.data[i] = new string[2];
            result.data[i][0] = uniqueValues[i];
            result.data[i][1] = to_string(counts[i]);
        }
        
        // Clean up temporary arrays
        delete[] uniqueValues;
        delete[] counts;
        
        return result;
    }
    
    
    /*
        Performs linear search on a string array
        Returns the index of the target string if found, otherwise -1
        
        This is a simple sequential search that works on both sorted and unsorted data
    */
    int linearSearch(string** data, int size, int column, const string& target) {
        // Check if array is empty
        if (size <= 0) return -1;
        
        // Sequential search through the array
        for (int i = 0; i < size; i++) {
            if (data[i][column] == target) {
                return i;  // Found the target, return its index
            }
        }
        
        // Element not found
        return -1;
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

    //Bubble Sort - Jia Yuan
    void bubbleSort(DataContainer2d data, int column) 
    {
        // Detect column types
        bool isDateColumn = false;
        bool isColumnNumeric = false;
        bool isColumnDecimal = false;

        // Check for date format
        string val = data.data[0][column];
        if (val.length() >= 10 && val[2] == '/' && val[5] == '/') {
            isDateColumn = true;
        } 
        else{
            // If not a date column, check if it's numeric
            isColumnNumeric = isNumeric(data.data[0][column]);
            isColumnDecimal = isDecimal(data.data[0][column]);
            }

        for (int i = 0; i < data.y; i++) 
        {
            bool swap = false;

            for (int j = 0; j < data.y - i - 1; j++) 
            {
                bool shouldSwap = false;
                
                if (isDateColumn) 
                {
                    if (parseDateString(data.data[j][column]) > parseDateString(data.data[j + 1][column])) 
                    {
                        shouldSwap = true;
                    }
                }
                else if(isColumnDecimal)
                {
                    if (stod(data.data[j][column]) > stod(data.data[j + 1][column]))
                    {
                        shouldSwap = true;
                    }
                }
                else if (isColumnNumeric)
                {
                    if (stoi(data.data[j][column]) > stoi(data.data[j + 1][column]))
                    {
                        shouldSwap = true;
                    }
                }
                
                else 
                {
                    if (data.data[j][column] > data.data[j + 1][column]) 
                    {
                        shouldSwap = true;
                    }
                }
            
                if (shouldSwap) 
                {
                    string* temp = data.data[j];
                    data.data[j] = data.data[j+1];
                    data.data[j+1] = temp;
                    swap = true;
                }
            }

            if(!swap) 
            {
                break;
            }
        }
    }

    int binarySearch(string** data, int size, int column, const string& target) 
    {
        // Check if array is empty
        if (size <= 0) return -1;
        
        int low = 0;
        int high = size - 1;
        
        // Detect if column contains dates, numeric or decimal values
        bool isDateColumn = false;
        bool isColumnNumeric = false;
        bool isColumnDecimal = false;
        
        // Check for date format
        string val = data[0][column];
        if (val.length() >= 10 && val[2] == '/' && val[5] == '/') 
        {
            isDateColumn = true;
        } 
        else 
        {
            // First check if it's a decimal
            if (val.find('.') != string::npos) 
            {
                try {
                    stod(val);
                    isColumnDecimal = true;
                } catch (...) {
                    // Not a valid decimal
                }
            }
            // If not decimal, check if it's numeric
            if (!isColumnDecimal) 
            {
                try 
                {
                    stoi(val);
                    isColumnNumeric = true;
                } 
                catch (...) 
                {
                    // Not a valid numeric
                }
            }
        }
        
        // Binary search
        while (low <= high) 
        {
            int mid = low + (high - low) / 2;
            
            // Check if target is present at mid
            bool isEqual = false;
            bool isLess = false;
            
            if (isDateColumn) {
                // Date comparison
                long midDate = parseDateString(data[mid][column]);
                long targetDate = parseDateString(target);
                isEqual = (midDate == targetDate);
                isLess = (midDate > targetDate);
            } 
            else if (isColumnDecimal) {
                // Decimal comparison
                double midVal = stod(data[mid][column]);
                double targetVal = stod(target);
                isEqual = (midVal == targetVal);
                isLess = (midVal > targetVal);
            }
            else if (isColumnNumeric) {
                // Integer comparison
                int midVal = stoi(data[mid][column]);
                int targetVal = stoi(target);
                isEqual = (midVal == targetVal);
                isLess = (midVal > targetVal);
            }
            else {
                // String comparison
                isEqual = (data[mid][column] == target);
                isLess = (data[mid][column] > target);
            }
            
            // If found at mid, return the position
            if (isEqual) 
                return mid;
            
            // If target is smaller, search in left half
            if (isLess)
                high = mid - 1;
            // If target is larger, search in right half
            else
                low = mid + 1;
        }
        
        // Element not found
        return -1;
    }

};

#endif // DATACONTAINER_H

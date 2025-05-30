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
private:
    /*
        Function to Count the number of colums in one string reference.
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
            // Manually check if the string contains a decimal point
            bool hasDecimalPoint = false;
            for (size_t i = 0; i < str.length(); i++) {
                if (str[i] == '.') {
                    hasDecimalPoint = true;
                    break;
                }
            }
            
            if (hasDecimalPoint) {
                stod(str);
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

    //  Array partition function for QuickSort
    int partition(string** data, int low, int high, int column, bool isDateColumn, bool isNumericColumn, bool isDecimalColumn) {
        
        int i = low - 1;  // Index of smaller element
        
        for (int j = low; j < high; j++) {
            bool shouldSwap = false;
            
            if (isDateColumn) {
                // Date comparison
                shouldSwap = parseDateString(data[j][column]) <= parseDateString(data[high][column]);
            }
            else if (isDecimalColumn) {
                // Decimal comparision
                shouldSwap = stod(data[j][column]) < stod(data[high][column]);
            } 
            else if (isNumericColumn) {
                // Numeric comparison
                shouldSwap = stoi(data[j][column]) <  stoi(data[high][column]);
            }
            else {
                // Regular string comparison
                shouldSwap = data[j][column] <= data[high][column];
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

    //  Recursive QuickSort helper function with numeric, decimal and date handling
    void quickSortHelper(string** data, int low, int high, int column, bool isDateColumn, bool isNumericColumn, bool isDecimalColumn) {
        if (low < high) {
            // pi is the partitioning index, data[pi] is now at right place
            int pi = partition(data, low, high, column, isDateColumn, isNumericColumn, isDecimalColumn);
            
            // Recursively sort elements before and after partition
            quickSortHelper(data, low, pi - 1, column, isDateColumn, isNumericColumn, isDecimalColumn);
            quickSortHelper(data, pi + 1, high, column, isDateColumn, isNumericColumn, isDecimalColumn);
        }
    }

    //  Merge function for MergeSort with numeric, decimal and date handling
    void merge(string** arr, int left, int mid, int right, int column, bool isDateColumn, bool isNumericColumn, bool isDecimalColumn) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        
        // Create temp arrays
        string** leftArr = new string*[n1];
        string** rightArr = new string*[n2];
        
        // Copy data to temp arrays
        for (int i = 0; i < n1; i++)
            leftArr[i] = arr[left + i];
            
        for (int j = 0; j < n2; j++)
            rightArr[j] = arr[mid + 1 + j];
        
        // Merge the temp arrays back into arr[left..right]
        int i = 0; // Initial index of first subarray
        int j = 0; // Initial index of second subarray
        int k = left; // Initial index of merged subarray
        
        while (i < n1 && j < n2) {
            bool shouldTakeLeft = false;
            
            if (isDateColumn) {
                // Date comparison
                shouldTakeLeft = parseDateString(leftArr[i][column]) <= parseDateString(rightArr[j][column]);
            }
            else if (isDecimalColumn) {
                shouldTakeLeft = stod(leftArr[i][column]) <= stod(rightArr[j][column]);
            
            } 
            else if (isNumericColumn) {
                // Numeric comparison
                shouldTakeLeft = stoi(leftArr[i][column]) <= stoi(rightArr[j][column]);
            }
            else {
                // Regular string comparison
                shouldTakeLeft = leftArr[i][column] <= rightArr[j][column];
            }
            
            if (shouldTakeLeft) {
                arr[k] = leftArr[i];
                i++;
            } else {
                arr[k] = rightArr[j];
                j++;
            }
            k++;
        }
        
        // Copy the remaining elements of leftArr[]
        while (i < n1) {
            arr[k] = leftArr[i];
            i++;
            k++;
        }
        
        // Copy the remaining elements of rightArr[]
        while (j < n2) {
            arr[k] = rightArr[j];
            j++;
            k++;
        }
        
        // Free memory
        delete[] leftArr;
        delete[] rightArr;
    }

    //  Recursive MergeSort helper function with numeric, decimal and date handling
    void mergeSortHelper(string** arr, int left, int right, int column, bool isDateColumn, bool isNumericColumn, bool isDecimalColumn) {
        if (left < right) {
            // Find the middle point
            int mid = (left + right)  / 2;
            
            // Sort first and second halves
            mergeSortHelper(arr, left, mid, column, isDateColumn, isNumericColumn, isDecimalColumn);
            mergeSortHelper(arr, mid + 1, right, column, isDateColumn, isNumericColumn, isDecimalColumn);
            
            // Merge the sorted halves
            merge(arr, left, mid, right, column, isDateColumn, isNumericColumn, isDecimalColumn);
        }
    }

    int countWordsInColumn(DataContainer2d data, int column) {
        int wordCount = 0;
    
        for (int i = 0; i < data.y; ++i) {
            //string cell = data.data[i][column];
    
            // Replace punctuation with space
            for (int j = 0; j < data.data[i][column].length(); ++j) {
                if (data.data[i][column][j] == ',' || data.data[i][column][j] == '.' || data.data[i][column][j] == '"') {
                    data.data[i][column][j] = ' ';
                }
            }
    
            bool inWord = false;
            for (int j = 0; j < data.data[i][column].length(); ++j) {
                if (data.data[i][column][j] != ' ') {
                    if (!inWord) {
                        wordCount++;
                        inWord = true;
                    }
                } else {
                    inWord = false;
                }
            }
        }
    
        return wordCount;
    }
    
public:
    
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
        Table creation function
        Takes a DataContainer2d as an argument
        prints data in a table with fieldnames
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
        Performs interpolation search on a sorted string array
        Returns the index of the target string if found, otherwise -1
        
        For string interpolation, we use a simple heuristic based on first character ASCII value
        This is a simplified approach since true string interpolation is more complex
    */
    DataContainer2d subDataContainer2d(DataContainer2d data, int first, int last){
            
        if (data.error == 1 || first > last){
            cout << "Error: Invalid data container or column index" << endl;
            DataContainer2d errorContainer;
            errorContainer.error = 1;
            return errorContainer;;
        }

        DataContainer2d result;
        result.error = 0;
        result.y = (last - first) + 1;
        result.x = data.x;
        result.fields = data.fields;

        result.data = new string*[result.y];

        for (int i = 0; i < result.y; i++) {
            result.data[i] = new string[result.x];
            for (int j = 0; j < result.x; j++) {
                result.data[i][j] = data.data[first + i][j];
            }
        }

        return result;

    }    
    
    /*
        Finds unique values in the specified column and counts their occurrences
        Assumes data is already sorted by the specified column
        Returns a DataContainer2d with two columns: the unique values and their counts
    */
    DataContainer2d repeatingItem(DataContainer2d& data, int column) {
        // Check for valid input
        if (data.error == 1 || column < 0 || column >= data.x) {
            cout << "Error: Invalid data container or column index" << endl;
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
        Merge Sort that handles numeric columns, date column and normal strings
    */
    void mergeSort(DataContainer2d data, int column) {
        if (data.error != 0) {
            cout << "Data entered is empty or invalid!" << endl;
            return;
        }
        
        // Detect column types
        bool isDateColumn = data.data[0][column].length() >= 10 && data.data[0][column][2] == '/' && data.data[0][column][5] == '/';
        bool isColumnDecimal = !isDateColumn && isDecimal(data.data[0][column]);;
        bool isColumnNumeric = !isDateColumn && !isColumnDecimal && isNumeric(data.data[0][column]);
        
        // Sort the data using MergeSort
        mergeSortHelper(data.data, 0, data.y - 1, column, isDateColumn, isColumnNumeric, isColumnDecimal);
    }

    /*
        Bubble Sort that handles numeric columns, date column and normal strings
    */
    void bubbleSort(DataContainer2d data, int column) {
        
        if (data.error != 0) {
            cout << "Data entered is empty or invalid!" << endl;
            return;
        }

        // Detecting column types
        bool isDateColumn = data.data[0][column].length() >= 10 && data.data[0][column][2] == '/' && data.data[0][column][5] == '/';
        bool isColumnDecimal = !isDateColumn && isDecimal(data.data[0][column]);;
        bool isColumnNumeric = !isDateColumn && !isColumnDecimal && isNumeric(data.data[0][column]);
        
        bool swap;

        for (int i = 0; i < data.y; i++) {
            swap = false;

            for (int j = 0; j < data.y - i - 1; j++) {
                bool shouldSwap = false;
                
                if (isDateColumn) {
                    shouldSwap = parseDateString(data.data[j][column]) > parseDateString(data.data[j + 1][column]);
                }
                else if(isColumnDecimal){
                    shouldSwap = stod(data.data[j][column]) > stod(data.data[j + 1][column]);
                }
                else if (isColumnNumeric){
                    shouldSwap = stoi(data.data[j][column]) > stoi(data.data[j + 1][column]);
                }
                else {
                    shouldSwap = data.data[j][column] > data.data[j + 1][column];
                }
            
                if (shouldSwap) {
                    string* temp = data.data[j];
                    data.data[j] = data.data[j+1];
                    data.data[j+1] = temp;
                    swap = true;
                }
            }

            if(!swap) {
                break;
            }
        }
    }

    /*
         QuickSort that handles numeric columns, date column and normal strings
    */
    void quickSort(DataContainer2d data, int column) {
        
        if (data.error != 0) {
            cout << "Data entered is empty or invalid!" << endl;
            return;
        }

        /// Detecting column types
        bool isDateColumn = data.data[0][column].length() >= 10 && data.data[0][column][2] == '/' && data.data[0][column][5] == '/';
        bool isColumnDecimal = !isDateColumn && isDecimal(data.data[0][column]);;
        bool isColumnNumeric = !isDateColumn && !isColumnDecimal && isNumeric(data.data[0][column]);
        
        // Sort the data using QuickSort
        quickSortHelper(data.data, 0, data.y - 1, column, isDateColumn, isColumnNumeric, isColumnDecimal);
        
        
    }
   
    /*
        Selection Sort that handles numeric columns (integers and decimals), date column and normal strings
    */
   void selectionSort(DataContainer2d data, int column) {
        
        if (data.error != 0) {
            cout << "Data entered is empty or invalid!" << endl;
            return;
        }
        
        // Detecting column types
        bool isDateColumn = data.data[0][column].length() >= 10 && data.data[0][column][2] == '/' && data.data[0][column][5] == '/';
        bool isColumnDecimal = !isDateColumn && isDecimal(data.data[0][column]);;
        bool isColumnNumeric = !isDateColumn && !isColumnDecimal && isNumeric(data.data[0][column]);
        
        // Selection Sort algorithm
        for (int i = 0; i < data.y - 1; i++) {
            // Find the minimum element in unsorted array
            int min_idx = i;
            
            for (int j = i + 1; j < data.y; j++) {
                bool shouldUpdate = false;
                
                if (isDateColumn) {
                    // Date comparison
                    shouldUpdate = parseDateString(data.data[j][column]) < parseDateString(data.data[min_idx][column]);
                } else if (isColumnDecimal) {
                    // Decimal comparison
                    shouldUpdate = stod(data.data[j][column]) < stod(data.data[min_idx][column]);
                } else if (isColumnNumeric) {
                    // Integer comparison
                    shouldUpdate = stoi(data.data[j][column]) < stoi(data.data[min_idx][column]);
                } else {
                    // Regular string comparison
                    shouldUpdate = data.data[j][column] < data.data[min_idx][column];
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
        Performs interpolation search on a string array
        returns first and last index as reference
        Data needs to be sorted as per column
    */
    void interpolationSearchRange(string** data, int size, int column, const string& target, int& first, int& last) {
        first = -1;
        last = -1;
        
        if (size <= 0) return;
    
        int low = 0;
        int high = size - 1;
    
        // Find FIRST occurrence
        while (low <= high && target >= data[low][column] && target <= data[high][column]) {
            if (low == high) {
                if (data[low][column] == target) {
                    first = low;
                }
                break;
            }
            
            int targetChar;
            int lowChar;
            int highChar;

            if (target.size() == 0) {
                targetChar = 0;
            } else {
                targetChar = target[0];
            }
            
            if (data[low][column].size() == 0) {
                lowChar = 0;
            } else {
                lowChar = data[low][column][0];
            }
        
            if (data[high][column].size() == 0) {
                highChar = 0;
            } else {
                highChar = data[high][column][0];
            }
            
    
            if (highChar == lowChar) {
                for (int i = low; i <= high; i++) {
                    if (data[i][column] == target) {
                        first = i;
                        break;
                    }
                }
                break;
            }
    
            int pos = low + ((targetChar - lowChar) * (high - low)) / (highChar - lowChar);
            if (pos < low) pos = low;
            if (pos > high) pos = high;
    
            if (data[pos][column] == target) {
                first = pos;
                high = pos - 1; 
            }
            else if (data[pos][column] < target) {
                low = pos + 1;
            }
            else {
                high = pos - 1;
            }
        }
    
        // Find LAST occurrence
        low = 0;
        high = size - 1;
    
        while (low <= high && target >= data[low][column] && target <= data[high][column]) 
        {
            if (low == high) {
                if (data[low][column] == target) {
                    last = low;
                }
                break;
            }
            
            int targetChar;
            int lowChar;
            int highChar;
            
            if (target.length() == 0) {
                targetChar = 0;
            } else {
                targetChar = target[0];
            }

            if (data[low][column].length() == 0) {
                lowChar = 0;
            } else {
                lowChar = data[low][column][0];
            }

            if (data[high][column].length() == 0) {
                highChar = 0;
            } else {
                highChar = data[high][column][0];
            }
    
            if (highChar == lowChar) {
                for (int i = high; i >= low; i--) {
                    if (data[i][column] == target) {
                        last = i;
                        break;
                    }
                }
                break;
            }
    
            int pos = low + ((targetChar - lowChar) * (high - low)) / (highChar - lowChar);
            if (pos < low) pos = low;
            if (pos > high) pos = high;
    
            if (data[pos][column] == target) {
                last = pos;
                low = pos + 1;
            }
            else if (data[pos][column] < target) {
                low = pos + 1;
            }
            else {
                high = pos - 1;
            }
        }
    }
      
    /*
        Performs linear search on a string array
        returns first and last index as reference
        Data needs to be sorted as per column
    */
    void linearSearchRange(string** data, int size, int column, const string& target, int& first, int& last) 
    {
        first = -1;
        last = -1;
    
        if (size <= 0) return;
    
        for (int i = 0; i < size; i++) {
            if (data[i][column] == target) {
                if (first == -1) {
                    first = i; // First time we find it
                }
                last = i; // Update last every time we find it
            }
        }
    }

    /*
        Performs binary search on a string array
        returns first and last index as reference
        Data needs to be sorted as per column
    */
    void binarySearchRange(string** data, int size, int column, const string& target, int& first, int& last) 
    {
        int low; 
        int high;

        bool isDateColumn = data[0][column].length() >= 10 && data[0][column][2] == '/' && data[0][column][5] == '/';
        bool isColumnDecimal = !isDateColumn && isDecimal(data[0][column]);;
        bool isColumnNumeric = !isDateColumn && !isColumnDecimal && isNumeric(data[0][column]);

        first = -1;
        last = -1;

        // Find FIRST occurrence
        low = 0; high = size - 1;
        while (low <= high) 
        {
            int mid = (high+low) / 2;
            bool isEqual = false;
            bool isLess = false;

            if (isDateColumn) {
                long midDate = parseDateString(data[mid][column]);
                long targetDate = parseDateString(target);
                isEqual = (midDate == targetDate);
                isLess = (midDate > targetDate);
            } 
            else if (isColumnDecimal) {
                double midVal = stod(data[mid][column]);
                double targetVal = stod(target);
                isEqual = (midVal == targetVal);
                isLess = (midVal > targetVal);
            }
            else if (isColumnNumeric) {
                int midVal = stoi(data[mid][column]);
                int targetVal = stoi(target);
                isEqual = (midVal == targetVal);
                isLess = (midVal > targetVal);
            }
            else {
                isEqual = (data[mid][column] == target);
                isLess = (data[mid][column] > target);
            }

            if (isEqual) {
                first = mid;
                high = mid - 1; // Move left to find earlier occurrence
            }
            else if (isLess) {
                high = mid - 1;
            }
            else {
                low = mid + 1;
            }
        }

        // Find LAST occurrence
        low = 0; high = size - 1;
        while (low <= high) 
        {
            int mid = (high+low) / 2;
            bool isEqual = false;
            bool isLess = false;

            if (isDateColumn) {
                long midDate = parseDateString(data[mid][column]);
                long targetDate = parseDateString(target);
                isEqual = (midDate == targetDate);
                isLess = (midDate > targetDate);
            } 
            else if (isColumnDecimal) {
                double midVal = stod(data[mid][column]);
                double targetVal = stod(target);
                isEqual = (midVal == targetVal);
                isLess = (midVal > targetVal);
            }
            else if (isColumnNumeric) {
                int midVal = stoi(data[mid][column]);
                int targetVal = stoi(target);
                isEqual = (midVal == targetVal);
                isLess = (midVal > targetVal);
            }
            else {
                isEqual = (data[mid][column] == target);
                isLess = (data[mid][column] > target);
            }

            if (isEqual) {
                last = mid;
                low = mid + 1; // Move right to find later occurrence
            }
            else if (isLess) {
                high = mid - 1;
            }
            else {
                low = mid + 1;
            }
        }
    }
    
    /*
        Performs fibonacci search on a string array
        returns first and last index as reference
        Data needs to be sorted as per column
    */
    void fibonacciSearchRange(string** data, int size, int column, const string& target, int& first, int& last) {
        first = -1;
        last = -1;
        
        if (size <= 0) return;
        
        // Detect column types
        bool isDateColumn = data[0][column].length() >= 10 && data[0][column][2] == '/' && data[0][column][5] == '/';
        bool isColumnDecimal = !isDateColumn && isDecimal(data[0][column]);;
        bool isColumnNumeric = !isDateColumn && !isColumnDecimal && isNumeric(data[0][column]);
        
        // Find FIRST occurrence using Fibonacci search
        int fibMMm2 = 0;  // (m-2)'th Fibonacci Number
        int fibMMm1 = 1;  // (m-1)'th Fibonacci Number
        int fibM = fibMMm2 + fibMMm1;  // m'th Fibonacci Number
        
        // Compute smallest Fibonacci number greater than or equal to size
        while (fibM < size) {
            fibMMm2 = fibMMm1;
            fibMMm1 = fibM;
            fibM = fibMMm2 + fibMMm1;
        }
        
        // Mark the eliminated range from front
        int offset = -1;
        
        // Do while loop for Fibonacci search
        while (fibM > 1) {
            // Check if fibMm2 is a valid location
            int i;
            
            if (offset + fibMMm2 < size)
                i = offset + fibMMm2;
            else
                i = size - 1;
            
            bool isEqual = false;
            bool isLess = false;
            
            if (isDateColumn) {
                long itemDate = parseDateString(target);
                long currDate = parseDateString(data[i][column]);
                isEqual = (itemDate == currDate);
                isLess = (itemDate > currDate);
            } 
            else if (isColumnDecimal) {
                double itemVal = stod(target);
                double currVal = stod(data[i][column]);
                isEqual = (itemVal == currVal);
                isLess = (itemVal > currVal);
            }
            else if (isColumnNumeric) {
                int itemVal = stoi(target);
                int currVal = stoi(data[i][column]);
                isEqual = (itemVal == currVal);
                isLess = (itemVal > currVal);
            }
            else {
                isEqual = (target == data[i][column]);
                isLess = (target > data[i][column]);
            }
            
            if (isEqual) {
                // We found the element, now find the first occurrence
                first = i;
                while (first > 0) {
                    bool prevEqual = false;
                    
                    if (isDateColumn) {
                        prevEqual = (parseDateString(data[first-1][column]) == parseDateString(target));
                    } 
                    else if (isColumnDecimal) {
                        prevEqual = (stod(data[first-1][column]) == stod(target));
                    }
                    else if (isColumnNumeric) {
                        prevEqual = (stoi(data[first-1][column]) == stoi(target));
                    }
                    else {
                        prevEqual = (data[first-1][column] == target);
                    }
                    
                    if (prevEqual) {
                        first--;
                    } 
                    else {
                        break;
                    }
                }
                break;
            }
            else if (isLess) {
                // Cut the subarray from offset to i
                fibM = fibMMm1;
                fibMMm1 = fibMMm2;
                fibMMm2 = fibM - fibMMm1;
                offset = i;
            }
            else {
                // Cut the subarray after i+1
                fibM = fibMMm2;
                fibMMm1 = fibMMm1 - fibMMm2;
                fibMMm2 = fibM - fibMMm1;
            }
        }
        
        // Compare the last element if we haven't found first yet
        if (first == -1 && fibMMm1 && offset + 1 < size) {
            bool isEqual = false;
            
            if (isDateColumn) {
                isEqual = (parseDateString(data[offset+1][column]) == parseDateString(target));
            } 
            else if (isColumnDecimal) {
                isEqual = (stod(data[offset+1][column]) == stod(target));
            }
            else if (isColumnNumeric) {
                isEqual = (stoi(data[offset+1][column]) == stoi(target));
            }
            else {
                isEqual = (data[offset+1][column] == target);
            }
            
            if (isEqual) {
                first = offset + 1;
            }
        }
        
        // If we found first, now find last
        if (first != -1) {
            last = first;
            while (last < size - 1) {
                bool nextEqual = false;
                
                if (isDateColumn) {
                    nextEqual = (parseDateString(data[last+1][column]) == parseDateString(target));
                } 
                else if (isColumnDecimal) {
                    nextEqual = (stod(data[last+1][column]) == stod(target));
                }
                else if (isColumnNumeric) {
                    nextEqual = (stoi(data[last+1][column]) == stoi(target));
                }
                else {
                    nextEqual = (data[last+1][column] == target);
                }
                
                if (nextEqual) {
                    last++;
                } else {
                    break;
                }
            }
        }
    }

    /*
        DataContainer for all words in a coloum
    */
    DataContainer2d words(DataContainer2d data, int column) {
        if (data.error == 1 || column < 0 || column >= data.x) {
            DataContainer2d err;
            err.error = 1;
            return err;
        }
    
        // Number of words
        int wordCount = countWordsInColumn(data,column);

        // result container 
        DataContainer2d result;
        result.error = 0;
        result.x = 1;
        result.y = wordCount;
        result.fields = new string[1];
        result.fields[0] = "Word";
        result.data = new string*[wordCount];
    
        // Extract words
        int wordIndex = 0;
    
        for (int i = 0; i < data.y; ++i) {
            
            // Build words manually
            string currentWord = "";
            for (int j = 0; j <= data.data[i][column].length(); ++j) {
                
                // Add trailing space to flush last word
                char c;
                if (j < data.data[i][column].length()){
                    c = data.data[i][column][j];
                }
                else{
                    c = ' ';
                }
                
                if (c != ' ') {
                    currentWord += c;
                } else {
                    if (!currentWord.empty()) {
                        result.data[wordIndex] = new string[1];
                        result.data[wordIndex][0] = currentWord;
                        wordIndex++;
                        currentWord = "";
                    }
                }
            }
        }
    
        return result;
    
    }
    
    
};

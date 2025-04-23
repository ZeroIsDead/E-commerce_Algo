#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

struct DataContainer2d {
    // All members are public
    int error; // 1 - error | 0 - fine
    string* fields; // an array containing the fields (headers)
    string** data; // a 2D array containing each line of data
    int y; // y - number of rows 
    int x; // x - number of columns
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


};

#endif // DATACONTAINER_H
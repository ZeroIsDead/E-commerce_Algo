#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

class Node {
    public:
        string* data;
        Node* next;

        Node() {
            data = nullptr;
            next = nullptr;
        }

        ~Node() {
            delete[] data;
            delete next;
        }

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
};

class LinkedList {
    public:
        int error;
        string* fieldHead;
        Node* head;
        int x;
        int y;


    public:
        LinkedList() {
            error = 0;
            fieldHead = nullptr;
            head = nullptr;
            x = 0;
            y = 0;
        }

        ~LinkedList() {
            delete[] fieldHead;
            delete head;
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
        string* createRow(const string& line, const int numCols) {
            int col = 0;
            string current = "";

            string* data = new string[numCols];
            
            for (size_t i = 0; i <= line.length(); i++) {
                if (i == line.length() || line[i] == ',') {
                    data[col] = current;
                    current = "";

                    col++;

                    if (col >= numCols) {
                        break;
                    }
                } else {
                    current += line[i];
                }

            }

            return data;
        }
        
        /*
            Datadata2d Stuct that take the CSV filename as argument.
            FIELD names are stored in FIELDS, 1D array
            DATA is sotries in data, 2D array
            INT X stores the number of columns
            INT Y stores the number of ROWS
            INT ERROR stores 1 or 0 depending on if the struct has been created properly or not
        */
        LinkedList getdata(const string& filename) {
            LinkedList data;
            string path = "../data/"+filename;
    
            
            // Count rows and prepare to read the file
            int totalRows = countRows(path);
            if (totalRows <= 1) { // Need at least header row and one data row
                data.error = 1;
                return data;
            }
            
            ifstream file(path);
            if (!file.is_open()) {
                cerr << "Error: Could not open file " << path << endl;
                data.error = 1;
                return data;
            }
            
            // Read the header row to determine number of columns
            string headerLine;
            if (!getline(file, headerLine)) {
                data.error = 1;
                file.close();
                return data;
            }

            
            // Count columns and allocate memory for fields
            int numCols = countColumns(headerLine);
            data.x = numCols;
            data.fieldHead = createRow(headerLine, numCols);

            // Set dimensions and allocate memory for data
            data.y = totalRows - 1; // Subtract 1 for header row
            data.head = buildLinkedList(file, numCols);

            file.close();
            
            // No Head -> Error
            if (!data.head) {
                data.error = 1;
            }

            data.error = 0;
            return data;
        }

        /*
            Displays the data in from struct as a table
        */
        void displayTabulatedData(const LinkedList& data) {
            if (data.error == 1 || !data.fieldHead || !data.head) {
                cout << "Error: Invalid data data" << endl;
                return;
            }

            Node* currentNode = data.head;
            int x = 0;
            
            // First, determine the maximum width needed for each column
            int* columnWidths = new int[data.x];

            const int extraPadding = 2;

            // Set Default 
            for (int x = 0; x < data.x; x++) {
                columnWidths[x] = (*(data.fieldHead + x)).length() + extraPadding;
            }

            // Get Max Column Width - Row by Row
            while (currentNode) {
                // Iterate through Row Data
                for (int x = 0; x < data.x; x++) {
                    int currentNodeLength = (*(currentNode->data + x)).length() + extraPadding;

                    if (currentNodeLength > columnWidths[x]) {
                        columnWidths[x] = currentNodeLength;
                    }
                }

                currentNode = currentNode->next;
            }
            
            // Print the table header with column names
            cout << "+";
            for (int j = 0; j < data.x; j++) {
                for (int k = 0; k < columnWidths[j] + 2; k++) {
                    cout << "-";
                }
                cout << "+";
            }
            cout << endl;

            // Print Header Row
            cout << "|";
            for (int x = 0; x < data.x; x++) {
                cout << " " << *(data.fieldHead + x);
                
                int nPadding = columnWidths[x] - (*(data.fieldHead + x)).length();
                // Add padding spaces
                for (int k = 0; k < nPadding; k++) {
                    cout << " ";
                }
                cout << " |";
            }
            cout << endl;
            
            // Print separator line
            cout << "+";
            for (int j = 0; j < data.x; j++) {
                for (int k = 0; k < columnWidths[j] + 2; k++) {
                    cout << "-";
                }
                cout << "+";
            }
            cout << endl;

            currentNode = data.head;

            // Print data rows
            while (currentNode) {
                // Initialize with header width
                columnWidths[x] = (*(data.fieldHead + x)).length();

                cout << "|";
                // Iterate through Row Data
                for (int x = 0; x < data.x; x++) {
                    string currentData = *(currentNode->data + x);

                    cout << " " << currentData;

                    int nPadding = columnWidths[x] - currentData.length();

                    // Add padding spaces
                    for (int k = 0; k < nPadding; k++) {
                        cout << " ";
                    }
                    cout << " |";
                }
                cout << endl;

                currentNode = currentNode->next;
            }            
            
            // Print bottom border
            cout << "+";
            for (int j = 0; j < data.x; j++) {
                for (int k = 0; k < columnWidths[j] + 2; k++) {
                    cout << "-";
                }
                cout << "+";
            }
            cout << endl;
            
            // Clean up
            delete[] columnWidths;
        }


    private:

        /*
            Get the head Node
        */
        Node* buildLinkedList(ifstream& file, const int numCols) {
            string line;

            // File has no more lines then return null
            if (!getline(file, line)) {
                return nullptr;
            }

            // Get Current and Next Rows
            Node* currentNode = new Node();
            currentNode->data = createRow(line, numCols);
            currentNode->next = buildLinkedList(file, numCols);

            return currentNode;
        }
};
    
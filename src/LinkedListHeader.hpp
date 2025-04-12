#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

class Node {
    public:
        string data;
        Node* nextColumn;
        Node* nextRow;

        Node() {
            data = "";
            nextColumn = nullptr;
            nextRow = nullptr;
        }
};

class LinkedList {
    public:
        int error;
        Node* fieldHead;
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
            while (fieldHead) {
                Node* next = fieldHead->nextColumn;
                delete fieldHead;
                fieldHead = next;
            }

            Node* downTraversingNode = head;

            // Delete Row by Row
            while (downTraversingNode) {
                // Get Current Row
                Node* rightTraversingNode = downTraversingNode;
                
                // Get Next Row
                downTraversingNode = downTraversingNode->nextRow;

                // Delete Current Row
                while (rightTraversingNode) {
                    Node* nextNode = rightTraversingNode->nextColumn;
                    delete rightTraversingNode;
                    rightTraversingNode = nextNode;
                }
            }
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
        Node* createRow(const string& line) {
            int col = 0;
            string current = "";

            Node* head = new Node();
            Node* currentNode = head;
            
            for (size_t i = 0; i <= line.length(); i++) {
                if (i == line.length() || line[i] == ',') {
                    currentNode->data = current;
                    current = "";

                    if (i != line.length()) {
                        currentNode->nextColumn = new Node();
                        currentNode = currentNode->nextColumn;
                    }

                } else {
                    current += line[i];
                }
            }

            return head;
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
            data.fieldHead = createRow(headerLine);

            // Set dimensions and allocate memory for data
            data.y = totalRows - 1; // Subtract 1 for header row
            data.head = buildLinkedList(file);

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

            Node* rightTraversingNode = data.fieldHead;
            Node* downTraversingHead = data.head;
            Node* downTraversingNode;
            int x = 0;
            
            // First, determine the maximum width needed for each column
            int* columnWidths = new int[data.x];
            while (rightTraversingNode) {

                if (downTraversingHead) {
                    downTraversingNode = downTraversingHead;
                    downTraversingHead = downTraversingHead->nextColumn;
                }

                // Initialize with header width
                columnWidths[x] = rightTraversingNode->data.length();


                // Check all data rows for this column
                while (downTraversingNode) {
                    int currentNodeLength = downTraversingNode->data.length();

                    if (currentNodeLength > columnWidths[x]) {
                        columnWidths[x] = currentNodeLength;
                    }

                    downTraversingNode = downTraversingNode->nextRow;
                }

                // Add some padding
                columnWidths[x] += 2;
                x += 1;
                rightTraversingNode = rightTraversingNode->nextColumn;
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
            

            rightTraversingNode = data.fieldHead;
            x = 0;

            cout << "|";
            while (rightTraversingNode) {
                cout << " " << rightTraversingNode->data;
                // Add padding spaces
                for (int k = 0; k < columnWidths[x] - rightTraversingNode->data.length(); k++) {
                    cout << " ";
                }
                cout << " |";
                rightTraversingNode = rightTraversingNode->nextColumn;
                x++;
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

            downTraversingNode = data.head;

            // Print data rows
            while (downTraversingNode) {
                cout << "|";
    
                rightTraversingNode = downTraversingNode;
                int x = 0;

                while (rightTraversingNode) {
                    cout << " " << rightTraversingNode->data;
                    // Add padding spaces
                    for (int k = 0; k < columnWidths[x] - rightTraversingNode->data.length(); k++) {
                        cout << " ";
                    }
                    cout << " |";
                    rightTraversingNode = rightTraversingNode->nextColumn;
                    x++;
                }
                cout << endl;
                downTraversingNode = downTraversingNode->nextRow;
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
        Node* buildLinkedList(ifstream& file) {
            string line;

            // File has no more lines then return null
            if (!getline(file, line)) {
                return nullptr;
            }

            // Get Current and Next Rows
            Node* currentHead = createRow(line);
            Node* nextHead = buildLinkedList(file);

            // Next Head doesnt exist then return current head
            if (!nextHead) {
                return currentHead;
            }

            // Create Pointers for traversal
            Node* currentNode1 = currentHead;
            Node* currentNode2 = nextHead;

            while (currentNode1 && currentNode2) {
                // Points 1 Node down
                currentNode1->nextRow = currentNode2;
                
                // Traverse
                currentNode1 = currentNode1->nextColumn;
                currentNode2 = currentNode2->nextColumn;
            }

            return currentHead;
        }
};
    
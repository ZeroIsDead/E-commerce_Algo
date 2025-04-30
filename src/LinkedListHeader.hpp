#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>

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
            // FIXED: Remove recursive deletion to prevent stack overflow
            // The LinkedList destructor will handle this iteratively
        }

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
};

class LinkedList {
    public:
        int error;
        string* fieldHead;
        Node* head;
        Node* tail;
        int x;
        int y;


    public:
        LinkedList() {
            error = 0;
            fieldHead = nullptr;
            head = nullptr;
            tail = nullptr;
            x = 0;
            y = 0;
        }

        ~LinkedList() {
            delete[] fieldHead;
            
            //  Iterative deletion of linked list to prevent stack overflow
            Node* current = head;
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                temp->next = nullptr;
                delete temp;
            }
        }
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
            auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    
            cout << "Time taken: " << duration.count() << " microseconds" << endl;
        }
};


class Functions {
    public:
    
        Node* getNodeTail(Node* head) {
            if (!head) return nullptr; // FIXED: Added null check
            
            Node* current = head;
            while (current && current->next) {
                current = current->next;
            }

            return current;
        }
       
        LinkedList getdata(const string& filename) {
            LinkedList data;
            string path = "../data/"+filename+".csv";
    
            
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
                return data;
            }

            data.tail = getNodeTail(data.head);

            data.error = 0;
            return data;
        }

        //  Displays the data in from struct as a table
        void displayTabulatedData(const LinkedList& data) {
            if (data.error == 1 || !data.fieldHead || !data.head) {
                cout << "Error: Invalid data data" << endl;
                return;
            }

            Node* currentNode = data.head;
            
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

        Node* getNodeAtIndex(Node* head, const int index) {
            if (!head) return nullptr; // FIXED: Added null check
            
            int i = 0;
            Node* current = head;
            while (current && i < index) {
                current = current->next;
                i++;
            }
            return current;
        }

        /*  
            Fibmonaccian Search Range, 
            returns the reference of fist and last index from a sorted linked list 
        */
        void fibMonaccianSearchRange(Node* head, int size, const string& item, const int fieldIndex, int& first, int& last) {
            // Initialize first and last to cover the entire range
            first = 0;
            last = size - 1;
            
            // If list is empty, set invalid indices and return
            if (!head || size <= 0) {
                first = -1;
                last = -1;
                return;
            }
            
            // Detect column type from the first node
            bool isDateColumn = (head->data[fieldIndex].length() >= 10 && head->data[fieldIndex][2] == '/' && head->data[fieldIndex][5] == '/');
            bool isDecimalColumn = !isDateColumn && isDecimal(head->data[fieldIndex]);
            bool isNumericColumn = !isDateColumn && !isDecimalColumn && isNumeric(head->data[fieldIndex]);
            
            // Find the range of nodes that match the item
            
            // First, find the leftmost occurrence
            int left = 0;
            int right = size - 1;
            first = -1;
            
            while (left <= right) {
                int mid = left + (right - left) / 2;
                
                // Get node at position mid
                Node* midNode = getNodeAtIndex(head, mid);
                
                if (!midNode) break;  // Safety check
                
                bool isGreaterOrEqual = false;
                
                // Compare based on data types
                if (isDateColumn) {
                    isGreaterOrEqual = parseDateString(midNode->data[fieldIndex]) >= parseDateString(item);
                } else if (isDecimalColumn) {
                    isGreaterOrEqual = stod(midNode->data[fieldIndex]) >= stod(item);
                } else if (isNumericColumn) {
                    isGreaterOrEqual = stoi(midNode->data[fieldIndex]) >= stoi(item);
                } else {
                    isGreaterOrEqual = midNode->data[fieldIndex] >= item;
                }
                
                if (isGreaterOrEqual) {
                    bool isEqual = false;
                    
                    // Check for equality
                    if (isDateColumn) {
                        isEqual = parseDateString(midNode->data[fieldIndex]) == parseDateString(item);
                    } else if (isDecimalColumn) {
                        isEqual = stod(midNode->data[fieldIndex]) == stod(item);
                    } else if (isNumericColumn) {
                        isEqual = stoi(midNode->data[fieldIndex]) == stoi(item);
                    } else {
                        isEqual = midNode->data[fieldIndex] == item;
                    }
                    
                    if (isEqual) {
                        first = mid;
                    }
                    
                    right = mid - 1;
                } 
                else {
                    left = mid + 1;
                }
            }
            
            // If first occurrence wasn't found, element doesn't exist
            if (first == -1) {
                last = -1;
                return;
            }
            
            // Now find the rightmost occurrence
            left = 0;
            right = size - 1;
            last = -1;
            
            while (left <= right) {
                int mid = left + (right - left) / 2;
                
                // Get node at position mid
                Node* midNode = getNodeAtIndex(head, mid);
                
                if (!midNode) break;  // Safety check
                
                bool isLessOrEqual = false;
                
                // Compare based on data types
                if (isDateColumn) {
                    isLessOrEqual = parseDateString(midNode->data[fieldIndex]) <= parseDateString(item);
                } else if (isDecimalColumn) {
                    isLessOrEqual = stod(midNode->data[fieldIndex]) <= stod(item);
                } else if (isNumericColumn) {
                    isLessOrEqual = stoi(midNode->data[fieldIndex]) <= stoi(item);
                } else {
                    isLessOrEqual = midNode->data[fieldIndex] <= item;
                }
                
                if (isLessOrEqual) {
                    bool isEqual = false;
                    
                    // Check for equality
                    if (isDateColumn) {
                        isEqual = parseDateString(midNode->data[fieldIndex]) == parseDateString(item);
                    } else if (isDecimalColumn) {
                        isEqual = stod(midNode->data[fieldIndex]) == stod(item);
                    } else if (isNumericColumn) {
                        isEqual = stoi(midNode->data[fieldIndex]) == stoi(item);
                    } else {
                        isEqual = midNode->data[fieldIndex] == item;
                    }
                    
                    if (isEqual) {
                        last = mid;
                    }
                    
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        
        /*  Binary Search Range, 
            returns the reference of fist and last index from a sorted linked list 
        */
        void binarySearchRange(Node* head, int size, const string& item, const int fieldIndex, int& first, int& last) {
            // Initialize first and last to cover the entire range
            first = -1;
            last = -1;
        
            // If list is empty, return
            if (!head || size <= 0) return;
        
            // Detect column type from the first node
            bool isDateColumn = (head->data[fieldIndex].length() >= 10 && head->data[fieldIndex][2] == '/' && head->data[fieldIndex][5] == '/');
            bool isDecimalColumn = !isDateColumn && isDecimal(head->data[fieldIndex]);
            bool isNumericColumn = !isDateColumn && !isDecimalColumn && isNumeric(head->data[fieldIndex]);
        
            // Find the leftmost occurrence (first)
            int left = 0, right = size - 1;
            while (left <= right) {
                int mid = left + (right - left) / 2;
        
                Node* midNode = getNodeAtIndex(head,mid);
        
                if (!midNode) break;
        
                bool isGreaterOrEqual = false;
        
                if (isDateColumn)
                    isGreaterOrEqual = parseDateString(midNode->data[fieldIndex]) >= parseDateString(item);
                else if (isDecimalColumn)
                    isGreaterOrEqual = stod(midNode->data[fieldIndex]) >= stod(item);
                else if (isNumericColumn)
                    isGreaterOrEqual = stoi(midNode->data[fieldIndex]) >= stoi(item);
                else
                    isGreaterOrEqual = midNode->data[fieldIndex] >= item;
        
                if (isGreaterOrEqual) {
                    bool isEqual = false;
                    if (isDateColumn)
                        isEqual = parseDateString(midNode->data[fieldIndex]) == parseDateString(item);
                    else if (isDecimalColumn)
                        isEqual = stod(midNode->data[fieldIndex]) == stod(item);
                    else if (isNumericColumn)
                        isEqual = stoi(midNode->data[fieldIndex]) == stoi(item);
                    else
                        isEqual = midNode->data[fieldIndex] == item;
        
                    if (isEqual)
                        first = mid;
        
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
        
            // If first wasn't found, item doesn't exist
            if (first == -1) return;
        
            // Find the rightmost occurrence (last)
            left = 0, right = size - 1;
            while (left <= right) {
                int mid = left + (right - left) / 2;
        
                Node* midNode = getNodeAtIndex(head,mid);
        
                if (!midNode) break;
        
                bool isLessOrEqual = false;
        
                if (isDateColumn)
                    isLessOrEqual = parseDateString(midNode->data[fieldIndex]) <= parseDateString(item);
                else if (isDecimalColumn)
                    isLessOrEqual = stod(midNode->data[fieldIndex]) <= stod(item);
                else if (isNumericColumn)
                    isLessOrEqual = stoi(midNode->data[fieldIndex]) <= stoi(item);
                else
                    isLessOrEqual = midNode->data[fieldIndex] <= item;
        
                if (isLessOrEqual) {
                    bool isEqual = false;
                    if (isDateColumn)
                        isEqual = parseDateString(midNode->data[fieldIndex]) == parseDateString(item);
                    else if (isDecimalColumn)
                        isEqual = stod(midNode->data[fieldIndex]) == stod(item);
                    else if (isNumericColumn)
                        isEqual = stoi(midNode->data[fieldIndex]) == stoi(item);
                    else
                        isEqual = midNode->data[fieldIndex] == item;
                    if (isEqual)
                        last = mid;
        
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        
        /*  Interpolation Search Range, 
            returns the reference of fist and last index from a sorted linked list 
        */
        void interpolationSearchRange(Node* head, int size, const string& item, const int fieldIndex, int& first, int& last) {
            // Initialize first and last to cover the entire range
            first = 0;
            last = size - 1;
            
            // If list is empty, set invalid indices and return
            if (!head || size <= 0) {
                first = -1;
                last = -1;
                return;
            }
            
            // Detect column type from the first node
            bool isDateColumn = (head->data[fieldIndex].length() >= 10 && head->data[fieldIndex][2] == '/' && head->data[fieldIndex][5] == '/');
            bool isDecimalColumn = !isDateColumn && isDecimal(head->data[fieldIndex]);
            bool isNumericColumn = !isDateColumn && !isDecimalColumn && isNumeric(head->data[fieldIndex]);
            
            // First, find the leftmost occurrence using interpolation search
            int left = 0;
            int right = size - 1;
            first = -1;
            
            while (left <= right) {
                // Get node at left position
                Node* leftNode = getNodeAtIndex(head, left);
                Node* rightNode = getNodeAtIndex(head, right);
                
                if (!leftNode || !rightNode) break;  // Safety check
                
                // Calculate interpolation position
                int pos = left;
                if (left != right) {
                    double valueLeft, valueRight, valueTarget;
                    
                    // Extract values based on column type
                    if (isDateColumn) {
                        valueLeft = parseDateString(leftNode->data[fieldIndex]);
                        valueRight = parseDateString(rightNode->data[fieldIndex]);
                        valueTarget = parseDateString(item);
                    } else if (isDecimalColumn) {
                        valueLeft = stod(leftNode->data[fieldIndex]);
                        valueRight = stod(rightNode->data[fieldIndex]);
                        valueTarget = stod(item);
                    } else if (isNumericColumn) {
                        valueLeft = (double)stoi(leftNode->data[fieldIndex]);
                        valueRight = (double)stoi(rightNode->data[fieldIndex]);
                        valueTarget = (double)stoi(item);
                    } else {
                        // For string comparisons, use ASCII values as an approximation
                        valueLeft = (double)leftNode->data[fieldIndex][0];
                        valueRight = (double)rightNode->data[fieldIndex][0];
                        valueTarget = (double)item[0];
                    }
                    
                    // Prevent division by zero and handle equal values
                    if (valueRight > valueLeft) {
                        // Calculate the estimated position
                        double fraction = (valueTarget - valueLeft) / (valueRight - valueLeft);
                        pos = left + (int)(fraction * (right - left));
                        
                        // Ensure pos is within bounds
                        pos = max(left, min(right, pos));
                    }
                }
                
                // Get node at calculated position
                Node* posNode = getNodeAtIndex(head, pos);
                
                if (!posNode){
                    break;  // Safety check
                } 
                
                bool isGreaterOrEqual = false;
                bool isEqual = false;
                
                // Compare based on data types
                if (isDateColumn) {
                    int posDate = parseDateString(posNode->data[fieldIndex]);
                    int targetDate = parseDateString(item);
                    isGreaterOrEqual = posDate >= targetDate;
                    isEqual = posDate == targetDate;
                } else if (isDecimalColumn) {
                    double posValue = stod(posNode->data[fieldIndex]);
                    double targetValue = stod(item);
                    isGreaterOrEqual = posValue >= targetValue;
                    isEqual = posValue == targetValue;
                } else if (isNumericColumn) {
                    int posValue = stoi(posNode->data[fieldIndex]);
                    int targetValue = stoi(item);
                    isGreaterOrEqual = posValue >= targetValue;
                    isEqual = posValue == targetValue;
                } else {
                    isGreaterOrEqual = posNode->data[fieldIndex] >= item;
                    isEqual = posNode->data[fieldIndex] == item;
                }
                
                if (isEqual) {
                    first = pos;
                    right = pos - 1;  // Continue searching to the left for first occurrence
                } else if (isGreaterOrEqual) {
                    right = pos - 1;
                } else {
                    left = pos + 1;
                }
            }
            
            // If first occurrence wasn't found, element doesn't exist
            if (first == -1) {
                last = -1;
                return;
            }
            
            // Now find the rightmost occurrence using interpolation search
            left = first;  // We can start from the first occurrence
            right = size - 1;
            last = -1;
            
            while (left <= right) {
                // Get node at left position
                Node* leftNode = getNodeAtIndex(head,left);
                Node* rightNode = getNodeAtIndex(head,right);
                
                if (!leftNode || !rightNode) break;  // Safety check
                
                // Calculate interpolation position
                int pos = left;
                if (left != right) {
                    double valueLeft, valueRight, valueTarget;
                    
                    // Extract values based on column type
                    if (isDateColumn) {
                        valueLeft = parseDateString(leftNode->data[fieldIndex]);
                        valueRight = parseDateString(rightNode->data[fieldIndex]);
                        valueTarget = parseDateString(item);
                    } else if (isDecimalColumn) {
                        valueLeft = stod(leftNode->data[fieldIndex]);
                        valueRight = stod(rightNode->data[fieldIndex]);
                        valueTarget = stod(item);
                    } else if (isNumericColumn) {
                        valueLeft = (double)stoi(leftNode->data[fieldIndex]);
                        valueRight = (double)stoi(rightNode->data[fieldIndex]);
                        valueTarget = (double)stoi(item);
                    } else {
                        // For string comparisons, use ASCII values as an approximation
                        valueLeft = (double)leftNode->data[fieldIndex][0];
                        valueRight = (double)rightNode->data[fieldIndex][0];
                        valueTarget = (double)item[0];
                    }
                    
                    // Prevent division by zero and handle equal values
                    if (valueRight > valueLeft) {
                        // Calculate the estimated position
                        double fraction = (valueTarget - valueLeft) / (valueRight - valueLeft);
                        pos = left + (int)(fraction * (right - left));
                        
                        // Ensure pos is within bounds
                        if (pos < left) {
                            pos = left;
                        }   
                        if (pos > right) {
                            pos = right;
                        }
                    }
                }
                
                // Get node at calculated position
                Node* posNode = getNodeAtIndex(head, pos);
                
                
                if (!posNode){
                    break;  // Safety check
                } 
                
                bool isLessOrEqual = false;
                bool isEqual = false;
                
                // Compare based on data types
                if (isDateColumn) {
                    int posDate = parseDateString(posNode->data[fieldIndex]);
                    int targetDate = parseDateString(item);
                    isLessOrEqual = posDate <= targetDate;
                    isEqual = posDate == targetDate;
                } else if (isDecimalColumn) {
                    double posValue = stod(posNode->data[fieldIndex]);
                    double targetValue = stod(item);
                    isLessOrEqual = posValue <= targetValue;
                    isEqual = posValue == targetValue;
                } else if (isNumericColumn) {
                    int posValue = stoi(posNode->data[fieldIndex]);
                    int targetValue = stoi(item);
                    isLessOrEqual = posValue <= targetValue;
                    isEqual = posValue == targetValue;
                } else {
                    isLessOrEqual = posNode->data[fieldIndex] <= item;
                    isEqual = posNode->data[fieldIndex] == item;
                }
                
                if (isEqual) {
                    last = pos;
                    left = pos + 1;  // Continue searching to the right for last occurrence
                } else if (isLessOrEqual) {
                    left = pos + 1;
                } else {
                    right = pos - 1;
                }
            }
        }

        /*  Linear Search Range, 
            returns the reference of fist and last index from a sorted linked list 
        */
        void linearSearchRange(Node* head, int size, const string& item, const int fieldIndex, int& first, int& last) {
            // Initialize first and last to invalid indices
            first = -1;
            last = -1;
            
            // If list is empty, return with invalid indices
            if (!head || size <= 0) {
                return;
            }

            for (int i = 0; i < size; i++) {
                
                Node* current = getNodeAtIndex(head, i);

                if (!current) break;  // Safety check
                
                if (current->data[fieldIndex] == item){
                    
                    if (first == -1) {
                        first = i;
                    }
                    // Update last to the current position
                    last = i;
                }
                
                current = current->next;
            }
        }


        Node* subLinkedList(Node* head, int first, int last, int x) {
            if (!head || first < 0 || last < first) {
                return nullptr;
            }
        
            // Traverse to the `first` node
            Node* current = getNodeAtIndex(head,first);
        
            if (!current) return nullptr; // Index out of range
        
            // Create the head of the new sublist
            Node* newHead = new Node();
            newHead->data = new string[x];
            for (int i = 0; i < x; ++i) {
                newHead->data[i] = current->data[i];
            }
        
            Node* subCurrent = newHead;
            current = current->next;
        
            for (int i = first + 1; i <= last && current; ++i) {
                Node* newNode = new Node();
                newNode->data = new string[x];
                for (int j = 0; j < x; ++j) {
                    newNode->data[j] = current->data[j];
                }
        
                subCurrent->next = newNode;
                subCurrent = newNode;
                current = current->next;
            }
        
            subCurrent->next = nullptr;
            return newHead;
        }
        
        //  mergeSort function with proper merge step
        Node* mergeSort(Node* head, const int length, const int fieldIndex) {
            if (length <= 1 || head == nullptr) {
                return head;
            }
        
            // Detect column type from the first node
            bool isDateColumn = (head->data[fieldIndex].length() >= 10 && head->data[fieldIndex][2] == '/' && head->data[fieldIndex][5] == '/');
            bool isDecimalColumn = !isDateColumn && isDecimal(head->data[fieldIndex]);
            bool isNumericColumn = !isDateColumn && !isDecimalColumn && isNumeric(head->data[fieldIndex]);
        
            // Split the list
            int mid = length / 2;
            Node* midNode = getNodeAtIndex(head, mid - 1);
            if (!midNode){
                return head; // Safety check
            } 
            
            Node* rightHead = midNode->next;
            midNode->next = nullptr;
        
            // Sort both halves
            Node* leftSorted = mergeSort(head, mid, fieldIndex);
            Node* rightSorted = mergeSort(rightHead, length - mid, fieldIndex);
        
            // Create a dummy head for easier merging
            Node dummy;
            Node* tail = &dummy;
        
            // Merge the two sorted halves
            while (leftSorted && rightSorted) {
                bool takeLeft = false;
                
                // Compare based on data types
                if (isDateColumn) {
                    takeLeft = parseDateString(leftSorted->data[fieldIndex]) <= parseDateString(rightSorted->data[fieldIndex]);
                } else if (isDecimalColumn) {
                    takeLeft = stod(leftSorted->data[fieldIndex]) <= stod(rightSorted->data[fieldIndex]);
                } else if (isNumericColumn) {
                    takeLeft = stoi(leftSorted->data[fieldIndex]) <= stoi(rightSorted->data[fieldIndex]);
                } else {
                    takeLeft = leftSorted->data[fieldIndex] <= rightSorted->data[fieldIndex];
                }
                
                // Add the smaller node to result
                if (takeLeft) {
                    tail->next = leftSorted;
                    leftSorted = leftSorted->next;
                } else {
                    tail->next = rightSorted;
                    rightSorted = rightSorted->next;
                }
                
                tail = tail->next;
            }
            
            // Attach remaining nodes
            if (leftSorted != nullptr) {
                tail->next = leftSorted;
            } else {
                tail->next = rightSorted;
            }
            
            return dummy.next;
        }

        Node* quickSort(Node* head, const int length, const int fieldIndex) {
            // Base case: empty list or single node list is already sorted
            if (!head || !head->next) {
                return head;
            }
            
            // Create three separate lists: less than pivot, equal to pivot, and greater than pivot
            Node dummyLess, dummyEqual, dummyGreater;
            Node* tailLess = &dummyLess;
            Node* tailEqual = &dummyEqual;
            Node* tailGreater = &dummyGreater;
            
            // Use the first node's value as the pivot
            string pivotValue = head->data[fieldIndex];
            
            // Detect column type from the first node
            bool isDateColumn = (head->data[fieldIndex].length() >= 10 && head->data[fieldIndex][2] == '/' && head->data[fieldIndex][5] == '/');
            bool isDecimalColumn = !isDateColumn && isDecimal(head->data[fieldIndex]);
            bool isNumericColumn = !isDateColumn && !isDecimalColumn && isNumeric(head->data[fieldIndex]);
            
            // Partition the list into three parts
            Node* current = head;
            Node* next = nullptr;
            int lessCount = 0;
            int equalCount = 0;
            int greaterCount = 0;
            
            while (current) {
                // Save next pointer because we'll modify current->next
                next = current->next;
                current->next = nullptr;
                
                bool isLess = false;
                bool isEqual = false;
                
                // Compare based on data types
                if (isDateColumn) {
                    long currentDate = parseDateString(current->data[fieldIndex]);
                    long pivotDate = parseDateString(pivotValue);
                    isLess = currentDate < pivotDate;
                    isEqual = currentDate == pivotDate;
                } else if (isDecimalColumn) {
                    double currentVal = stod(current->data[fieldIndex]);
                    double pivotVal = stod(pivotValue);
                    isLess = currentVal < pivotVal;
                    isEqual = currentVal == pivotVal;
                } else if (isNumericColumn) {
                    int currentVal = stoi(current->data[fieldIndex]);
                    int pivotVal = stoi(pivotValue);
                    isLess = currentVal < pivotVal;
                    isEqual = currentVal == pivotVal;
                } else {
                    // String comparison
                    isLess = current->data[fieldIndex] < pivotValue;
                    isEqual = current->data[fieldIndex] == pivotValue;
                }
                
                // Add to appropriate list
                if (isLess) {
                    tailLess->next = current;
                    tailLess = current;
                    lessCount++;
                } else if (isEqual) {
                    tailEqual->next = current;
                    tailEqual = current;
                    equalCount++;
                } else {
                    tailGreater->next = current;
                    tailGreater = current;
                    greaterCount++;
                }
                
                current = next;
            }
            
            // Recursively sort the "less than" and "greater than" lists
            Node* sortedLess = nullptr;
            if (lessCount > 0) {
                sortedLess = quickSort(dummyLess.next, lessCount, fieldIndex);
                // Find the tail of the sorted less list
                tailLess = sortedLess;
                while (tailLess && tailLess->next) {
                    tailLess = tailLess->next;
                }
            }
            
            Node* sortedGreater = nullptr;
            if (greaterCount > 0) {
                sortedGreater = quickSort(dummyGreater.next, greaterCount, fieldIndex);
            }
            
            // Connect the three lists: less -> equal -> greater
            if (sortedLess) {
                // If we have a "less than" list, connect it to the "equal" list
                tailLess->next = dummyEqual.next;
                tailEqual->next = sortedGreater;
                return sortedLess;
            } else {
                // If no "less than" list, start with the "equal" list
                tailEqual->next = sortedGreater;
                return dummyEqual.next;
            }
        }
 
        // Selection Sort implementation
        Node* selectSort(Node* head, const int length, const int fieldIndex) {
            if (!head || !head->next) return head;
            
            // First node to check column type
            bool isDateColumn = (head->data[fieldIndex].length() >= 10 && head->data[fieldIndex][2] == '/' && head->data[fieldIndex][5] == '/');
            bool isDecimalColumn = !isDateColumn && isDecimal(head->data[fieldIndex]);
            bool isNumericColumn = !isDateColumn && !isDecimalColumn && isNumeric(head->data[fieldIndex]);
            
            Node* current = head;
            
            // Iterate through the list
            while (current) {
                Node* minNode = current;
                Node* temp = current->next;
                
                // Find the minimum value in the remaining list
                while (temp) {
                    bool isLess = false;
                    
                    // Compare based on data types
                    if (isDateColumn) {
                        isLess = parseDateString(temp->data[fieldIndex]) < parseDateString(minNode->data[fieldIndex]);
                    } else if (isDecimalColumn) {
                        isLess = stod(temp->data[fieldIndex]) < stod(minNode->data[fieldIndex]);
                    } else if (isNumericColumn) {
                        isLess = stoi(temp->data[fieldIndex]) < stoi(minNode->data[fieldIndex]);
                    } else {
                        isLess = temp->data[fieldIndex] < minNode->data[fieldIndex];
                    }
                    
                    if (isLess) {
                        minNode = temp;
                    }
                    
                    temp = temp->next;
                }
                
                // Swap data if minimum is not the current node
                if (minNode != current) {
                    // Swap all data fields
                    // Assuming data is an array of strings
                    string* tempData = current->data;
                    current->data = minNode->data;
                    minNode->data = tempData;
                }
                
                current = current->next;
            }
            
            return head;
        }

        // Bubble Sort implementation
        Node* bubbleSort(Node* head, const int length, const int fieldIndex) {
            
            if (!head || !head->next){
                return head;
            } 
            
            // First node to check column type
            bool isDateColumn = (head->data[fieldIndex].length() >= 10 && head->data[fieldIndex][2] == '/' && head->data[fieldIndex][5] == '/');
            bool isDecimalColumn = !isDateColumn && isDecimal(head->data[fieldIndex]);
            bool isNumericColumn = !isDateColumn && !isDecimalColumn && isNumeric(head->data[fieldIndex]);
            
            int swapped;
            Node* current;
            Node* lastSorted = nullptr;
            
            // Keep iterating until no more swaps are needed
            do {
                swapped = 0;
                current = head;
                
                while (current->next != lastSorted) {
                    bool shouldSwap = false;
                    
                    // Compare based on data types
                    if (isDateColumn) {
                        shouldSwap = parseDateString(current->data[fieldIndex]) > parseDateString(current->next->data[fieldIndex]);
                    } else if (isDecimalColumn) {
                        shouldSwap = stod(current->data[fieldIndex]) > stod(current->next->data[fieldIndex]);
                    } else if (isNumericColumn) {
                        shouldSwap = stoi(current->data[fieldIndex]) > stoi(current->next->data[fieldIndex]);
                    } else {
                        shouldSwap = current->data[fieldIndex] > current->next->data[fieldIndex];
                    }
                    
                    if (shouldSwap) {
                        // Swap data
                        // Assuming data is an array of strings
                        string* tempData = current->data;
                        current->data = current->next->data;
                        current->next->data = tempData;
                        swapped = 1;
                    }
                    
                    current = current->next;
                }
                
                // Update the last sorted node
                lastSorted = current;
                
            } while (swapped);
            
            return head;
        }

    private:
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
        
        //  Get the head Node
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
            } catch (const std::out_of_range&) {
                return false; // FIXED: Also catch out_of_range exception
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
};
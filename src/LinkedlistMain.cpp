#include "LinkedListHeader.hpp"
#include <iostream>
#include <string>

using namespace std;

int main() {
    // Create a Functions object
    Functions functions;
 
    // Use the specified file path
    string filepath = "transactions_cleaned.csv";
    
    // Read the CSV file - THIS LINE WAS MISSING
    cout << "Reading file: " << filepath << endl;
    LinkedList data = functions.getdata(filepath);

    cout << data.error;

    // Check if there was an error reading the file
    if (data.error == 1) {
         cout << "Error occurred while reading the file." << endl;
         return 0;
    }
    else{
        // cout << "[DEBUG] data.error = " << data.error << endl;
        // cout << "[DEBUG] fieldHead is " << (data.fieldHead ? "not null" : "null") << endl;
        // cout << "[DEBUG] head is " << (data.head ? "not null" : "null") << endl;

        // Display the data
        cout << "\n==== CSV File Contents ====\n" << endl;
        data.head = functions.mergeSort(data.head, data.y, 2);
        data.tail = functions.getNodeTail(data.head);
        // functions.displayTabulatedData(data);
        cout << functions.fibMonaccianSearch(data.head, data.y, "Electronics", 2) << " Index" << endl;
        
        // Show some stats about the data
        cout << "\n==== File Statistics ====\n" << endl;
        cout << "Number of columns: " << data.x << endl;
        cout << "Number of data rows: " << data.y << endl;
        cout << "Total cells: " << data.x * data.y << endl;  
    }
    return 0;
}
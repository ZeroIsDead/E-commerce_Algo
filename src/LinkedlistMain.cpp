#include "LinkedListHeader.hpp"
#include <iostream>
#include <string>

using namespace std;

int main() {
    // Create a Functions object
    Functions functions;
 
    // Use the specified file path
    string filepath = "reviews_cleaned";
    
    // Read the CSV file - THIS LINE WAS MISSING
    cout << "Reading file: " << filepath << endl;
    LinkedList data = functions.getdata(filepath);

    cout << data.error;

    // Check if there was an error reading the file
    if (data.error == 1) {
         cout << "Error occurred while reading the file." << endl;
         return 0;
    }
    
    Timer timer = Timer();
    
    timer.start();
    data.head = functions.mergeSort(data.head, data.y, 2);
    data.tail = functions.getNodeTail(data.head);
    cout << "Merge Sort ";
    timer.end();

    int first, last;

    functions.binarySearchRange(data.head, data.y, "1", 2, first, last);

    cout << first << " " << last << endl;
    data.head = functions.subLinkedList(data.head, first, last, data.x);
    data.tail = functions.getNodeTail(data.head);
    data.y = last-first+1;
    
    data.head = functions.mergeSort(data.head,data.y,3);
    data.tail = functions.getNodeTail(data.head);

    LinkedList data2 = functions.repeatingItem(data.head, data.y, 3, data.x);

    data2.head = functions.mergeSort(data2.head, data.y, 1);
    data2.tail = functions.getNodeTail(data2.head);
    
    functions.displayTabulatedData(data2);
    

    // Show some stats about the data
    cout << "\n==== File Statistics ====\n" << endl;
    cout << "Number of columns: " << data.x << endl;
    cout << "Number of data rows: " << data.y << endl;
    cout << "Total cells: " << data.x * data.y << endl;
    return 0;
}
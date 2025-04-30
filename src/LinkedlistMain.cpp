#include "LinkedListHeader.hpp"
#include <iostream>
#include <string>

using namespace std;

int main() {
    // Create a Functions object
    Functions functions;
 
    // Use the specified file path
    string filepath = "transactions_cleaned";
    
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
        Timer timer = Timer();
        
        timer.start();
        data.head = functions.mergeSort(data.head, data.y, 4);
        data.tail = functions.getNodeTail(data.head);
        cout << "Merge Sort ";
        timer.end();
        
        //functions.displayTabulatedData(data);
        
        int first, last;

        timer.start();
        functions.fibMonaccianSearchRange(data.head, data.y, "01/01/2024", 4, first, last);
        cout << "Binary Search \"01/01/2024\" ";
        timer.end();

        data.head = functions.subLinkedList(data.head, first, last, data.x);
        data.tail = functions.getNodeTail(data.head);
        data.y = last-first + 1;

        timer.start();
        data.head = functions.quickSort(data.head, data.y,1);
        data.tail = functions.getNodeTail(data.head);
        cout << "Quick sort";
        timer.end();

        timer.start();
        functions.fibMonaccianSearchRange(data.head, data.y, "Gaming Console", 1, first, last);
        cout << "Binary Search \"Gaming Console\"";
        timer.end();


        data.head = functions.subLinkedList(data.head, first,last, data.x);
        data.tail = functions.getNodeTail(data.head);
        data.y = last-first + 1;

        functions.displayTabulatedData(data);

        // functions.displayTabulatedData(data);
        
        // Show some stats about the data
        cout << "\n==== File Statistics ====\n" << endl;
        cout << "Number of columns: " << data.x << endl;
        cout << "Number of data rows: " << data.y << endl;
        cout << "Total cells: " << data.x * data.y << endl;  
    }
    return 0;
}
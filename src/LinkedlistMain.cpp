#include "LinkedListHeader.hpp"
#include <iostream>
#include <string>

using namespace std;

void printMenu(){
    cout << "1) Number of Transcation for Days" << endl;
    cout << "2) Purchase made under \"Electronic\" using \"Credit Card\"" << endl;
    cout << "3) Recurring sentences for 1 star review" << endl;
};

int inputHandl(int lowestNumber, int highestNumber){
    int input;
    cout << "Please enter you choice: ";
    cin >> input;

    while (input < lowestNumber || input > highestNumber){
        cout << "Wrong Input";
        inputHandl(lowestNumber,highestNumber);
    }
    cout << endl;
    return input;
}

int main() {
    // Create a Functions object
    Functions functions;
    Timer timer = Timer();

    // Use the specified file path
    string review = "reviews_cleaned";
    string transaction = "transactions_cleaned";
    
    //LinkedList reviewList = functions.getdata(review);
    //LinkedList transactionList = functions.getdata(transaction);

    // Check if there was an error reading the file
    
    printMenu();
    int input = inputHandl(1,3);

    if (input == 1){

        LinkedList transactionList1 = functions.getdata(transaction);
        LinkedList transactionList2 = functions.getdata(transaction);
        LinkedList transactionList3 = functions.getdata(transaction);
        LinkedList transactionList4 = functions.getdata(transaction);


        timer.start();
        transactionList1.head = functions.mergeSort(transactionList1.head, transactionList1.y, 4);
        cout << "Merge sort by date ";
        timer.end();

        timer.start();
        transactionList2.head = functions.bubbleSort(transactionList2.head, transactionList2.y, 4);
        cout << "Bubble sort by date ";
        timer.end();

        timer.start();
        transactionList3.head = functions.quickSort(transactionList3.head, transactionList3.y, 4);
        cout << "Quick sort by date ";
        timer.end();

        timer.start();
        transactionList4.head = functions.selectSort(transactionList4.head, transactionList4.y, 4);
        cout << "Select sort by date ";
        timer.end();

        int total = transactionList4.y;

        transactionList1.tail = functions.getNodeTail(transactionList1.head);
        transactionList2.tail = functions.getNodeTail(transactionList2.head);
        transactionList3.tail = functions.getNodeTail(transactionList3.head);
        transactionList4.tail = functions.getNodeTail(transactionList4.head);


        LinkedList repeatingdate1 = functions.repeatingItem(transactionList1.head,transactionList1.y,4,transactionList1.x);

        repeatingdate1.head = functions.quickSort(repeatingdate1.head, repeatingdate1.y, 1);
        repeatingdate1.tail = functions.getNodeTail(repeatingdate1.head);

        functions.displayTabulatedData(repeatingdate1);
        cout << "The Total number of transactions is: " << total << endl;
        cout << endl;
        main();
    }
    else if (input == 2){
        
        LinkedList transactionList1 = functions.getdata(transaction);
        LinkedList transactionList2 = functions.getdata(transaction);
        LinkedList transactionList3 = functions.getdata(transaction);
        LinkedList transactionList4 = functions.getdata(transaction);

        int total = transactionList1.y;

        int first, last;

        timer.start();
        transactionList1.head = functions.mergeSort(transactionList1.head, transactionList1.y, 2);
        cout << "Merge sort by Catagory ";
        timer.end();

        transactionList1.tail = functions.getNodeTail(transactionList1.head);

        timer.start();
        functions.fibMonaccianSearchRange(transactionList1.head,transactionList1.y,"Electronics",2,first,last);
        cout << "Fibonacci Seach for Electronics ";
        timer.end();

        transactionList1.head = functions.subLinkedList(transactionList1.head,first,last,transactionList1.x);
        transactionList1.tail = functions.getNodeTail(transactionList1.head);
        int electroncisTotal = transactionList1.y = (last - first)+1;

        timer.start();
        transactionList1.head = functions.mergeSort(transactionList1.head, transactionList1.y, 5);
        cout << "Merge Sort for Payment Method ";
        timer.end();

        timer.start();
        functions.fibMonaccianSearchRange(transactionList1.head,transactionList1.y,"Credit Card",5,first,last);
        cout << "Fibonacci Seach for Credit Card ";
        timer.end();
        
        transactionList1.head = functions.subLinkedList(transactionList1.head,first,last,transactionList1.x);
        transactionList1.tail = functions.getNodeTail(transactionList1.head);
        transactionList1.y= (last - first)+1;

        timer.start();
        transactionList2.head = functions.quickSort(transactionList2.head, transactionList2.y, 2);
        cout << "Quick sort by Catagory ";
        timer.end();

        transactionList2.tail = functions.getNodeTail(transactionList2.head);

        timer.start();
        functions.interpolationSearchRange(transactionList2.head,transactionList2.y,"Electronics",2,first,last);
        cout << "interpolation Seach for Electronics ";
        timer.end();

        transactionList2.head = functions.subLinkedList(transactionList2.head,first,last,transactionList2.x);
        transactionList2.tail = functions.getNodeTail(transactionList2.head);
        transactionList2.y= (last - first)+1;

        timer.start();
        transactionList2.head = functions.quickSort(transactionList2.head, transactionList2.y, 5);
        cout << "Quick Sort for Payment Method ";
        timer.end();

        timer.start();
        functions.interpolationSearchRange(transactionList2.head,transactionList2.y,"Credit Card",5,first,last);
        cout << "Interpolation Seach for Credit Card ";
        timer.end();
        
        transactionList2.head = functions.subLinkedList(transactionList2.head,first,last,transactionList2.x);
        transactionList2.tail = functions.getNodeTail(transactionList2.head);
        transactionList2.y= (last - first)+1;
        
        timer.start();
        transactionList3.head = functions.bubbleSort(transactionList3.head, transactionList3.y, 2);
        cout << "Bubble sort by Catagory ";
        timer.end();

        transactionList3.tail = functions.getNodeTail(transactionList3.head);

        timer.start();
        functions.binarySearchRange(transactionList3.head,transactionList3.y,"Electronics",2,first,last);
        cout << "Binary Seach for Electronics ";
        timer.end();

        transactionList3.head = functions.subLinkedList(transactionList3.head,first,last,transactionList3.x);
        transactionList3.tail = functions.getNodeTail(transactionList3.head);
        transactionList3.y= (last - first)+1;

        timer.start();
        transactionList3.head = functions.quickSort(transactionList3.head, transactionList3.y, 5);
        cout << "Bubble Sort for Payment Method ";
        timer.end();

        timer.start();
        functions.binarySearchRange(transactionList3.head,transactionList3.y,"Credit Card",5,first,last);
        cout << "Binary Seach for Credit Card ";
        timer.end();
        
        transactionList3.head = functions.subLinkedList(transactionList3.head,first,last,transactionList3.x);
        transactionList3.tail = functions.getNodeTail(transactionList3.head);
        transactionList3.y= (last - first)+1;

        timer.start();
        transactionList4.head = functions.selectSort(transactionList4.head, transactionList4.y, 2);
        cout << "Select sort by Catagory ";
        timer.end();

        transactionList4.tail = functions.getNodeTail(transactionList4.head);

        timer.start();
        functions.linearSearchRange(transactionList4.head,transactionList4.y,"Electronics",2,first,last);
        cout << "Linear Seach for Electronics ";
        timer.end();

        transactionList4.head = functions.subLinkedList(transactionList4.head,first,last,transactionList4.x);
        transactionList4.tail = functions.getNodeTail(transactionList4.head);
        transactionList4.y= (last - first)+1;

        timer.start();
        transactionList4.head = functions.selectSort(transactionList4.head, transactionList4.y, 5);
        cout << "Select Sort for Payment Method ";
        timer.end();

        timer.start();
        functions.linearSearchRange(transactionList4.head,transactionList4.y,"Credit Card",5,first,last);
        cout << "Linear Seach for Credit Card ";
        timer.end();
        
        transactionList4.head = functions.subLinkedList(transactionList4.head,first,last,transactionList4.x);
        transactionList4.tail = functions.getNodeTail(transactionList4.head);
        transactionList4.y= (last - first)+1;

        double percentage1 = ((double)transactionList1.y/(double)total)*100;
        double percentage2 = ((double)transactionList1.y/(double)electroncisTotal)*100;
        
        functions.displayTabulatedData(transactionList3);
        cout << "Percentage of \"Electronics\" and \"Credit Card\" by all transactions: " << percentage1 << "%" << endl;
        cout << "Percentage of \"Electronics\" and \"Credit Card\" by all Electronic transactions: " << percentage2 << "%" << endl;
        cout << endl;
        main();
        
    }
    else{
        LinkedList reviewList1 = functions.getdata(review);
        LinkedList reviewList2 = functions.getdata(review);
        LinkedList reviewList3 = functions.getdata(review);
        LinkedList reviewList4 = functions.getdata(review);

        int first, last;

        timer.start();
        reviewList1.head = functions.mergeSort(reviewList1.head, reviewList1.y, 2);
        cout << "Merge sort by rating ";
        timer.end();

        reviewList1.tail = functions.getNodeTail(reviewList1.head);

        timer.start();
        functions.fibMonaccianSearchRange(reviewList1.head,reviewList1.y,"1",2,first,last);
        cout << "Fibonacci Seach for 1 star reviews ";
        timer.end();

        reviewList1.head = functions.subLinkedList(reviewList1.head,first,last,reviewList1.x);
        reviewList1.tail = functions.getNodeTail(reviewList1.head);
        reviewList1.y = (last - first)+1;

        timer.start();
        reviewList2.head = functions.quickSort(reviewList2.head, reviewList2.y, 2);
        cout << "Quick sort by rating ";
        timer.end();

        reviewList2.tail = functions.getNodeTail(reviewList2.head);

        timer.start();
        functions.interpolationSearchRange(reviewList2.head,reviewList2.y,"1",2,first,last);
        cout << "interpolation Seach for 1 ";
        timer.end();

        reviewList2.head = functions.subLinkedList(reviewList2.head,first,last,reviewList2.x);
        reviewList2.tail = functions.getNodeTail(reviewList2.head);
        reviewList2.y= (last - first)+1;

        timer.start();
        reviewList3.head = functions.bubbleSort(reviewList3.head, reviewList3.y, 2);
        cout << "Bubble sort by rating ";
        timer.end();

        reviewList3.tail = functions.getNodeTail(reviewList3.head);

        timer.start();
        functions.binarySearchRange(reviewList3.head,reviewList3.y,"1",2,first,last);
        cout << "Binary Seach for 1 star rating ";
        timer.end();
        
        reviewList3.head = functions.subLinkedList(reviewList3.head,first,last,reviewList3.x);
        reviewList3.tail = functions.getNodeTail(reviewList3.head);
        reviewList3.y= (last - first)+1;

        timer.start();
        reviewList4.head = functions.selectSort(reviewList4.head, reviewList4.y, 2);
        cout << "Select sort by rating ";
        timer.end();

        reviewList4.tail = functions.getNodeTail(reviewList4.head);
        
        timer.start();
        functions.linearSearchRange(reviewList4.head,reviewList4.y,"1",2,first,last);
        cout << "Linear Seach for 1 star review ";
        timer.end();
        
        reviewList4.head = functions.subLinkedList(reviewList4.head,first,last,reviewList4.x);
        reviewList4.tail = functions.getNodeTail(reviewList4.head);
        reviewList4.y= (last - first)+1;

        timer.start();
        reviewList1.head = functions.mergeSort(reviewList1.head,reviewList1.y,3);
        cout << "Merge sort for reviews ";
        timer.end();
        reviewList1.tail - functions.getNodeTail(reviewList1.head);

        timer.start();
        reviewList2.head = functions.quickSort(reviewList2.head,reviewList2.y,3);
        cout << "Quick sort for reviews ";
        timer.end();
        reviewList2.tail - functions.getNodeTail(reviewList2.head);

        timer.start();
        reviewList3.head = functions.bubbleSort(reviewList3.head,reviewList3.y,3);
        cout << "Bubble sort for reviews ";
        timer.end();
        reviewList3.tail - functions.getNodeTail(reviewList3.head);

        timer.start();
        reviewList4.head = functions.selectSort(reviewList4.head,reviewList3.y,3);
        cout << "Select sort for reviews ";
        timer.end();
        reviewList4.tail - functions.getNodeTail(reviewList4.head);

        LinkedList repeangReview = functions.repeatingItem(reviewList2.head,reviewList2.y,3,reviewList1.x);

        functions.displayTabulatedData(repeangReview);
        cout << endl;
        main();
    }

    return 0;
}
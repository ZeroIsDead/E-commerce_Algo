#include "ArrayHeader.h"
#include <iostream>
#include <string>

using namespace std;

void printMenu(){
    cout << "1) Number of Transcation for Days" << endl;
    cout << "2) Purchase made under \"Electronic\" using \"Credit Card\"" << endl;
    cout << "3) Recurring sentences for 1 star review" << endl;
    cout << "4) Recurring words for 1 star review" << endl;
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
    
    int input;
    
    // Create a Functions object
    Functions functions;
    Timer timer = Timer();
 
    string transactionFilepath = "transactions_cleaned";
    string reviewFilepath = "reviews_cleaned";

    printMenu();
    input = inputHandl(1,4);

    if (input == 1){
        
        DataContainer2d transactionData1 = functions.getdata(transactionFilepath);
        DataContainer2d transactionData2 = functions.getdata(transactionFilepath);
        DataContainer2d transactionData3 = functions.getdata(transactionFilepath);
        DataContainer2d transactionData4 = functions.getdata(transactionFilepath);

        timer.start();
        functions.mergeSort(transactionData1, 4);
        cout << "Merge Sort ";
        timer.end();

        timer.start();
        functions.selectionSort(transactionData2, 4);
        cout << "Selection Sort ";
        timer.end();
        
        timer.start();
        functions.bubbleSort(transactionData3,4);
        cout << "Bubble Sort ";
        timer.end();
        
        timer.start();
        functions.quickSort(transactionData4,4);
        cout << "Quick Sort";
        timer.end();

        DataContainer2d repeatingData = functions.repeatingItem(transactionData4,4);
        functions.mergeSort(repeatingData,1);

        int totalTransacation = functions.NumberofTransactions(repeatingData);

        functions.displayTabulatedData(repeatingData);
        cout << endl;
        cout << "The Total Number of Transactions is: " << totalTransacation << endl; 

        main();
    }
    else if (input == 2){
        int first, last;

        DataContainer2d transactionData1 = functions.getdata(transactionFilepath);
        DataContainer2d transactionData2 = functions.getdata(transactionFilepath);
        DataContainer2d transactionData3 = functions.getdata(transactionFilepath);
        DataContainer2d transactionData4 = functions.getdata(transactionFilepath);

        int totalDataSize = transactionData1.y;

        timer.start();
        functions.bubbleSort(transactionData1,2);
        cout << "Bubble Sort for Catagoy ";
        timer.end();

        timer.start();
        functions.selectionSort(transactionData2,2);
        cout << "Select Sort Catagoy  ";
        timer.end();

        timer.start();
        functions.quickSort(transactionData3,2);
        cout << "Quick Sort Catagoy  ";
        timer.end();

        timer.start();
        functions.mergeSort(transactionData4,2);
        cout << "Merge Sort Catagoy  ";
        timer.end();

        timer.start();
        functions.binarySearchRange(transactionData1.data, transactionData1.y, 2, "Electronics", first, last);
        cout << "Binary Search for Electronics  ";
        timer.end();

        transactionData1 = functions.subDataContainer2d(transactionData1, first, last);
        int electronicSize = transactionData1.y;

        timer.start();
        functions.bubbleSort(transactionData1, 5);
        cout << "Bubble sort Payment Methods ";
        timer.end();

        timer.start();
        functions.binarySearchRange(transactionData1.data, transactionData1.y, 5, "Credit Card", first, last);
        cout << "Binary Search for Credit Card ";
        timer.end();

        transactionData1 = functions.subDataContainer2d(transactionData1, first, last);
        
        timer.start();
        functions.linearSearchRange(transactionData2.data, transactionData2.y, 2, "Electronics", first, last);
        cout << "Linear Search for Electronics  ";
        timer.end();

        transactionData2 = functions.subDataContainer2d(transactionData2, first, last);
        
        timer.start();
        functions.selectionSort(transactionData2, 5);
        cout << "Selection sort Payment Methods ";
        timer.end();

        timer.start();
        functions.linearSearchRange(transactionData2.data, transactionData2.y, 5, "Credit Card", first, last);
        cout << "Linear Search for Credit Card ";
        timer.end();

        transactionData2 = functions.subDataContainer2d(transactionData2, first, last);

        timer.start();
        functions.interpolationSearchRange(transactionData3.data, transactionData3.y, 2, "Electronics", first, last);
        cout << "Interpolation Search for Electronics ";
        timer.end();

        transactionData3 = functions.subDataContainer2d(transactionData3, first, last);
        
        timer.start();
        functions.quickSort(transactionData3, 5);
        cout << "Quick sort Payment Methods ";
        timer.end();

        timer.start();
        functions.interpolationSearchRange(transactionData3.data, transactionData3.y, 5, "Credit Card", first, last);
        cout << "Interpolation Search for Credit Card ";
        timer.end();

        transactionData3 = functions.subDataContainer2d(transactionData3, first, last);

        timer.start();
        functions.fibonacciSearchRange(transactionData4.data, transactionData4.y, 2, "Electronics", first, last);
        cout << "Fibonacci Search for Electornics ";
        timer.end();

        transactionData4 = functions.subDataContainer2d(transactionData4, first, last);
        
        timer.start();
        functions.mergeSort(transactionData4, 5);
        cout << "Meger sort Payment Methods ";
        timer.end();

        timer.start();
        functions.fibonacciSearchRange(transactionData4.data, transactionData4.y, 5, "Credit Card", first, last);
        cout << "Fibonacci Search for Credit Card ";
        timer.end();

        transactionData4 = functions.subDataContainer2d(transactionData4, first, last);

        functions.displayTabulatedData(transactionData4);

        double percentage = ((double)transactionData1.y/(double)totalDataSize)*100;

        double percentage2 = ((double)transactionData1.y/(double)electronicSize)*100;

        cout << "Percentage of \"Electronics\" and \"Credit Card\" by all transactions: " << percentage << "%" << endl;
        cout << "Percentage of \"Electronics\" and \"Credit Card\" by all Electronic transactions: " << percentage2 << "%" << endl;
        cout << endl;
        main();
    }
    else if (input == 3){

        int first, last;

        DataContainer2d reviewData1 = functions.getdata(reviewFilepath);
        DataContainer2d reviewData2 = functions.getdata(reviewFilepath);
        DataContainer2d reviewData3 = functions.getdata(reviewFilepath);
        DataContainer2d reviewData4 = functions.getdata(reviewFilepath);

        timer.start();
        functions.bubbleSort(reviewData1, 2);
        cout << "Bubble Sort for review ";
        timer.end();

        timer.start();
        functions.binarySearchRange(reviewData1.data, reviewData1.y, 2, "1", first, last);
        cout<< "Binary Search for all 1 star review ";
        timer.end();

        timer.start();
        functions.selectionSort(reviewData1, 2);
        cout << "Selection Sort for review ";
        timer.end();

        timer.start();
        functions.linearSearchRange(reviewData1.data, reviewData1.y, 2, "1", first, last);
        cout<< "Linear Search for all 1 star review ";
        timer.end();

        timer.start();
        functions.mergeSort(reviewData1, 2);
        cout << "Merge Sort for review ";
        timer.end();

        timer.start();
        functions.fibonacciSearchRange(reviewData1.data, reviewData1.y, 2, "1", first, last);
        cout<< "Finbonacci Search for all 1 star review ";
        timer.end();

        timer.start();
        functions.quickSort(reviewData1, 2);
        cout << "Quick Sort for review ";
        timer.end();

        timer.start();
        functions.interpolationSearchRange(reviewData1.data, reviewData1.y, 2, "1", first, last);
        cout<< "Interpolation Search for all 1 star review ";
        timer.end();

        reviewData1 = functions.subDataContainer2d(reviewData1,first,last);
        functions.quickSort(reviewData1, 3);

        reviewData1 = functions.repeatingItem(reviewData1, 3);

        functions.quickSort(reviewData1, 1);

        functions.reverseArray(reviewData1.data, reviewData1.y);

        functions.displayTabulatedData(reviewData1);
        cout << endl;

        main();
    }
    else{

        DataContainer2d reviewData1 = functions.getdata(reviewFilepath);
        DataContainer2d reviewData2 = functions.getdata(reviewFilepath);
        DataContainer2d reviewData3 = functions.getdata(reviewFilepath);
        DataContainer2d reviewData4 = functions.getdata(reviewFilepath);

        int first, last;

        timer.start();
        functions.bubbleSort(reviewData1, 2);
        cout << "Bubble Sort for review ";
        timer.end();

        timer.start();
        functions.binarySearchRange(reviewData1.data, reviewData1.y, 2, "1", first, last);
        cout<< "Binary Search for all 1 star review ";
        timer.end();

        timer.start();
        functions.selectionSort(reviewData2, 2);
        cout << "Selection Sort for review ";
        timer.end();

        timer.start();
        functions.linearSearchRange(reviewData2.data, reviewData2.y, 2, "1", first, last);
        cout<< "Linear Search for all 1 star review ";
        timer.end();

        timer.start();
        functions.mergeSort(reviewData3, 2);
        cout << "Merge Sort for review ";
        timer.end();

        timer.start();
        functions.fibonacciSearchRange(reviewData3.data, reviewData3.y, 2, "1", first, last);
        cout<< "Finbonacci Search for all 1 star review ";
        timer.end();

        timer.start();
        functions.quickSort(reviewData4, 2);
        cout << "Quick Sort for review ";
        timer.end();

        timer.start();
        functions.interpolationSearchRange(reviewData4.data, reviewData4.y, 2, "1", first, last);
        cout<< "Interpolation Search for all 1 star review ";
        timer.end();

        reviewData1 = functions.subDataContainer2d(reviewData1,first,last);
        reviewData2 = functions.subDataContainer2d(reviewData2,first,last);
        reviewData3 = functions.subDataContainer2d(reviewData3,first,last);
        reviewData4 = functions.subDataContainer2d(reviewData4,first,last);

        reviewData1 = functions.words(reviewData1,3);
        reviewData2 = functions.words(reviewData2,3);
        reviewData3 = functions.words(reviewData3,3);
        reviewData4 = functions.words(reviewData4,3);
        
        timer.start();
        functions.bubbleSort(reviewData1,0);
        cout << "Bubble sort for words ";
        timer.end();

        timer.start();
        functions.selectionSort(reviewData2,0);
        cout << "Selection sort for words ";
        timer.end();
        
        timer.start();
        functions.mergeSort(reviewData3,0);
        cout << "Merge sort for words ";
        timer.end();
        
        timer.start();
        functions.quickSort(reviewData4,0);
        cout << "Quick sort for words ";
        timer.end();

        reviewData1 = functions.repeatingItem(reviewData1, 0);

        functions.quickSort(reviewData1,1);

        functions.displayTabulatedData(reviewData1);

        cout << endl;
        main();
    }

    
}
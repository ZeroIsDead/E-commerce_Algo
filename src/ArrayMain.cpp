#include "ArrayHeader.h"
#include <iostream>
#include <string>

using namespace std;

void printMenu(){
    cout << "1) Number of Transcation for Days" << endl;
    cout << "2) Purchase made under \"Electronic\" using \"Credit Card\"" << endl;
    cout << "3) Recurring words" << endl;
};

int inputHandl(int lowestNumber, int highestNumber){
    int input;
    cout << "Please enter you choice: ";
    cin >> input;

    while (input < lowestNumber || input > highestNumber){
        cout << "Wrong Input";
        inputHandl(lowestNumber,highestNumber);
    }

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
    input = inputHandl(1,3);

    if (input == 1){
        
        DataContainer2d transactionData1 = functions.getdata(transactionFilepath);
        DataContainer2d transactionData2 = functions.getdata(transactionFilepath);
        DataContainer2d transactionData3 = functions.getdata(transactionFilepath);
        DataContainer2d transactionData4 = functions.getdata(transactionFilepath);

        timer.start();
        functions.mergeSort(transactionData1.data, transactionData1.y, 4);
        cout << "Mege Sort ";
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
        functions.quickSort(transactionData1,5);
        cout << "Quick Sort";
        timer.end();

        timer.start();
        functions.quickSort(transactionData2,5);
        cout << "Quick Sort";
        timer.end();

        timer.start();
        functions.quickSort(transactionData3,5);
        cout << "Quick Sort";
        timer.end();

        timer.start();
        functions.quickSort(transactionData4,5);
        cout << "Quick Sort";
        timer.end();

        timer.start();
        functions.binarySearchRange(transactionData1.data, transactionData1.y, 5, "Credit Card", first, last);
        cout << "Binary Search for Credit Card ";
        timer.end();

        transactionData1 = functions.subDataContainer2d(transactionData1, first, last);
        
        functions.quickSort(transactionData1, 2);

        timer.start();
        functions.binarySearchRange(transactionData1.data, transactionData1.y, 2, "Electronics", first, last);
        cout << "Binary Search for Electronics ";
        timer.end();

        transactionData1 = functions.subDataContainer2d(transactionData1, first, last);
        
        timer.start();
        functions.linearSearchRange(transactionData2.data, transactionData2.y, 5, "Credit Card", first, last);
        cout << "Linear Search for Credit Card ";
        timer.end();

        transactionData2 = functions.subDataContainer2d(transactionData2, first, last);
        
        functions.quickSort(transactionData2, 2);

        timer.start();
        functions.linearSearchRange(transactionData2.data, transactionData2.y, 2, "Electronics", first, last);
        cout << "Linear Search for Electronics ";
        timer.end();

        transactionData2 = functions.subDataContainer2d(transactionData2, first, last);

        timer.start();
        functions.interpolationSearchRange(transactionData3.data, transactionData3.y, 5, "Credit Card", first, last);
        cout << "Interpolation Search for Credit Card ";
        timer.end();

        transactionData3 = functions.subDataContainer2d(transactionData3, first, last);
        
        functions.quickSort(transactionData3, 2);

        timer.start();
        functions.interpolationSearchRange(transactionData3.data, transactionData3.y, 2, "Electronics", first, last);
        cout << "Interpolation Search for Electronics ";
        timer.end();

        transactionData3 = functions.subDataContainer2d(transactionData3, first, last);
        
        functions.displayTabulatedData(transactionData1);

        double percentage = ((double)transactionData1.y/(double)totalDataSize)*100;

        cout << "Percentage of \"Electronics\" and \"Credit Card\": " << percentage << "%" << endl;
    }
    else{

        int first, last;

        DataContainer2d reviewData1 = functions.getdata(reviewFilepath);
        DataContainer2d reviewData2 = functions.getdata(reviewFilepath);
        DataContainer2d reviewData3 = functions.getdata(reviewFilepath);
        DataContainer2d reviewData4 = functions.getdata(reviewFilepath);

        functions.quickSort(reviewData1, 2);
        functions.binarySearchRange(reviewData1.data, reviewData1.y, 2, "1", first, last);
        
        reviewData1 = functions.subDataContainer2d(reviewData1,first,last);

        functions.quickSort(reviewData1, 3);

        DataContainer2d repeatingReviews = functions.repeatingItem(reviewData1, 3);

        functions.quickSort(repeatingReviews, 1);

        functions.reverseArray(repeatingReviews.data, repeatingReviews.y);

        functions.displayTabulatedData(repeatingReviews);
    }

    
}
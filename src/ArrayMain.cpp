#include "ArrayHeader.h"
#include <iostream>
#include <string>

using namespace std;

void printMenu(){
    cout << "1) Number of Transcation for Days" << endl;
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
    
    DataContainer2d reviewData = functions.getdata(reviewFilepath);

    printMenu();
    input = inputHandl(1,1);

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

    }
    
}
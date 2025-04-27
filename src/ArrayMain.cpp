#include "ArrayHeader.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    // Create a Functions object
    Functions functions;
 
    // Use the specified file path
    //string filepath = "transactions_cleaned.csv";
    string filepath = "transactions_cleaned";
    
    // Read the CSV file - THIS LINE WAS MISSING
    cout << "Reading file: " << filepath << endl;
    DataContainer2d data = functions.getdata(filepath);

    // Load reviews file
    string reviewFilepath = "reviews_cleaned";
    cout << "Reading file: " << reviewFilepath << endl;
    DataContainer2d reviews = functions.getdata(reviewFilepath);

    // Check if there was an error reading the file
    if (data.error == 1 || reviews.error == 1) {
         cout << "Error occurred while reading the file." << endl;
         return 0;
    }
    else{
        // Display the data

        Timer timer = Timer();
        cout << "\n==== CSV File Contents ====\n" << endl;

        // timer.start();
        // data.data = functions.mergeSort(data.data, data.y, 1);
        // timer.end();

        // timer.start();
        // cout << functions.fibMonaccianSearch(data.data, data.y, "Mouse", 1) << endl;
        // timer.end();

        // timer.start();
        // functions.sortByFieldName(data, "Date", true); 
        // timer.end(); 

        DataContainer2d transactions = functions.getdata("transactions_cleaned");
        DataContainer2d reviews = functions.getdata("reviews_cleaned");
        functions.compareDatasets(transactions, reviews);
        
        

        ///functions.displayTabulatedData(data);

        // Show some stats about the data
        cout << "\n==== File Statistics ====\n" << endl;
        cout << "Number of columns: " << data.x << endl;
        cout << "Number of data rows: " << data.y << endl;
        cout << "Total cells: " << data.x * data.y << endl;    
    }
    return 0;
}
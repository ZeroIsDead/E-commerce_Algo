#include "ArrayHeader.h"
#include <iostream>
#include <string>

using namespace std;

int main(){
    Functions function;
    Timer timer;

    DataContainer2d data = function.getdata("transactions_cleaned.csv");

    DataContainer2d data2 = function.getdata("transactions_cleaned.csv");
    
    timer.start();
    DataContainer2d heapSortdate = function.heapSort(data, 4);
    cout << "Heap Sort ";
    timer.end();

    timer.start();
    DataContainer2d heapSortCustomer = function.heapSort(data2, 0);
    timer.end();

    timer.start();
    DataContainer2d uniqueCount = function.repeatingItem(heapSortCustomer,0);
    cout << "Counting unique dates ";
    timer.end();

    function.displayTabulatedData(uniqueCount);


    return 0;

}
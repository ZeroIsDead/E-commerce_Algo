#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

bool invalidLine(const string& line) {
    stringstream ss(line);
    string cell;

    while (getline(ss, cell, ',')) {
        if (cell.empty() || cell == "NaN" || cell == "Invalid Date" || cell == "Invalid Rating") {
            return true;
        }
    }

    // Edge case: line ends with a comma (implies empty last cell)
    if (!line.empty() && line.back() == ',') {
        return true;
    }

    return false;
}

void cleanCSV(const char* inputPath, const char* outputPath) {
    ifstream inFile(inputPath);
    ofstream outFile(outputPath);

    if (!inFile.is_open()) {
        cerr << "Failed to open input file: " << inputPath << endl;
        return;
    }

    if (!outFile.is_open()) {
        cerr << "Failed to open output file: " << outputPath << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        if (!invalidLine(line)) {
            outFile << line << "\n";
        }
    }

    inFile.close();
    outFile.close();
}

int main() {
    cleanCSV("../data/transactions.csv", "../data/transactions_cleaned.csv");
    cleanCSV("../data/reviews.csv","../data/reviews_cleaned.csv");

    cout << "Cleaned files generated successfully." << endl;
    return 0;
}

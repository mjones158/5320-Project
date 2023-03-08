#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Function: Read the data line by line, finding the max number of bytes and storing it in currentNum.
// Compare currentNum and currentMax.
// Replace the value of currentMax with currentNum if currentNum is greater.
// Iterate through the data repeating the steps.
// Output the value of currentMax after going through the data.
// Tested on CTU13_Attack_Traffic.csv from https://github.com/imfaisalmalik/CTU13-CSV-Dataset/blob/main/CTU13_Attack_Traffic.csv

void findMax(const string& filename) {
    ifstream file(filename);  // Open file stream
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line);  // Ignore first line containing category names

    int currentMax = 0;  // Initialize currentMax to zero
    while (getline(file, line)) {  // Loop through each line
        stringstream ss(line);
        string field;
        int count = 0;
        int currentNum = 0;
        while (getline(ss, field, ',')) {  // Loop through each field
            count++;
            if (count == 7) {  // Check if it's the 7th field "Fwd Pkt Len Max"
                try {
                    currentNum = stoi(field);  // Convert to integer
                }
                catch (...) {
                    cerr << "Failed to convert field to integer: " << field << endl;
                    //return;
                }
                break;
            }
        }

        if (currentNum > currentMax) {  // Check if currentNum is greater than currentMax
            currentMax = currentNum;  // Replace currentMax with currentNum
        }
    }

    cout << "The maximum value found in the file is: " << currentMax << " bytes" << endl;
}

int main(){
    //clock_t tStart = clock();
    findMax("CTU-13.csv");
    //printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

void featureSearch(int data[]);
int accuracy;

int main() {
    cout << "Welcome to Rachel's Feature Selection Algorithm." << endl;
    cout << "Type in the name of the file to test: ";
    string fileName;
    cin >> fileName;

    ifstream file{fileName};
    string line;
    vector<vector<double>> data;

    if(!file.is_open()) {
        cout << "Error opening file!" << endl;
        return -1;
    }

    while(getline(file, line)) {
        stringstream sstream(line);
        vector<double> row;
        string value;

        while(sstream >> value) {
            row.push_back(stod(value));
        }
        data.push_back(row);
    }

    file.close();



    return 0;
}

void featureSearch(int data[]) {
    //for(int i = 0; i < )
}

int accuracy() {
    return 0;
}
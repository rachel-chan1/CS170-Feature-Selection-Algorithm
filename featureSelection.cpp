#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

void featureSearch(vector<vector<double>> data);
int leaveOneOutCrossValidation();

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

    featureSearch(data);



    return 0;
}

void featureSearch(vector<vector<double>> data) {
    vector<int> currentSetOfFeatures(data[0].size() - 1);
    int featureToAddAtThisLevel = -1;

    int bestSoFarAccuracy = 0;
    int accuracy = 0;

    for(int i = 1; i < data[0].size(); ++i) {
        cout << "On the " << i << "th level of the search tree" << endl;
        bestSoFarAccuracy = 0;
        for(int j = 1; j < data[i].size(); ++j) {
            // check if current feature being considered has already been added
            if(find(currentSetOfFeatures.begin(), currentSetOfFeatures.end(), j) != currentSetOfFeatures.end()) {
                continue;
            }
            cout << "Considering adding the " << j << " feature" << endl;
            accuracy = leaveOneOutCrossValidation();

            if(accuracy > bestSoFarAccuracy) {
                bestSoFarAccuracy = accuracy;
                featureToAddAtThisLevel = j;
            }
        }
        currentSetOfFeatures[i] = featureToAddAtThisLevel;
        cout << "On level " << i << " I added feature " << featureToAddAtThisLevel << " to current set" << endl;
    }
}

int leaveOneOutCrossValidation() {
    return 0;
}
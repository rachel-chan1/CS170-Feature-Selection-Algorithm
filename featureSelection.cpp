#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm> // find
#include <cmath> //sqrt

using namespace std;

void featureSearch(vector<vector<double>>);
double leaveOneOutCrossValidation(vector<vector<double>>, vector<int>, int);
double euclideanDistance(vector<double>, vector<double>);

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
    //leaveOneOutCrossValidation(data);



    return 0;
}

// forward search
void featureSearch(vector<vector<double>> data) {
    vector<int> currentSetOfFeatures;
    int featureToAddAtThisLevel = -1;

    double bestSoFarAccuracy = 0;
    double accuracy = 0;

    for(int i = 1; i < data[0].size(); ++i) {
        cout << "On the " << i << "th level of the search tree" << endl;
        bestSoFarAccuracy = 0;
        for(int j = 1; j < data[i].size(); ++j) {
            // check if current feature being considered has already been added
            if(find(currentSetOfFeatures.begin(), currentSetOfFeatures.end(), j) != currentSetOfFeatures.end()) {
                continue;
            }
            cout << "Considering adding the " << j << " feature" << endl;
            accuracy = leaveOneOutCrossValidation(data, currentSetOfFeatures, j);
            //cout << "ACCCCCCCCCUUUUUURRRRRAAAACCCYYYYYYYY " << accuracy << endl;

            if(accuracy > bestSoFarAccuracy) {
                bestSoFarAccuracy = accuracy;
                featureToAddAtThisLevel = j;
            }
        }
        currentSetOfFeatures.push_back(featureToAddAtThisLevel);
        cout << "On level " << i << " I added feature " << featureToAddAtThisLevel << " to current set" << endl;
    }
}

double leaveOneOutCrossValidation(vector<vector<double>> data, vector<int> currentSet, int featureToAdd) {
    vector<vector<double>> tempData;
    for(int i = 0; i < data.size(); ++i) {
        vector<double> tempRow;
        tempRow.push_back(data[i][0]);
        for(int j = 0; j < currentSet.size(); ++j) {
            tempRow.push_back(data[i][currentSet[j]]);
        }
        tempRow.push_back(data[i][featureToAdd]);
        tempData.push_back(tempRow);
    }

    int numberCorrectlyClassified = 0; 
    for(int i = 0; i < tempData.size(); ++i) {
        vector<double> objectToClassify(tempData[i].begin() + 1, tempData[i].end());
        int objectToClassifyLabel = tempData[i][0];

        //cout << "Looping over i, at the " << i << " location" << endl;
        //cout << "The " << i << "th object is in class " << label << endl;

        double nearestNeighborDistance = INT_MAX;
        int nearestNeighborLocation = INT_MAX;
        int nearestNeighborLabel = INT_MAX;

        for(int j = 1; j < data.size(); ++j) {
            if(j != i) {
                //cout << "Ask if " << i << " is nearest neighbor with " << j << endl;
                vector<double> neighbor(data[j].begin() + 1, data[j].end());
                double distance = euclideanDistance(objectToClassify, neighbor);
                if(distance < nearestNeighborDistance) {
                    nearestNeighborDistance = distance;
                    nearestNeighborLocation = j;
                    nearestNeighborLabel = data[j][0];
                }
            }
        }
        if(objectToClassifyLabel == nearestNeighborLabel) {
            numberCorrectlyClassified++;
        }
        //cout << "Object " << i << " is in class " << objectToClassifyLabel << endl;
        //cout << "It's nearest neighbor is " << nearestNeighborLocation << " which is in class " << nearestNeighborLabel << endl;
    }
    //cout << "AAAAAAAAAAAAAAAAAAAA " << static_cast<double>(numberCorrectlyClassified) / data.size() << endl;
    return (double)(numberCorrectlyClassified) / data.size();
}

double euclideanDistance(vector<double> object, vector<double> neighbor) {
    double dist = 0;
    for(int i = 0; i < object.size(); ++i) {
        dist += pow((neighbor[i] - object[i]), 2);
    }
    return sqrt(dist);
}
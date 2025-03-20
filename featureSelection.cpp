#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm> // find
#include <cmath> //sqrt
#include <cfloat> // dbl_max

using namespace std;

void forwardSearch(vector<vector<double>>);
void backwardElimination(vector<vector<double>>);
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

    cout << "Type the number of the algorithm you want to run." << endl
         << "   1) Forward Selection" << endl
         << "   2) Backward Elimination" << endl;

    int alg = -1;
    cin >> alg;
    
    vector<int> allFeatures;
    for(int i = 1; i < data[0].size(); ++i) {
        allFeatures.push_back(i);
    }
    double accuracy = leaveOneOutCrossValidation(data, allFeatures, -1);

    cout << "This dataset has " << data[0].size()-1 << " features (not including the class attribute), with " << data.size() << " instances." << endl;
    cout << "Running nearest neighbor with all " << data[0].size()-1 << " features, using \"leaving-one-out\" evaluation, I get an accuracy of " << accuracy*100 << '%' << endl << endl;
    cout << "Beginning search." << endl;

    if(alg == 1) {
        forwardSearch(data);
    }
    else if(alg == 2) {
        backwardElimination(data);
    }

    return 0;
}

// forward search
void forwardSearch(vector<vector<double>> data) {
    vector<int> currentSetOfFeatures;
    vector<int> bestAccuracySubset;
    int featureToAddAtThisLevel = -1;

    double bestSoFarAccuracy = 0;
    double accuracy = 0;
    double bestOverallAccuracy = 0;

    for(int i = 1; i < data[0].size(); ++i) {
        cout << "On the " << i << "th level of the search tree" << endl;
        bestSoFarAccuracy = 0;
        for(int j = 1; j < data[0].size(); ++j) {
            // check if current feature being considered has already been added
            if(find(currentSetOfFeatures.begin(), currentSetOfFeatures.end(), j) != currentSetOfFeatures.end()) {
                continue;
            }
            //cout << "Considering adding the " << j << " feature" << endl;
            accuracy = leaveOneOutCrossValidation(data, currentSetOfFeatures, j);
            if(currentSetOfFeatures.size() == 0) {
                cout << "   Using feature(s) {" << j << "} accuracy is " << accuracy*100 << '%' << endl;
            }
            else {
                cout << "   Using feature(s) {" << j << ", ";
                for(int i = 0; i < currentSetOfFeatures.size(); ++i) {
                    if(i == currentSetOfFeatures.size()-1) {
                        cout << currentSetOfFeatures[i];
                    }
                    else {
                        cout << currentSetOfFeatures[i] << ", ";
                    }
                }
                cout << "} accuracy is " << accuracy*100 << '%' << endl;
            }

            if(accuracy > bestSoFarAccuracy) {
                bestSoFarAccuracy = accuracy;
                featureToAddAtThisLevel = j;
            }
        }
        currentSetOfFeatures.push_back(featureToAddAtThisLevel);
        sort(currentSetOfFeatures.begin(), currentSetOfFeatures.end());
        if(bestSoFarAccuracy > bestOverallAccuracy) {
            bestOverallAccuracy = bestSoFarAccuracy;
            bestAccuracySubset = currentSetOfFeatures;
        }
        
        cout << "Feature set {";
        for(int i = 0; i < currentSetOfFeatures.size(); ++i) {
            if(i == currentSetOfFeatures.size()-1) {
                cout << currentSetOfFeatures[i];
            }
            else {
                cout << currentSetOfFeatures[i] << ", ";
            }
        }
        cout << "} was best, accuracy is " << bestSoFarAccuracy*100 << '%' << endl;
        //cout << "On level " << i << " I added feature " << featureToAddAtThisLevel << " to current set" << endl;
    }
    cout << "Finished search!! The best feature subset is {";
    for(int i = 0; i < bestAccuracySubset.size(); ++i) {
        if(i == bestAccuracySubset.size()-1) {
            cout << bestAccuracySubset[i];
        }
        else {
            cout << bestAccuracySubset[i] << ", ";
        }
    }
    cout << "}, which has an accuracy of " << bestOverallAccuracy*100 << '%' << endl;
}

// backward elimination
void backwardElimination(vector<vector<double>> data) {
    vector<int> currentSetOfFeatures;
    vector<int> bestAccuracySubset;
    int featureToRemoveAtThisLevel = -1;

    double bestSoFarAccuracy = 0;
    double accuracy = 0;
    double bestOverallAccuracy = 0;

    for(int i = 1; i < data[0].size(); ++i) {
        currentSetOfFeatures.push_back(i);
    }

    cout << "On the 1th level of the search tree" << endl;
    accuracy = leaveOneOutCrossValidation(data, currentSetOfFeatures, -1);
    cout << "   Using feature(s) {";
    for(int i = 0; i < currentSetOfFeatures.size(); ++i) {
        if(i == currentSetOfFeatures.size()-1) {
            cout << currentSetOfFeatures[i];
        }
        else {
            cout << currentSetOfFeatures[i] << ", ";
        }
    }
    cout << "} accuracy is " << accuracy*100 << '%' << endl;

    int i = 2;
    while(currentSetOfFeatures.size() > 1) {
        cout << "On the " << i << "th level of the search tree" << endl;
        bestSoFarAccuracy = 0;

        for(int j = 1; j < data[i].size(); ++j) {
            // check if feature being considered for removal is in current set of features
            if(find(currentSetOfFeatures.begin(), currentSetOfFeatures.end(), j) == currentSetOfFeatures.end()) {
                continue;
            }

            accuracy = leaveOneOutCrossValidation(data, currentSetOfFeatures, j);
            if(currentSetOfFeatures.size() == 1) {
                cout << "   Using feature(s) {" << currentSetOfFeatures[0] << "} accuracy is " << accuracy*100 << '%' << endl;
            }
            else {
                cout << "   Using feature(s) {";
                for(int i = 0; i < currentSetOfFeatures.size(); ++i) {
                    if(currentSetOfFeatures[i] != j) {
                        if(j == currentSetOfFeatures[currentSetOfFeatures.size()-1] && i == currentSetOfFeatures.size()-2) {
                            cout << currentSetOfFeatures[i];
                        }
                        else if(i == currentSetOfFeatures.size()-1) {
                            cout << currentSetOfFeatures[i];
                        }
                        else {
                            cout << currentSetOfFeatures[i] << ", ";
                        }
                    }
                }
                cout << "} accuracy is " << accuracy*100 << '%' << endl;
            }

            if(accuracy > bestSoFarAccuracy) {
                bestSoFarAccuracy = accuracy;
                featureToRemoveAtThisLevel = j;
            }
        }

        currentSetOfFeatures.erase(find(currentSetOfFeatures.begin(), currentSetOfFeatures.end(), featureToRemoveAtThisLevel));
        if(bestSoFarAccuracy > bestOverallAccuracy) {
            bestOverallAccuracy = bestSoFarAccuracy;
            bestAccuracySubset = currentSetOfFeatures;
        }
        cout << "Feature set {";
        for(int i = 0; i < currentSetOfFeatures.size(); ++i) {
            if(i == currentSetOfFeatures.size()-1) {
                cout << currentSetOfFeatures[i];
            }
            else {
                cout << currentSetOfFeatures[i] << ", ";
            }
        }
        cout << "} was best, accuracy is " << bestSoFarAccuracy*100 << '%' << endl;
        i++;
    }
    cout << "Finished search!! The best feature subset is {";
    for(int i = 0; i < bestAccuracySubset.size(); ++i) {
        if(i == bestAccuracySubset.size()-1) {
            cout << bestAccuracySubset[i];
        }
        else {
            cout << bestAccuracySubset[i] << ", ";
        }
    }
    cout << "}, which has an accuracy of " << bestOverallAccuracy*100 << '%' << endl;
}



double leaveOneOutCrossValidation(vector<vector<double>> data, vector<int> currentSet, int featureToAdd) {
    vector<vector<double>> tempData;
    //cout << "CURRENT SET SIZE " << currentSet.size() << endl;
    if(featureToAdd > 0 && find(currentSet.begin(), currentSet.end(), featureToAdd) == currentSet.end()) {      // featureToAdd is not in current set and is a valid feature
        currentSet.push_back(featureToAdd);
        sort(currentSet.begin(), currentSet.end());
        for(int i = 0; i < data.size(); ++i) {
            vector<double> tempRow;
            tempRow.push_back(data[i][0]);
            for(int j = 0; j < currentSet.size(); ++j) {
                tempRow.push_back(data[i][currentSet[j]]);
            }
            tempData.push_back(tempRow);
        }
    }
    else if (find(currentSet.begin(), currentSet.end(), featureToAdd) != currentSet.end()) {    // feature is in current set meaning backward elimination
        for(int i = 0; i < data.size(); ++i) {
            vector<double> tempRow;
            tempRow.push_back(data[i][0]);  // class label
            for(int j = 0; j < currentSet.size(); ++j) {
                if(currentSet[j] != featureToAdd) {     // only add features that are not the one being eliminated
                    tempRow.push_back(data[i][currentSet[j]]);
                }
            }
            tempData.push_back(tempRow);
        }
    }
    else {      // full set of features
        tempData = data;
    }

    int numberCorrectlyClassified = 0; 
    for(int i = 0; i < tempData.size(); ++i) {
        vector<double> objectToClassify(tempData[i].begin() + 1, tempData[i].end());
        int objectToClassifyLabel = tempData[i][0];

        //cout << "Looping over i, at the " << i+1 << " location" << endl;
        //cout << "The " << i << "th object is in class " << objectToClassifyLabel << endl;

        double nearestNeighborDistance = DBL_MAX;
        int nearestNeighborLocation = INT_MAX;
        int nearestNeighborLabel = INT_MAX;

        for(int j = 0; j < tempData.size(); ++j) {
            if(j != i) {
                //cout << "Ask if " << i << " is nearest neighbor with " << j << endl;
                vector<double> neighbor(tempData[j].begin() + 1, tempData[j].end());
                double distance = euclideanDistance(objectToClassify, neighbor);
                if(distance < nearestNeighborDistance) {
                    nearestNeighborDistance = distance;
                    nearestNeighborLocation = j;
                    nearestNeighborLabel = tempData[j][0];
                }
            }
        }
        if(objectToClassifyLabel == nearestNeighborLabel) {
            numberCorrectlyClassified++;
        }
    }
    return (double)(numberCorrectlyClassified) / data.size();
}

double euclideanDistance(vector<double> object, vector<double> neighbor) {
    double dist = 0;
    for(int i = 0; i < object.size(); ++i) {
        dist += pow((neighbor[i] - object[i]), 2);
    }
    return sqrt(dist);
}
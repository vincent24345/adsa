#include <iostream>
#include <vector>
#include <climits>
#include <sstream>
using namespace std;

// Allowed character set for cost calculation
string allowedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Cost calculator for character-based input
int calculateCharCost(char ch) {
    return allowedChars.find(ch);
}

// Build the matrix of construction and destruction costs
vector<vector<int>> buildCostMatrix(const vector<vector<int>>& roadStatus, const vector<vector<char>>& buildCharCosts, const vector<vector<char>>& destroyCharCosts) {
    int matrixSize = roadStatus.size();
    vector<vector<int>> costMatrix(matrixSize, vector<int>(matrixSize, 0));

    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            if (roadStatus[i][j] == 0) { // No existing road, construction required
                costMatrix[i][j] = calculateCharCost(buildCharCosts[i][j]);
            } else { // Existing road, consider destruction cost
                costMatrix[i][j] = -calculateCharCost(destroyCharCosts[i][j]);
            }
        }
    }
    return costMatrix;
}

// Disjoint set data for union-find structure in MST construction
vector<int> setRoot(100), setRank(100);

// Retrieve root of the set in union-find structure
int findSetRoot(int node) {
    if (setRoot[node] != node) {
        setRoot[node] = findSetRoot(setRoot[node]);
    }
    return setRoot[node];
}

// Union by rank function to combine disjoint sets
void combineSets(int nodeA, int nodeB) {
    int rootA = findSetRoot(nodeA);
    int rootB = findSetRoot(nodeB);
    if (rootA != rootB) {
        if (setRank[rootA] > setRank[rootB]) {
            setRoot[rootB] = rootA;
        } else if (setRank[rootA] < setRank[rootB]) {
            setRoot[rootA] = rootB;
        } else {
            setRoot[rootB] = rootA;
            setRank[rootA]++;
        }
    }
}

// Kruskal's algorithm to find Minimum Spanning Tree for cost minimization
vector<vector<int>> constructMST(const vector<vector<int>>& edgeCosts, int nodeCount) {
    vector<vector<int>> mstEdges(nodeCount, vector<int>(nodeCount, 0));

    // Initialize the disjoint sets for MST construction
    for (int i = 0; i < nodeCount; i++) {
        setRoot[i] = i;
    }

    int edgesAdded = 0;
    while (edgesAdded < nodeCount - 1) {
        int minEdgeCost = INT_MAX;
        int fromNode = -1, toNode = -1;

        for (int i = 0; i < nodeCount; i++) {
            for (int j = 0; j < nodeCount; j++) {
                if (findSetRoot(i) != findSetRoot(j) && edgeCosts[i][j] < minEdgeCost) {
                    minEdgeCost = edgeCosts[i][j];
                    fromNode = i;
                    toNode = j;
                }
            }
        }

        if (fromNode != -1 && toNode != -1) {
            combineSets(fromNode, toNode);
            edgesAdded++;
            mstEdges[fromNode][toNode] = mstEdges[toNode][fromNode] = 1;
        }
    }
    return mstEdges;
}

// Compute the total minimized cost of the new road network configuration
int computeTotalCost(const vector<vector<int>>& initialRoads, const vector<vector<int>>& mstEdges, const vector<vector<int>>& roadCostMatrix) {
    int totalCost = 0;
    int roadMatrixSize = initialRoads.size();

    for (int i = 0; i < roadMatrixSize; i++) {
        for (int j = 0; j < roadMatrixSize; j++) {
            if (initialRoads[i][j] == 0 && mstEdges[i][j] == 1) {
                totalCost += roadCostMatrix[i][j];
            } else if (initialRoads[i][j] == 1 && mstEdges[i][j] == 0) {
                totalCost += abs(roadCostMatrix[i][j]);
            }
        }
    }
    return totalCost / 2; // Accounts for bidirectional nature of roads
}

// Convert comma-separated integer string to matrix
vector<vector<int>> parseIntegerMatrix(const string& input) {
    vector<vector<int>> matrix;
    vector<int> currentRow;
    for (char ch : input) {
        if (ch == ',') {
            matrix.push_back(currentRow);
            currentRow.clear();
        } else {
            currentRow.push_back(ch - '0');
        }
    }
    matrix.push_back(currentRow);
    return matrix;
}

// Convert comma-separated character string to matrix
vector<vector<char>> parseCharacterMatrix(const string& input) {
    vector<vector<char>> matrix;
    vector<char> currentRow;
    for (char ch : input) {
        if (ch == ',') {
            matrix.push_back(currentRow);
            currentRow.clear();
        } else {
            currentRow.push_back(ch);
        }
    }
    matrix.push_back(currentRow);
    return matrix;
}

int main() {
    string lineInput;
    getline(cin, lineInput);

    istringstream inputStream(lineInput);
    vector<string> parsedData;
    string segment;
    while (getline(inputStream, segment, ' ')) {
        parsedData.push_back(segment);
    }

    vector<vector<int>> currentRoadMatrix = parseIntegerMatrix(parsedData[0]);
    vector<vector<char>> buildCostChars = parseCharacterMatrix(parsedData[1]);
    vector<vector<char>> destroyCostChars = parseCharacterMatrix(parsedData[2]);

    vector<vector<int>> edgeCostMatrix = buildCostMatrix(currentRoadMatrix, buildCostChars, destroyCostChars);
    vector<vector<int>> mstResult = constructMST(edgeCostMatrix, currentRoadMatrix.size());

    int finalMinCost = computeTotalCost(currentRoadMatrix, mstResult, edgeCostMatrix);
    cout << finalMinCost << endl;
    return 0;
}

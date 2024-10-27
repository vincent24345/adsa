#include <iostream>
#include <vector>
#include <climits>
#include <sstream>
using namespace std;

// Allowed characters for cost calculation
string validCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Cost function to get the index of the character
int getCharacterCost(char character) {
    return validCharacters.find(character);
}

// Create a cost matrix based on road status and character costs
vector<vector<int>> createCostMatrix(const vector<vector<int>>& roadStatus, const vector<vector<char>>& buildCosts, const vector<vector<char>>& destroyCosts) {
    int size = roadStatus.size();
    vector<vector<int>> costMatrix(size, vector<int>(size, 0));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (roadStatus[i][j] == 0) { // No road exists, need to build
                costMatrix[i][j] = getCharacterCost(buildCosts[i][j]);
            } else { // Road exists, consider destruction
                costMatrix[i][j] = -getCharacterCost(destroyCosts[i][j]);
            }
        }
    }
    return costMatrix;
}

// Disjoint set data for union-find structure in MST construction
vector<int> parentSet(100), rankSet(100);

// Find the root of the set in the union-find structure
int findRoot(int node) {
    if (parentSet[node] != node) {
        parentSet[node] = findRoot(parentSet[node]);
    }
    return parentSet[node];
}

// Union by rank to merge two sets
void unionSets(int nodeA, int nodeB) {
    int rootA = findRoot(nodeA);
    int rootB = findRoot(nodeB);
    if (rootA != rootB) {
        if (rankSet[rootA] > rankSet[rootB]) {
            parentSet[rootB] = rootA;
        } else if (rankSet[rootA] < rankSet[rootB]) {
            parentSet[rootA] = rootB;
        } else {
            parentSet[rootB] = rootA;
            rankSet[rootA]++;
        }
    }
}

// Kruskal's algorithm to construct Minimum Spanning Tree (MST)
vector<vector<int>> kruskalMST(const vector<vector<int>>& edgeCosts, int totalNodes) {
    vector<vector<int>> mstEdges(totalNodes, vector<int>(totalNodes, 0));

    // Initialize the disjoint sets for MST
    for (int i = 0; i < totalNodes; i++) {
        parentSet[i] = i;
    }

    int edgesCounted = 0;
    while (edgesCounted < totalNodes - 1) {
        int minCost = INT_MAX;
        int fromNode = -1, toNode = -1;

        for (int i = 0; i < totalNodes; i++) {
            for (int j = 0; j < totalNodes; j++) {
                if (findRoot(i) != findRoot(j) && edgeCosts[i][j] < minCost) {
                    minCost = edgeCosts[i][j];
                    fromNode = i;
                    toNode = j;
                }
            }
        }

        if (fromNode != -1 && toNode != -1) {
            unionSets(fromNode, toNode);
            edgesCounted++;
            mstEdges[fromNode][toNode] = mstEdges[toNode][fromNode] = 1;
        }
    }
    return mstEdges;
}

// Calculate total cost of the new road configuration
int calculateTotalCost(const vector<vector<int>>& existingRoads, const vector<vector<int>>& mstEdges, const vector<vector<int>>& costMatrix) {
    int totalCost = 0;
    int size = existingRoads.size();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (existingRoads[i][j] == 0 && mstEdges[i][j] == 1) {
                totalCost += costMatrix[i][j];
            } else if (existingRoads[i][j] == 1 && mstEdges[i][j] == 0) {
                totalCost += abs(costMatrix[i][j]);
            }
        }
    }
    return totalCost / 2; // Adjust for bidirectionality
}

// Parse a comma-separated integer string into a matrix
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

// Parse a comma-separated character string into a matrix
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
    string inputLine;
    getline(cin, inputLine);

    istringstream stream(inputLine);
    vector<string> segments;
    string segment;
    while (getline(stream, segment, ' ')) {
        segments.push_back(segment);
    }

    vector<vector<int>> currentRoads = parseIntegerMatrix(segments[0]);
    vector<vector<char>> buildCosts = parseCharacterMatrix(segments[1]);
    vector<vector<char>> destroyCosts = parseCharacterMatrix(segments[2]);

    vector<vector<int>> edgeCosts = createCostMatrix(currentRoads, buildCosts, destroyCosts);
    vector<vector<int>> mst = kruskalMST(edgeCosts, currentRoads.size());

    int minimumCost = calculateTotalCost(currentRoads, mst, edgeCosts);
    cout << minimumCost << endl;
    return 0;
}

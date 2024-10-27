#include <iostream>
#include <vector>
#include <limits.h>
#include <sstream>
using namespace std;

// List of valid characters
string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Cost calculation for each character
int calcCost(char c) {
    return charset.find(c);
}

// Build the cost matrix
vector<vector<int>> constructCostMatrix(const vector<vector<int>>& currentRoads, const vector<vector<char>>& buildCost, const vector<vector<char>>& destroyCost) {
    int n = currentRoads.size();
    vector<vector<int>> costMatrix(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (currentRoads[i][j] == 0) { // Road needs to be built
                costMatrix[i][j] = calcCost(buildCost[i][j]);
            } else { // Existing road, consider destroy cost
                costMatrix[i][j] = -calcCost(destroyCost[i][j]);
            }
        }
    }
    return costMatrix;
}

// Disjoint set for Kruskal's algorithm
vector<int> root(100), rankSet(100);

// Find the root of a set
int findRoot(int node) {
    if (root[node] != node) {
        root[node] = findRoot(root[node]);
    }
    return root[node];
}

// Union by rank
void unionSets(int u, int v) {
    int rootU = findRoot(u);
    int rootV = findRoot(v);
    if (rootU != rootV) {
        if (rankSet[rootU] > rankSet[rootV]) {
            root[rootV] = rootU;
        } else if (rankSet[rootU] < rankSet[rootV]) {
            root[rootU] = rootV;
        } else {
            root[rootV] = rootU;
            rankSet[rootU]++;
        }
    }
}

// Kruskal's algorithm to find MST
vector<vector<int>> applyKruskal(const vector<vector<int>>& costs, int totalCities) {
    vector<vector<int>> mstMatrix(totalCities, vector<int>(totalCities, 0));
    int mstCost = 0;

    for (int i = 0; i < totalCities; i++) {
        root[i] = i;
    }

    int edgeCount = 0;
    while (edgeCount < totalCities - 1) {
        int minCost = INT_MAX;
        int u = -1, v = -1;
        for (int i = 0; i < totalCities; i++) {
            for (int j = 0; j < totalCities; j++) {
                if (findRoot(i) != findRoot(j) && costs[i][j] < minCost) {
                    minCost = costs[i][j];
                    u = i;
                    v = j;
                }
            }
        }
        if (u != -1 && v != -1) {
            unionSets(u, v);
            edgeCount++;
            mstCost += minCost;
            mstMatrix[u][v] = mstMatrix[v][u] = 1;
        }
    }
    return mstMatrix;
}

// Calculate total minimum cost after MST
int calculateMinCost(const vector<vector<int>>& currentRoads, const vector<vector<int>>& mst, const vector<vector<int>>& costMatrix) {
    int totalCost = 0;
    int cities = currentRoads.size();

    for (int i = 0; i < cities; i++) {
        for (int j = 0; j < cities; j++) {
            if (currentRoads[i][j] == 0 && mst[i][j] == 1) {
                totalCost += costMatrix[i][j];
            } else if (currentRoads[i][j] == 1 && mst[i][j] == 0) {
                totalCost += abs(costMatrix[i][j]);
            }
        }
    }
    return totalCost / 2; // Divided by 2 to account for bidirectional roads
}

// Helper function to parse a comma-separated string into an integer matrix
vector<vector<int>> parseIntMatrix(const string& str) {
    vector<vector<int>> matrix;
    vector<int> row;
    for (char ch : str) {
        if (ch == ',') {
            matrix.push_back(row);
            row.clear();
        } else {
            row.push_back(ch - '0');
        }
    }
    matrix.push_back(row);
    return matrix;
}

// Helper function to parse a comma-separated string into a character matrix
vector<vector<char>> parseCharMatrix(const string& str) {
    vector<vector<char>> matrix;
    vector<char> row;
    for (char ch : str) {
        if (ch == ',') {
            matrix.push_back(row);
            row.clear();
        } else {
            row.push_back(ch);
        }
    }
    matrix.push_back(row);
    return matrix;
}

int main() {
    string input;
    getline(cin, input);

    istringstream stream(input);
    vector<string> data;
    string token;
    while (getline(stream, token, ' ')) {
        data.push_back(token);
    }

    vector<vector<int>> currentRoads = parseIntMatrix(data[0]);
    vector<vector<char>> buildCost = parseCharMatrix(data[1]);
    vector<vector<char>> destroyCost = parseCharMatrix(data[2]);

    vector<vector<int>> costMatrix = constructCostMatrix(currentRoads, buildCost, destroyCost);
    vector<vector<int>> mst = applyKruskal(costMatrix, currentRoads.size());

    int minCost = calculateMinCost(currentRoads, mst, costMatrix);
    cout << minCost << endl;
    return 0;
}

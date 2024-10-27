#include <iostream>
#include <vector>
#include <climits>
#include <sstream>
#include <algorithm>
using namespace std;

// Allowed characters for cost calculation
const string characterSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Calculate the cost for building or destroying a road based on the character
int getCharacterCost(char c) {
    return characterSet.find(c);
}

// Construct a list of edges with their costs
vector<pair<int, pair<int, int>>> buildEdgeList(const vector<vector<int>>& currentRoads, 
                                                 const vector<vector<char>>& buildCosts, 
                                                 const vector<vector<char>>& destroyCosts) {
    vector<pair<int, pair<int, int>>> edges; // {cost, {node1, node2}}
    int n = currentRoads.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                int cost = 0;
                if (currentRoads[i][j] == 0) { // No road exists, need to build
                    cost = getCharacterCost(buildCosts[i][j]);
                } else { // A road exists, can destroy it
                    cost = -getCharacterCost(destroyCosts[i][j]);
                }
                edges.push_back({cost, {i, j}});
            }
        }
    }
    return edges;
}

// Disjoint set union-find data structure
vector<int> parent, rankSet; // Renamed rank to rankSet

// Find the root of the set containing node
int findParent(int node) {
    if (parent[node] != node) {
        parent[node] = findParent(parent[node]);
    }
    return parent[node];
}

// Union operation for two sets
void unionSets(int nodeA, int nodeB) {
    int rootA = findParent(nodeA);
    int rootB = findParent(nodeB);
    
    if (rootA != rootB) {
        if (rankSet[rootA] < rankSet[rootB]) {
            parent[rootA] = rootB;
        } else if (rankSet[rootA] > rankSet[rootB]) {
            parent[rootB] = rootA;
        } else {
            parent[rootB] = rootA;
            rankSet[rootA]++;
        }
    }
}

// Kruskal's algorithm implementation to find Minimum Spanning Tree (MST)
vector<pair<int, pair<int, int>>> kruskalMST(vector<pair<int, pair<int, int>>>& edges, int nodeCount) {
    vector<pair<int, pair<int, int>>> mst; // Store edges in MST
    sort(edges.begin(), edges.end()); // Sort edges by cost

    parent.resize(nodeCount);
    rankSet.resize(nodeCount, 0); // Changed rank to rankSet
    for (int i = 0; i < nodeCount; i++) {
        parent[i] = i;
    }

    for (const auto& edge : edges) {
        int cost = edge.first;
        int u = edge.second.first;
        int v = edge.second.second;

        if (findParent(u) != findParent(v)) {
            mst.push_back(edge); // Include edge in MST
            unionSets(u, v); // Union the sets
        }
    }
    return mst;
}

// Calculate the total minimum cost of the roads
int calculateTotalCost(const vector<vector<int>>& currentRoads, 
                       const vector<pair<int, pair<int, int>>>& mst, 
                       const vector<vector<char>>& buildCosts, 
                       const vector<vector<char>>& destroyCosts) {
    int totalCost = 0;

    // Calculate the cost of the MST and adjustments
    for (const auto& edge : mst) {
        int cost = edge.first; // This variable was previously unused but is now necessary
        int u = edge.second.first;
        int v = edge.second.second;

        if (currentRoads[u][v] == 0) { // If building a new road
            totalCost += getCharacterCost(buildCosts[u][v]);
        } else { // If an existing road is being destroyed
            totalCost += -getCharacterCost(destroyCosts[u][v]);
        }
    }
    return totalCost;
}

// Parse a comma-separated string into a matrix of integers
vector<vector<int>> parseIntegerMatrix(const string& input) {
    vector<vector<int>> matrix;
    vector<int> row;
    stringstream ss(input);
    string token;
    
    while (getline(ss, token, ';')) {
        row.clear();
        stringstream rowStream(token);
        while (getline(rowStream, token, ',')) {
            row.push_back(stoi(token));
        }
        matrix.push_back(row);
    }
    return matrix;
}

// Parse a comma-separated string into a matrix of characters
vector<vector<char>> parseCharacterMatrix(const string& input) {
    vector<vector<char>> matrix;
    vector<char> row;
    stringstream ss(input);
    string token;

    while (getline(ss, token, ';')) {
        row.clear();
        for (char ch : token) {
            if (ch != ',') {
                row.push_back(ch);
            }
        }
        matrix.push_back(row);
    }
    return matrix;
}

int main() {
    string inputLine;
    getline(cin, inputLine);

    istringstream stream(inputLine);
    vector<string> inputData;
    string segment;
    while (getline(stream, segment, ' ')) {
        inputData.push_back(segment);
    }

    vector<vector<int>> currentRoads = parseIntegerMatrix(inputData[0]);
    vector<vector<char>> buildCosts = parseCharacterMatrix(inputData[1]);
    vector<vector<char>> destroyCosts = parseCharacterMatrix(inputData[2]);

    vector<pair<int, pair<int, int>>> edges = buildEdgeList(currentRoads, buildCosts, destroyCosts);
    vector<pair<int, pair<int, int>>> mstEdges = kruskalMST(edges, currentRoads.size());

    int minimumCost = calculateTotalCost(currentRoads, mstEdges, buildCosts, destroyCosts);
    cout << minimumCost << endl;

    return 0;
}

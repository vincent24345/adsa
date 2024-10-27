#include <iostream>
#include <vector>
#include <sstream>
#include <queue>
#include <utility>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Allowed characters for cost mapping
const string validCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Function to get the index of the character for cost calculation
int getCharIndex(char character) {
    return validCharacters.find(character);
}

// Class representing the union-find data structure
class UnionFind {
public:
    UnionFind(int size) {
        parent.resize(size);
        rank.resize(size, 0);
        for (int i = 0; i < size; ++i) {
            parent[i] = i;
        }
    }

    int find(int node) {
        if (node != parent[node]) {
            parent[node] = find(parent[node]); // Path compression
        }
        return parent[node];
    }

    void unionSets(int a, int b) {
        int rootA = find(a);
        int rootB = find(b);
        if (rootA != rootB) {
            if (rank[rootA] > rank[rootB]) {
                parent[rootB] = rootA;
            } else {
                parent[rootA] = rootB;
                if (rank[rootA] == rank[rootB]) {
                    rank[rootB]++;
                }
            }
        }
    }

private:
    vector<int> parent;
    vector<int> rank;
};

// Function to create a cost matrix based on road statuses and costs
vector<vector<int>> generateCostMatrix(const vector<vector<int>>& roads, 
                                        const vector<vector<char>>& buildCosts, 
                                        const vector<vector<char>>& destroyCosts) {
    int n = roads.size();
    vector<vector<int>> costMatrix(n, vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (roads[i][j] == 0) { // No road exists, cost to build
                costMatrix[i][j] = getCharIndex(buildCosts[i][j]);
            } else { // Road exists, cost to destroy
                costMatrix[i][j] = -getCharIndex(destroyCosts[i][j]);
            }
        }
    }
    return costMatrix;
}

// Function to find the Minimum Spanning Tree (MST) using Kruskal's algorithm
vector<vector<int>> findMST(const vector<vector<int>>& costs, int nodeCount) {
    vector<vector<int>> mst(nodeCount, vector<int>(nodeCount, 0));
    UnionFind uf(nodeCount);
    using Edge = pair<int, pair<int, int>>; // (cost, (from, to))
    priority_queue<Edge, vector<Edge>, greater<Edge>> edgeQueue;

    // Add all edges to the priority queue
    for (int i = 0; i < nodeCount; ++i) {
        for (int j = 0; j < nodeCount; ++j) {
            if (costs[i][j] != 0) {
                edgeQueue.push({costs[i][j], {i, j}});
            }
        }
    }

    // Process edges in order of cost
    while (!edgeQueue.empty()) {
        auto edge = edgeQueue.top();
        edgeQueue.pop();
        int cost = edge.first;
        int u = edge.second.first;
        int v = edge.second.second;

        if (uf.find(u) != uf.find(v)) {
            uf.unionSets(u, v);
            mst[u][v] = mst[v][u] = 1; // Mark the edge in MST
        }
    }
    return mst;
}

// Function to compute the total cost based on the existing roads and the MST
int computeTotalCost(const vector<vector<int>>& existingRoads, 
                     const vector<vector<int>>& mst, 
                     const vector<vector<int>>& costs) {
    int totalCost = 0;
    int n = existingRoads.size();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (existingRoads[i][j] == 0 && mst[i][j] == 1) {
                totalCost += costs[i][j]; // Cost to build new roads
            } else if (existingRoads[i][j] == 1 && mst[i][j] == 0) {
                totalCost += abs(costs[i][j]); // Cost to destroy existing roads
            }
        }
    }
    return totalCost / 2; // Adjust for bidirectionality
}

// Function to parse a comma-separated string of integers into a 2D matrix
vector<vector<int>> parseIntegerMatrix(const string& input) {
    vector<vector<int>> matrix;
    vector<int> currentRow;
    stringstream ss(input);
    string item;
    
    while (getline(ss, item, ',')) {
        currentRow.push_back(item[0] - '0');
        if (item.back() == ';') {
            matrix.push_back(currentRow);
            currentRow.clear();
        }
    }
    return matrix;
}

// Function to parse a comma-separated string of characters into a 2D matrix
vector<vector<char>> parseCharacterMatrix(const string& input) {
    vector<vector<char>> matrix;
    vector<char> currentRow;
    stringstream ss(input);
    string item;
    
    while (getline(ss, item, ',')) {
        currentRow.push_back(item[0]);
        if (item.back() == ';') {
            matrix.push_back(currentRow);
            currentRow.clear();
        }
    }
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

    vector<vector<int>> edgeCosts = generateCostMatrix(currentRoads, buildCosts, destroyCosts);
    vector<vector<int>> mst = findMST(edgeCosts, currentRoads.size());

    int totalMinimumCost = computeTotalCost(currentRoads, mst, edgeCosts);
    cout << totalMinimumCost << endl;

    return 0;
}

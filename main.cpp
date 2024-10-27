#include <iostream>
#include <vector>
#include <climits>
#include <sstream>
#include <utility> // for std::pair

using namespace std;

// Allowed characters for cost mapping
const string validChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Function to calculate the index of a character for cost
int getCharIndex(char character) {
    return validChars.find(character);
}

// Create a cost matrix based on road presence and costs
vector<vector<int>> buildCostMatrix(const vector<vector<int>>& roads, 
                                     const vector<vector<char>>& buildCosts, 
                                     const vector<vector<char>>& destroyCosts) {
    int n = roads.size();
    vector<vector<int>> costs(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (roads[i][j] == 0) { // Road needs to be built
                costs[i][j] = getCharIndex(buildCosts[i][j]);
            } else { // Road exists, consider destruction
                costs[i][j] = -getCharIndex(destroyCosts[i][j]);
            }
        }
    }
    return costs;
}

// Union-Find structure for Kruskal's algorithm
class UnionFind {
public:
    UnionFind(int size) {
        parent.resize(size);
        rank.resize(size, 0);
        for (int i = 0; i < size; i++) {
            parent[i] = i;
        }
    }

    int find(int node) {
        if (parent[node] != node) {
            parent[node] = find(parent[node]); // Path compression
        }
        return parent[node];
    }

    void unite(int nodeA, int nodeB) {
        int rootA = find(nodeA);
        int rootB = find(nodeB);
        if (rootA != rootB) {
            if (rank[rootA] > rank[rootB]) {
                parent[rootB] = rootA;
            } else if (rank[rootA] < rank[rootB]) {
                parent[rootA] = rootB;
            } else {
                parent[rootB] = rootA;
                rank[rootA]++;
            }
        }
    }

private:
    vector<int> parent;
    vector<int> rank;
};

// Kruskal's algorithm to construct the Minimum Spanning Tree (MST)
vector<vector<int>> kruskalMST(const vector<vector<int>>& costs, int nodeCount) {
    vector<vector<int>> mst(nodeCount, vector<int>(nodeCount, 0));
    UnionFind uf(nodeCount);
    vector<pair<int, pair<int, int>>> edges; // (cost, (start, end))

    for (int i = 0; i < nodeCount; i++) {
        for (int j = 0; j < nodeCount; j++) {
            if (costs[i][j] != 0) {
                edges.push_back({costs[i][j], {i, j}});
            }
        }
    }

    // Sort edges based on cost
    sort(edges.begin(), edges.end(), [](const pair<int, pair<int, int>>& a, const pair<int, pair<int, int>>& b) {
        return a.first < b.first;
    });

    for (const auto& edge : edges) {
        int cost = edge.first;
        int u = edge.second.first;
        int v = edge.second.second;
        if (uf.find(u) != uf.find(v)) {
            uf.unite(u, v);
            mst[u][v] = mst[v][u] = 1; // Mark the edge in MST
        }
    }
    return mst;
}

// Calculate the total cost of the road configuration
int calculateTotalRoadCost(const vector<vector<int>>& existingRoads, 
                            const vector<vector<int>>& mstEdges, 
                            const vector<vector<int>>& costs) {
    int totalCost = 0;
    int n = existingRoads.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (existingRoads[i][j] == 0 && mstEdges[i][j] == 1) {
                totalCost += costs[i][j]; // Cost for building new roads
            } else if (existingRoads[i][j] == 1 && mstEdges[i][j] == 0) {
                totalCost += abs(costs[i][j]); // Cost for destroying existing roads
            }
        }
    }
    return totalCost / 2; // Correct for double counting
}

// Function to parse a comma-separated string of integers into a matrix
vector<vector<int>> parseIntegerInput(const string& input) {
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

// Function to parse a comma-separated string of characters into a matrix
vector<vector<char>> parseCharacterInput(const string& input) {
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

    vector<vector<int>> currentRoads = parseIntegerInput(segments[0]);
    vector<vector<char>> buildCosts = parseCharacterInput(segments[1]);
    vector<vector<char>> destroyCosts = parseCharacterInput(segments[2]);

    vector<vector<int>> edgeCosts = buildCostMatrix(currentRoads, buildCosts, destroyCosts);
    vector<vector<int>> mst = kruskalMST(edgeCosts, currentRoads.size());

    int minimumCost = calculateTotalRoadCost(currentRoads, mst, edgeCosts);
    cout << minimumCost << endl;

    return 0;
}

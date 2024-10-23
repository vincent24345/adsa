#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <tuple>

using namespace std;

struct Edge {
    int u, v, cost;
};

// Disjoint Set Union (Union-Find) for Kruskal's algorithm
class UnionFind {
public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }

    int find(int u) {
        if (parent[u] != u)
            parent[u] = find(parent[u]);
        return parent[u];
    }

    void unite(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }

private:
    vector<int> parent, rank;
};

// Function to convert letter costs to numerical values
int letterToCost(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    return 0; // Default
}

int main() {
    string input;
    getline(cin, input);

    // Split the input into three parts
    stringstream ss(input);
    string countryStr, buildStr, destroyStr;
    getline(ss, countryStr, ' ');
    getline(ss, buildStr, ' ');
    getline(ss, destroyStr, ' ');

    // Parse the country matrix (adjacency matrix)
    vector<vector<int>> country;
    stringstream countryStream(countryStr);
    string line;
    while (getline(countryStream, line, ',')) {
        vector<int> row;
        for (char c : line) {
            row.push_back(c - '0');
        }
        country.push_back(row);
    }

    int n = country.size();
    UnionFind uf(n);
    vector<Edge> buildEdges;
    vector<Edge> destroyEdges;

    // Parse build costs and destroy costs and create edges
    stringstream buildStream(buildStr);
    stringstream destroyStream(destroyStr);
    string buildLine, destroyLine;

    for (int i = 0; i < n; ++i) {
        getline(buildStream, buildLine, ',');
        getline(destroyStream, destroyLine, ',');

        for (int j = 0; j < n; ++j) {
            if (country[i][j] == 1) { // Existing road
                int destroyCost = letterToCost(destroyLine[j]);
                destroyEdges.push_back({i, j, destroyCost}); // Add destroying cost
            } else if (i < j) { // Only consider build costs once
                int buildCost = letterToCost(buildLine[j]);
                buildEdges.push_back({i, j, buildCost}); // Add building cost
            }
        }
    }

    // First, destroy unnecessary roads
    int destroyCostTotal = 0;
    for (const auto& edge : destroyEdges) {
        if (uf.find(edge.u) != uf.find(edge.v)) {
            uf.unite(edge.u, edge.v); // Merge sets (destroy road)
            destroyCostTotal += edge.cost;
        }
    }

    // Now, build necessary roads to connect disconnected components
    sort(buildEdges.begin(), buildEdges.end(), [](const Edge &a, const Edge &b) {
        return a.cost < b.cost;
    });

    int buildCostTotal = 0;
    for (const auto& edge : buildEdges) {
        if (uf.find(edge.u) != uf.find(edge.v)) {
            uf.unite(edge.u, edge.v); // Merge sets (build road)
            buildCostTotal += edge.cost;
        }
    }

    // Final total cost includes both building and destroying costs
    int totalCost = destroyCostTotal + buildCostTotal;
    cout << totalCost << endl;

    return 0;
}

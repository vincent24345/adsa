#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Edge {
    int u, v, cost;
    bool isBuild;  // true for build, false for destroy
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
    return 0; // Should not happen
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

    // Parse the country matrix
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
    vector<Edge> edges;

    // Parse build and destroy costs into matrices for easy access
    vector<vector<int>> buildCosts(n, vector<int>(n));
    vector<vector<int>> destroyCosts(n, vector<int>(n));

    // Parse build costs
    stringstream buildStream(buildStr);
    for (int i = 0; i < n; ++i) {
        getline(buildStream, line, ',');
        for (int j = 0; j < n; ++j) {
            buildCosts[i][j] = letterToCost(line[j]);
        }
    }

    // Parse destroy costs
    stringstream destroyStream(destroyStr);
    for (int i = 0; i < n; ++i) {
        getline(destroyStream, line, ',');
        for (int j = 0; j < n; ++j) {
            destroyCosts[i][j] = letterToCost(line[j]);
        }
    }

    // Create edges for the graph
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {  // Only consider each pair once
            if (country[i][j] == 1) {
                // Existing road, consider destruction cost
                edges.push_back({i, j, destroyCosts[i][j], false});
            } else {
                // No road, consider build cost
                edges.push_back({i, j, buildCosts[i][j], true});
            }
        }
    }

    // Sort edges by cost
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        return a.cost < b.cost;
    });

    int totalCost = 0;
    // Kruskal's algorithm to construct the MST
    for (const Edge &edge : edges) {
        if (uf.find(edge.u) != uf.find(edge.v)) {
            totalCost += edge.cost;
            uf.unite(edge.u, edge.v);
        }
    }

    cout << totalCost << endl;
    return 0;
}

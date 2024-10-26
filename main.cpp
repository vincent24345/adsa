#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_set>

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

    bool connected(int u, int v) {
        return find(u) == find(v);
    }

private:
    vector<int> parent, rank;
};

// Function to convert letter costs to numerical values
int letterToCost(char c) {
    return isupper(c) ? c - 'A' : c - 'a' + 26; // Treat uppercase and lowercase letters
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

    // Union-Find initialization for the existing roads
    UnionFind uf(n);
    vector<Edge> edges;

    // Parse build and destroy costs into two 2D arrays
    vector<vector<int>> buildCosts(n, vector<int>(n));
    vector<vector<int>> destroyCosts(n, vector<int>(n));
    stringstream buildStream(buildStr), destroyStream(destroyStr);

    for (int i = 0; i < n; ++i) {
        getline(buildStream, line, ',');
        for (int j = 0; j < n; ++j) {
            buildCosts[i][j] = letterToCost(line[j]);
        }
        getline(destroyStream, line, ',');
        for (int j = 0; j < n; ++j) {
            destroyCosts[i][j] = letterToCost(line[j]);
        }
    }

    // Build edges considering existing roads for destruction
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) { // Avoid self-loops and duplicate edges
            if (country[i][j] == 1) {
                edges.push_back({i, j, destroyCosts[i][j], false}); // Existing road (destroy)
                uf.unite(i, j); // Union existing roads
            } else {
                edges.push_back({i, j, buildCosts[i][j], true}); // Non-existing road (build)
            }
        }
    }

    // Sort edges by cost
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        return a.cost < b.cost;
    });

    int totalCost = 0;

    // Kruskal's algorithm with modification to connect all cities
    for (const Edge &edge : edges) {
        if (!uf.connected(edge.u, edge.v)) {
            uf.unite(edge.u, edge.v);
            totalCost += edge.cost;  // Add cost of either building or destroying edges
        }
    }

    cout << totalCost << endl;
    return 0;
}

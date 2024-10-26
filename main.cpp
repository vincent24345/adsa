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

// DFS to check if the graph is already optimally connected
void dfs(int node, vector<vector<int>>& country, vector<bool>& visited) {
    visited[node] = true;
    for (int i = 0; i < country.size(); ++i) {
        if (country[node][i] == 1 && !visited[i]) {
            dfs(i, country, visited);
        }
    }
}

// Check if the initial graph is already a spanning tree
bool isOptimallyConnected(vector<vector<int>>& country) {
    int n = country.size();
    vector<bool> visited(n, false);
    dfs(0, country, visited);

    // Check if all nodes are visited and count the number of edges
    int edgeCount = 0;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) return false;  // Not fully connected
        for (int j = i + 1; j < n; ++j) {
            if (country[i][j] == 1) edgeCount++;
        }
    }

    // Check if the edge count equals n - 1 for a spanning tree
    return edgeCount == n - 1;
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

    // Check if the country is already optimally connected
    if (isOptimallyConnected(country)) {
        cout << 0 << endl;
        return 0;
    }

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

    // Generate edges for build and destroy costs
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (country[i][j] == 1) {
                // Existing road, consider destruction cost
                edges.push_back({i, j, destroyCosts[i][j], false});
            } 
            // Always consider building a new road if it's cheaper or non-existent
            edges.push_back({i, j, buildCosts[i][j], true});
        }
    }

    // Sort edges by cost
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        return a.cost < b.cost;
    });

    int totalCost = 0;
    int edgeCount = 0;

    // Kruskal's algorithm for MST construction
    for (const Edge &edge : edges) {
        if (uf.find(edge.u) != uf.find(edge.v)) {
            uf.unite(edge.u, edge.v);
            totalCost += edge.cost;
            edgeCount++;
            if (edgeCount == n - 1) break;  // Minimum edges for full connection
        }
    }

    cout << totalCost << endl;
    return 0;
}

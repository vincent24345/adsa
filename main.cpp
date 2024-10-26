#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Edge {
    int u, v, cost;
    bool isBuild;  // true if it's a build edge, false if it's a destroy edge
};

// Disjoint Set Union (Union-Find) for Kruskal's algorithm
class UnionFind {
public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int u) {
        if (parent[u] != u) parent[u] = find(parent[u]);
        return parent[u];
    }

    void unite(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            if (rank[rootU] < rank[rootV]) parent[rootU] = rootV;
            else if (rank[rootU] > rank[rootV]) parent[rootV] = rootU;
            else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }

private:
    vector<int> parent, rank;
};

// Convert letter to cost value
int letterToCost(char c) {
    return isupper(c) ? c - 'A' : c - 'a' + 26;
}

// Depth-First Search (DFS) to find connected components
void dfs(int node, const vector<vector<int>>& country, vector<bool>& visited, vector<int>& component) {
    visited[node] = true;
    component.push_back(node);
    for (int i = 0; i < country.size(); ++i) {
        if (country[node][i] == 1 && !visited[i]) dfs(i, country, visited, component);
    }
}

// Find all subgraphs (connected components) in the initial country matrix
vector<vector<int>> findSubgraphs(const vector<vector<int>>& country) {
    int n = country.size();
    vector<bool> visited(n, false);
    vector<vector<int>> subgraphs;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            vector<int> component;
            dfs(i, country, visited, component);
            subgraphs.push_back(component);
        }
    }
    return subgraphs;
}

int main() {
    string input;
    getline(cin, input);

    // Split input into three parts
    stringstream ss(input);
    string countryStr, buildStr, destroyStr;
    getline(ss, countryStr, ' ');
    getline(ss, buildStr, ' ');
    getline(ss, destroyStr, ' ');

    // Parse country matrix
    vector<vector<int>> country;
    stringstream countryStream(countryStr);
    string line;
    while (getline(countryStream, line, ',')) {
        vector<int> row;
        for (char c : line) row.push_back(c - '0');
        country.push_back(row);
    }
    int n = country.size();

    // Parse build and destroy cost matrices
    vector<vector<int>> buildCosts(n, vector<int>(n)), destroyCosts(n, vector<int>(n));
    stringstream buildStream(buildStr), destroyStream(destroyStr);

    for (int i = 0; i < n; ++i) {
        getline(buildStream, line, ',');
        for (int j = 0; j < n; ++j) buildCosts[i][j] = letterToCost(line[j]);

        getline(destroyStream, line, ',');
        for (int j = 0; j < n; ++j) destroyCosts[i][j] = letterToCost(line[j]);
    }

    // Identify connected components (subgraphs)
    vector<vector<int>> subgraphs = findSubgraphs(country);
    int totalCost = 0;

    // Process each subgraph to create MST with minimum cost
    for (const auto& component : subgraphs) {
        UnionFind uf(n);
        vector<Edge> edges;

        // Collect edges within each component
        for (int i : component) {
            for (int j : component) {
                if (i != j) {
                    if (country[i][j] == 1) edges.push_back({i, j, destroyCosts[i][j], false});
                    else edges.push_back({i, j, buildCosts[i][j], true});
                }
            }
        }

        // Sort edges by cost for MST construction
        sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
            return a.cost < b.cost;
        });

        // Apply Kruskal’s algorithm to connect component nodes with minimal cost
        for (const Edge &edge : edges) {
            if (uf.find(edge.u) != uf.find(edge.v)) {
                uf.unite(edge.u, edge.v);
                totalCost += edge.cost;
            }
        }
    }

    // Collect all edges between components for minimum inter-connection
    vector<Edge> interComponentEdges;
    for (int i = 0; i < subgraphs.size(); ++i) {
        for (int j = i + 1; j < subgraphs.size(); ++j) {
            for (int u : subgraphs[i]) {
                for (int v : subgraphs[j]) {
                    interComponentEdges.push_back({u, v, buildCosts[u][v], true});
                }
            }
        }
    }

    // Sort inter-component edges by cost for minimal connection
    sort(interComponentEdges.begin(), interComponentEdges.end(), [](const Edge &a, const Edge &b) {
        return a.cost < b.cost;
    });

    // Connect all subgraphs with minimum possible cost
    UnionFind ufTotal(n);
    for (const Edge &edge : interComponentEdges) {
        if (ufTotal.find(edge.u) != ufTotal.find(edge.v)) {
            ufTotal.unite(edge.u, edge.v);
            totalCost += edge.cost;
        }
    }

    cout << totalCost << endl;
    return 0;
}

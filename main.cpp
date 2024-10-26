#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>

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

// DFS to find all nodes in a subgraph
void dfs(int node, vector<vector<int>>& country, vector<bool>& visited, vector<int>& component) {
    visited[node] = true;
    component.push_back(node);
    for (int i = 0; i < country.size(); ++i) {
        if (country[node][i] == 1 && !visited[i]) {
            dfs(i, country, visited, component);
        }
    }
}

// Find all subgraphs in the country graph
vector<vector<int>> findSubgraphs(vector<vector<int>>& country) {
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

    // Find all subgraphs
    vector<vector<int>> subgraphs = findSubgraphs(country);

    int totalCost = 0;

    for (const auto& component : subgraphs) {
        UnionFind uf(n);
        vector<Edge> edges;

        // Gather edges for MST construction within each component
        for (int i : component) {
            for (int j : component) {
                if (i != j) {
                    if (country[i][j] == 1) {
                        edges.push_back({i, j, destroyCosts[i][j], false});
                    } else {
                        edges.push_back({i, j, buildCosts[i][j], true});
                    }
                }
            }
        }

        // Sort edges by cost
        sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
            return a.cost < b.cost;
        });

        // Apply Kruskal's algorithm for MST within the component
        for (const Edge &edge : edges) {
            if (uf.find(edge.u) != uf.find(edge.v)) {
                uf.unite(edge.u, edge.v);
                totalCost += edge.cost;
            }
        }
    }

    // Gather edges between different subgraphs
    vector<Edge> interSubgraphEdges;
    for (int i = 0; i < subgraphs.size(); ++i) {
        for (int j = i + 1; j < subgraphs.size(); ++j) {
            for (int u : subgraphs[i]) {
                for (int v : subgraphs[j]) {
                    interSubgraphEdges.push_back({u, v, buildCosts[u][v], true});
                }
            }
        }
    }

    // Sort inter-subgraph edges by cost and connect subgraphs
    sort(interSubgraphEdges.begin(), interSubgraphEdges.end(), [](const Edge &a, const Edge &b) {
        return a.cost < b.cost;
    });

    UnionFind ufTotal(n);
    for (const Edge &edge : interSubgraphEdges) {
        if (ufTotal.find(edge.u) != ufTotal.find(edge.v)) {
            ufTotal.unite(edge.u, edge.v);
            totalCost += edge.cost;
        }
    }

    cout << totalCost << endl;
    return 0;
}

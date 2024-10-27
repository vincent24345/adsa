#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>

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
    return isupper(c) ? c - 'A' : c - 'a' + 26; // Treat uppercase and lowercase letters
}

// Function to check if the initial graph is already optimally connected
bool isConnectedComponent(const vector<vector<int>>& country) {
    int n = country.size();
    vector<bool> visited(n, false);

    function<void(int)> dfs = [&](int node) {
        visited[node] = true;
        for (int i = 0; i < n; ++i) {
            if (country[node][i] == 1 && !visited[i]) {
                dfs(i);
            }
        }
    };

    dfs(0);
    return all_of(visited.begin(), visited.end(), [](bool v) { return v; });
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
        vector<int> row(line.size());
        for (size_t i = 0; i < line.size(); ++i) {
            row[i] = line[i] - '0';
        }
        country.push_back(row);
    }

    int n = country.size();

    // Check if the country is already optimally connected
    if (isConnectedComponent(country)) {
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
        for (int j = i + 1; j < n; ++j) { // Only add each edge once (undirected)
            if (country[i][j] == 1) {
                // Existing road, consider destruction cost
                edges.push_back({i, j, destroyCosts[i][j], false});
            } else {
                // Non-existent road, consider build cost
                edges.push_back({i, j, buildCosts[i][j], true});
            }
        }
    }

    // Sort edges by cost
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        return a.cost < b.cost;
    });

    // Kruskal's algorithm with modification
    int totalCost = 0;
    int edgesUsed = 0;  // Track how many edges are used for connecting components

    // Add edges to the total cost based on connection needs
    for (const Edge &edge : edges) {
        // Check if edge connects different components
        if (uf.find(edge.u) != uf.find(edge.v)) {
            uf.unite(edge.u, edge.v);
            totalCost += edge.cost;  // Add cost of either building or destroying edges
            edgesUsed++;

            // Stop if we've connected all components
            if (edgesUsed == n - 1) break;
        }
    }

    // Output the total cost for connecting all components
    cout << totalCost << endl;
    return 0;
}

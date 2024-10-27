#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
using namespace std;

struct Edge {
    int u, v;
    int cost;
    bool isBuild;  // true for build, false for destroy
};

// Union-Find class to manage connected components
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
    return isupper(c) ? c - 'A' : c - 'a' + 26;
}

// Check if all cities are already connected
bool isConnectedComponent(const vector<vector<int>>& country) {
    int n = country.size();
    vector<bool> visited(n, false);

    // Depth First Search
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

    // Split input into country connections, build costs, and destroy costs
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

    // Check if the initial graph is already a spanning tree
    if (isConnectedComponent(country)) {
        cout << 0 << endl;
        return 0;
    }

    UnionFind uf(n);
    vector<Edge> edges;

    // Parse the build and destroy costs into two 2D arrays
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

    // Create edges for existing roads (destruction cost) and potential new roads (build cost)
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (country[i][j] == 1) {
                edges.push_back({i, j, destroyCosts[i][j], false});
            } else {
                edges.push_back({i, j, buildCosts[i][j], true});
            }
        }
    }

    // Sort edges by cost
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        return a.cost < b.cost;
    });

    int totalCost = 0;
    int edgesUsed = 0;

    // Kruskal's algorithm to construct the MST
    for (const Edge &edge : edges) {
        if (uf.find(edge.u) != uf.find(edge.v)) {
            uf.unite(edge.u, edge.v);
            if (edge.isBuild) {
                totalCost += edge.cost;  // Add the cost of building a new edge
            } else {
                totalCost += edge.cost;  // Add the cost of destroying an existing edge
            }
            edgesUsed++;
            if (edgesUsed == n - 1) break;  // Stop when all cities are connected
        }
    }

    cout << totalCost << endl;

    return 0;
}

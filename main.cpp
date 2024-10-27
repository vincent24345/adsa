#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Utility function to convert character to cost
int charToCost(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    else return c - 'a' + 26;
}

// Edge structure to store graph edges
struct Edge {
    int u, v, cost;
    bool operator<(const Edge& other) const {
        return cost < other.cost;
    }
};

// Disjoint-set (Union-Find) structure for Kruskal's algorithm
class UnionFind {
    vector<int> parent, rank;
public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    
    int find(int u) {
        if (parent[u] != u) parent[u] = find(parent[u]);
        return parent[u];
    }
    
    bool unite(int u, int v) {
        int rootU = find(u), rootV = find(v);
        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) parent[rootV] = rootU;
            else if (rank[rootU] < rank[rootV]) parent[rootU] = rootV;
            else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
            return true;
        }
        return false;
    }
};

int minimumReconstructionCost(int n, const vector<string>& country, const vector<string>& build, const vector<string>& destroy) {
    vector<Edge> edges;
    
    // Create edges based on the input matrices
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (country[i][j] == '1') {
                // Existing road with destruction cost
                edges.push_back({i, j, charToCost(destroy[i][j])});
            } else {
                // No road, consider building cost
                edges.push_back({i, j, charToCost(build[i][j])});
            }
        }
    }
    
    // Sort edges by cost
    sort(edges.begin(), edges.end());
    
    // Kruskal's algorithm to find MST
    UnionFind uf(n);
    int minCost = 0;
    
    for (const Edge& edge : edges) {
        if (uf.unite(edge.u, edge.v)) {
            minCost += edge.cost;
        }
    }
    
    return minCost;
}

int main() {
    // Sample input parsing
    int n;
    string countryStr, buildStr, destroyStr;
    cin >> n >> countryStr >> buildStr >> destroyStr;

    vector<string> country(n), build(n), destroy(n);
    for (int i = 0; i < n; ++i) {
        country[i] = countryStr.substr(i * n, n);
        build[i] = buildStr.substr(i * n, n);
        destroy[i] = destroyStr.substr(i * n, n);
    }

    int result = minimumReconstructionCost(n, country, build, destroy);
    cout << result << endl;

    return 0;
}

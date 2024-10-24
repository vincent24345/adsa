#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Edge {
    int u, v, cost;
    bool isBuild; // true if build, false if destroy
};

// Union-Find (Disjoint Set) data structure
class UnionFind {
    vector<int> parent, rank;

public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int u) {
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    bool unionSets(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU == rootV) return false;

        if (rank[rootU] < rank[rootV]) parent[rootU] = rootV;
        else if (rank[rootU] > rank[rootV]) parent[rootV] = rootU;
        else {
            parent[rootV] = rootU;
            rank[rootU]++;
        }
        return true;
    }
};

// Helper function to convert the cost represented by characters
int charToCost(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    return -1; // Should not happen with valid input
}

int main() {
    string countryStr, buildStr, destroyStr;
    cin >> countryStr >> buildStr >> destroyStr;

    // Parse inputs
    vector<string> country, build, destroy;
    int N = countryStr.find(',');
    while (N != string::npos) {
        country.push_back(countryStr.substr(0, N));
        build.push_back(buildStr.substr(0, N));
        destroy.push_back(destroyStr.substr(0, N));
        countryStr.erase(0, N + 1);
        buildStr.erase(0, N + 1);
        destroyStr.erase(0, N + 1);
        N = countryStr.find(',');
    }
    country.push_back(countryStr);
    build.push_back(buildStr);
    destroy.push_back(destroyStr);

    int n = country.size();
    vector<Edge> edges;

    // Process the road network
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (country[i][j] == '1') {
                // There is a road between i and j -> consider destroying it
                edges.push_back({i, j, charToCost(destroy[i][j]), false});
            }
            else {
                // No road between i and j -> consider building it
                edges.push_back({i, j, charToCost(build[i][j]), true});
            }
        }
    }

    // Sort edges by cost
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.cost < b.cost;
    });

    // Apply Kruskal's algorithm to find the minimum cost
    UnionFind uf(n);
    int totalCost = 0;

    for (const Edge& edge : edges) {
        if (uf.unionSets(edge.u, edge.v)) {
            // If the cities are not yet connected, process this edge
            totalCost += edge.cost;
        }
    }

    // Output the total minimal cost
    cout << totalCost << endl;

    return 0;
}

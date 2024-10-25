#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

struct Edge {
    int u, v, buildCost, destroyCost;
};

class UnionFind {
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
            if (rank[rootU] < rank[rootV]) swap(rootU, rootV);
            parent[rootV] = rootU;
            if (rank[rootU] == rank[rootV]) ++rank[rootU];
            return true;
        }
        return false;
    }

private:
    vector<int> parent, rank;
};

int letterToCost(char c) {
    return (c >= 'a') ? (c - 'a' + 26) : (c - 'A');
}

int main() {
    string input;
    getline(cin, input);

    stringstream ss(input);
    string countryStr, buildStr, destroyStr;
    getline(ss, countryStr, ' ');
    getline(ss, buildStr, ' ');
    getline(ss, destroyStr);

    vector<vector<int>> country;
    stringstream countryStream(countryStr);
    string line;
    while (getline(countryStream, line, ',')) {
        vector<int> row;
        for (char c : line) row.push_back(c - '0');
        country.push_back(row);
    }

    int n = country.size();
    vector<Edge> edges;

    int buildIdx = 0, destroyIdx = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int buildCost = letterToCost(buildStr[buildIdx++]);
            int destroyCost = letterToCost(destroyStr[destroyIdx++]);
            edges.push_back({i, j, buildCost, destroyCost});
        }
    }

    // Sort edges to prefer lower-cost operations
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        return min(a.buildCost, a.destroyCost) < min(b.buildCost, b.destroyCost);
    });

    UnionFind uf(n);
    int totalCost = 0;

    for (const Edge &edge : edges) {
        int u = edge.u, v = edge.v;
        if (uf.find(u) != uf.find(v)) {
            if (country[u][v] == 1) {
                // Road exists, choose between keeping or rebuilding
                if (edge.destroyCost < edge.buildCost) {
                    totalCost += edge.destroyCost;
                } else {
                    totalCost += edge.buildCost;
                }
            } else {
                // No road, so build it
                totalCost += edge.buildCost;
            }
            uf.unite(u, v);
        } else if (country[u][v] == 1) {
            // If cities are already connected but have an existing road, destroy it if beneficial
            totalCost += edge.destroyCost;
        }
    }

    cout << totalCost << endl;
    return 0;
}

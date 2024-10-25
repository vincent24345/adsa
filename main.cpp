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

    // Sort edges to prefer lower-cost operations (destroy or build)
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        return min(a.buildCost, a.destroyCost) < min(b.buildCost, b.destroyCost);
    });

    UnionFind uf(n);
    int totalCost = 0;

    for (const Edge &edge : edges) {
        int u = edge.u, v = edge.v;
        bool connected = (uf.find(u) == uf.find(v));

        if (country[u][v] == 1) {  // Road exists
            if (connected) {
                // Only destroy if it reduces cost without disconnecting
                totalCost += edge.destroyCost;
            } else {
                // Decide to either keep or replace the road
                int cost = min(edge.buildCost, edge.destroyCost);
                totalCost += cost;
                uf.unite(u, v);
            }
        } else if (!connected) {  // No existing road
            totalCost += edge.buildCost;
            uf.unite(u, v);
        }
    }

    cout << totalCost << endl;
    return 0;
}

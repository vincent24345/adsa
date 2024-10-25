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
        int rootU = find(u), rootV = find(v);
        if (rootU != rootV) {
            if (rank[rootU] < rank[rootV]) swap(rootU, rootV);
            parent[rootV] = rootU;
            if (rank[rootU] == rank[rootV]) ++rank[rootU];
        }
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
    UnionFind uf(n);
    vector<Edge> edges;

    int buildIdx = 0, destroyIdx = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (country[i][j] == 1) {
                int destroyCost = letterToCost(destroyStr[destroyIdx++]);
                edges.push_back({i, j, destroyCost, false});
                edges.push_back({i, j, 0, true});  // Option to keep the road
            } else {
                int buildCost = letterToCost(buildStr[buildIdx++]);
                edges.push_back({i, j, buildCost, true});
            }
        }
    }

    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        return a.cost < b.cost;
    });

    int totalCost = 0;
    for (const Edge &edge : edges) {
        if (uf.find(edge.u) != uf.find(edge.v)) {
            totalCost += edge.cost;
            uf.unite(edge.u, edge.v);
        }
    }

    cout << totalCost << endl;
    return 0;
}
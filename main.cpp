#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>

using namespace std;

// Helper function to convert character costs to integer costs
int charToCost(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    return -1;  // Invalid character
}

// Disjoint Set Union (DSU) or Union-Find structure to manage connected components
class DSU {
public:
    DSU(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int x, int y) {
        int xr = find(x), yr = find(y);
        if (xr == yr) return false;

        if (rank[xr] < rank[yr]) {
            parent[xr] = yr;
        } else if (rank[xr] > rank[yr]) {
            parent[yr] = xr;
        } else {
            parent[yr] = xr;
            rank[xr]++;
        }
        return true;
    }

private:
    vector<int> parent;
    vector<int> rank;
};

int main() {
    string country_str, build_str, destroy_str;
    cin >> country_str >> build_str >> destroy_str;

    // Parse the input strings into 2D arrays
    vector<vector<int>> country, build, destroy;
    for (int i = 0; i < country_str.length(); i += 3) {
        vector<int> row;
        for (int j = 0; j < 3; j++) {
            row.push_back(country_str[i + j] - '0');
        }
        country.push_back(row);
    }

    for (int i = 0; i < build_str.length(); i += 3) {
        vector<int> row;
        for (int j = 0; j < 3; j++) {
            row.push_back(charToCost(build_str[i + j]));
        }
        build.push_back(row);
    }

    for (int i = 0; i < destroy_str.length(); i += 3) {
        vector<int> row;
        for (int j = 0; j < 3; j++) {
            row.push_back(charToCost(destroy_str[i + j]));
        }
        destroy.push_back(row);
    }

    int n = country.size();
    DSU dsu(n);

    vector<pair<int, pair<int, int>>> edges;
    vector<pair<int, pair<int, int>>> destroyEdges;

    // Build the edges from the country matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (country[i][j] == 1) {
                destroyEdges.push_back({destroy[i][j], {i, j}});
            } else if (i != j) {
                edges.push_back({build[i][j], {i, j}});
            }
        }
    }

    // Sort the destroy edges in ascending order of cost
    sort(destroyEdges.begin(), destroyEdges.end());

    int totalCost = 0;

    // Add destroy edges first
    for (auto& edge : destroyEdges) {
        int cost = edge.first;
        int u = edge.second.first;
        int v = edge.second.second;

        if (dsu.find(u) == dsu.find(v)) {
            totalCost += cost;
        } else {
            dsu.unite(u, v);
        }
    }

    // Sort the build edges in ascending order of cost
    sort(edges.begin(), edges.end());

    // Add build edges to form MST
    for (auto& edge : edges) {
        int cost = edge.first;
        int u = edge.second.first;
        int v = edge.second.second;

        if (dsu.unite(u, v)) {
            totalCost += cost;
        }
    }

    cout << totalCost << endl;

    return 0;
}

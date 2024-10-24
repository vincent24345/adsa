#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 55;
int parent[MAXN], rank_[MAXN];

// Helper function to convert the letter representation to an integer cost
int convertCost(char c) {
    if ('A' <= c && c <= 'Z') return c - 'A';
    if ('a' <= c && c <= 'z') return c - 'a' + 26;
    return 0; // Should not happen with valid input
}

// Union-Find (Disjoint Set) methods for Kruskal's algorithm
int find(int u) {
    if (parent[u] != u) {
        parent[u] = find(parent[u]);
    }
    return parent[u];
}

void unionSets(int u, int v) {
    int rootU = find(u);
    int rootV = find(v);
    if (rootU != rootV) {
        if (rank_[rootU] > rank_[rootV]) {
            parent[rootV] = rootU;
        } else if (rank_[rootU] < rank_[rootV]) {
            parent[rootU] = rootV;
        } else {
            parent[rootV] = rootU;
            rank_[rootU]++;
        }
    }
}

// Edge structure to store the cost and the nodes
struct Edge {
    int u, v, cost;
    bool toDestroy; // true if this is an existing road to destroy

    bool operator<(const Edge& other) const {
        return cost < other.cost;
    }
};

int main() {
    string countryStr, buildStr, destroyStr;
    cin >> countryStr >> buildStr >> destroyStr;

    // Split the input strings into 2D arrays for country, build, destroy
    vector<vector<int>> country;
    vector<vector<int>> build;
    vector<vector<int>> destroy;
    int N = 0;
    for (size_t i = 0; i < countryStr.length(); ++i) {
        if (countryStr[i] == ',') continue;
        int row = i / (N + 1);
        if (row >= N) N++;
        country[row].push_back(countryStr[i] - '0');
        build[row].push_back(convertCost(buildStr[i]));
        destroy[row].push_back(convertCost(destroyStr[i]));
    }

    // Prepare for Kruskal's algorithm
    vector<Edge> edges;
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            if (country[i][j] == 1) {
                edges.push_back({i, j, destroy[i][j], true}); // Destroy road
            }
            edges.push_back({i, j, build[i][j], false}); // Build road
        }
    }

    // Sort edges by cost (Kruskal's algorithm)
    sort(edges.begin(), edges.end());

    // Initialize Union-Find structures
    for (int i = 0; i < N; ++i) {
        parent[i] = i;
        rank_[i] = 0;
    }

    int totalCost = 0;
    for (const Edge& edge : edges) {
        if (find(edge.u) != find(edge.v)) {
            unionSets(edge.u, edge.v);
            totalCost += edge.cost;
        } else if (edge.toDestroy) {
            totalCost += edge.cost; // Destroy the redundant road
        }
    }

    cout << totalCost << endl;
    return 0;
}

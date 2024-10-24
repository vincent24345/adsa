#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <tuple>

using namespace std;

// Disjoint Set Union-Find Class
class UnionFind {
public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int u) {
        if (parent[u] != u) parent[u] = find(parent[u]);
        return parent[u];
    }

    void unite(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }

private:
    vector<int> parent, rank;
};

// Convert a letter to a cost
int letterToCost(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A' + 1;  // A = 1, B = 2, ..., Z = 26
    if (c >= 'a' && c <= 'z') return c - 'a' + 27; // a = 27, b = 28, ..., z = 52
    return 0;  // default case for any unexpected input
}

int main() {
    string input;
    getline(cin, input);

    stringstream ss(input);
    string countryStr, buildStr, destroyStr;
    getline(ss, countryStr, ' ');
    getline(ss, buildStr, ' ');
    getline(ss, destroyStr, ' ');

    // Parsing adjacency matrix (country)
    vector<vector<int>> country;
    stringstream countryStream(countryStr);
    string row;
    while (getline(countryStream, row, ',')) {
        vector<int> tempRow;
        for (char c : row) {
            tempRow.push_back(c - '0');
        }
        country.push_back(tempRow);
    }

    int n = country.size();
    UnionFind uf(n);
    vector<tuple<int, int, int>> edgesToBuild;     // (cost, u, v)
    vector<tuple<int, int, int>> edgesToDestroy;   // (cost, u, v)

    // Parsing build and destroy cost matrices
    stringstream buildStream(buildStr);
    stringstream destroyStream(destroyStr);
    string buildRow, destroyRow;

    for (int i = 0; i < n; ++i) {
        getline(buildStream, buildRow, ',');
        getline(destroyStream, destroyRow, ',');

        for (int j = 0; j < n; ++j) {
            if (i != j) {  // Avoid self-loops
                int buildCost = letterToCost(buildRow[j]);
                int destroyCost = letterToCost(destroyRow[j]);

                if (country[i][j] == 1) {
                    // Road exists, consider destruction
                    edgesToDestroy.push_back(make_tuple(destroyCost, i, j));
                } else {
                    // Road does not exist, consider building
                    edgesToBuild.push_back(make_tuple(buildCost, i, j));
                }
            }
        }
    }

    // Handle destruction of unnecessary roads (if needed)
    int totalDestroyCost = 0;
    for (const auto& edge : edgesToDestroy) {
        int destroyCost, u, v;
        tie(destroyCost, u, v) = edge;
        if (uf.find(u) == uf.find(v)) {
            totalDestroyCost += destroyCost;  // Add destroy cost if already connected
        } else {
            uf.unite(u, v);  // Unite if not connected
        }
    }

    // Sort edges by build cost (for building roads)
    sort(edgesToBuild.begin(), edgesToBuild.end());

    int totalBuildCost = 0;
    for (const auto& edge : edgesToBuild) {
        int buildCost, u, v;
        tie(buildCost, u, v) = edge;
        if (uf.find(u) != uf.find(v)) {
            uf.unite(u, v);
            totalBuildCost += buildCost;  // Add build cost only if cities are not connected
        }
    }

    // Calculate total cost (destroy + build)
    int totalCost = totalDestroyCost + totalBuildCost;
    cout << totalCost << endl;

    return 0;
}

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Represent roads between cities with their build/destroy cost and status
struct Road {
    int cityA, cityB;
    int cost;
    bool isNew; // true if road needs to be built
};

// Disjoint Set Union-Find class to manage connected cities
class DisjointSet {
public:
    DisjointSet(int n) {
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

// Convert letter to cost using ASCII difference
int getCost(char c) {
    return isupper(c) ? c - 'A' : c - 'a' + 26;
}

// Parse a string with comma-separated values to an integer matrix
vector<vector<int>> parseIntegerMatrix(const string& data) {
    vector<vector<int>> matrix;
    stringstream ss(data);
    string row;
    while (getline(ss, row, ',')) {
        vector<int> currentRow;
        for (char ch : row) currentRow.push_back(ch - '0');
        matrix.push_back(currentRow);
    }
    return matrix;
}

// Parse a string with comma-separated values to a character matrix
vector<vector<char>> parseCharMatrix(const string& data) {
    vector<vector<char>> matrix;
    stringstream ss(data);
    string row;
    while (getline(ss, row, ',')) {
        vector<char> currentRow(row.begin(), row.end());
        matrix.push_back(currentRow);
    }
    return matrix;
}

// Check if all cities are connected in the existing setup
bool isAlreadyConnected(const vector<vector<int>>& map) {
    int n = map.size();
    vector<bool> visited(n, false);

    function<void(int)> dfs = [&](int node) {
        visited[node] = true;
        for (int j = 0; j < n; ++j) {
            if (map[node][j] && !visited[j]) dfs(j);
        }
    };

    dfs(0);
    return all_of(visited.begin(), visited.end(), [](bool v) { return v; });
}

int main() {
    string input;
    getline(cin, input);

    // Split input into three parts
    stringstream ss(input);
    string countryData, buildData, destroyData;
    getline(ss, countryData, ' ');
    getline(ss, buildData, ' ');
    getline(ss, destroyData, ' ');

    // Parse country connection matrix and costs
    vector<vector<int>> country = parseIntegerMatrix(countryData);
    vector<vector<char>> buildCosts = parseCharMatrix(buildData);
    vector<vector<char>> destroyCosts = parseCharMatrix(destroyData);
    int n = country.size();

    if (isAlreadyConnected(country)) {
        cout << "0\n";
        return 0;
    }

    // Initialize roads vector for MST
    vector<Road> roads;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (country[i][j]) {
                roads.push_back({i, j, getCost(destroyCosts[i][j]), false});
            } else {
                roads.push_back({i, j, getCost(buildCosts[i][j]), true});
            }
        }
    }

    // Sort roads by cost for MST using Kruskal’s algorithm
    sort(roads.begin(), roads.end(), [](const Road& a, const Road& b) {
        return a.cost < b.cost;
    });

    DisjointSet ds(n);
    int totalCost = 0;
    int edgesUsed = 0;

    for (const Road& road : roads) {
        if (ds.find(road.cityA) != ds.find(road.cityB)) {
            ds.unite(road.cityA, road.cityB);
            totalCost += road.cost;
            edgesUsed++;

            // Break once we connect all cities
            if (edgesUsed == n - 1) break;
        }
    }

    cout << totalCost << endl;
    return 0;
}

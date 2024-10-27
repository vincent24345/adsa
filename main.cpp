#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Structure representing each road with cost details and status
struct Road {
    int from, to;
    int cost;
    bool isPlanned; // true if road needs to be built
};

// Union-Find structure to handle city connectivity for MST construction
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

// Calculate road cost using ASCII difference for simplicity
int computeCost(char ch) {
    return isupper(ch) ? ch - 'A' : ch - 'a' + 26;
}

// Function to parse a comma-separated string into an integer matrix
vector<vector<int>> parseIntMatrix(const string& data) {
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

// Function to parse a comma-separated string into a character matrix
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

// Check if all cities are already connected using DFS traversal
bool citiesConnected(const vector<vector<int>>& adjacencyMatrix) {
    int n = adjacencyMatrix.size();
    vector<bool> visited(n, false);

    function<void(int)> dfs = [&](int node) {
        visited[node] = true;
        for (int j = 0; j < n; ++j) {
            if (adjacencyMatrix[node][j] && !visited[j]) dfs(j);
        }
    };

    dfs(0);
    return all_of(visited.begin(), visited.end(), [](bool visitedCity) { return visitedCity; });
}

int main() {
    string inputLine;
    getline(cin, inputLine);

    // Separate input into different parts
    stringstream ss(inputLine);
    string countryData, buildData, destroyData;
    getline(ss, countryData, ' ');
    getline(ss, buildData, ' ');
    getline(ss, destroyData, ' ');

    // Parse connection matrix and cost matrices
    vector<vector<int>> country = parseIntMatrix(countryData);
    vector<vector<char>> buildCostMatrix = parseCharMatrix(buildData);
    vector<vector<char>> destroyCostMatrix = parseCharMatrix(destroyData);
    int cityCount = country.size();

    // If cities are connected, no cost is required
    if (citiesConnected(country)) {
        cout << "0\n";
        return 0;
    }

    // Road data collection for MST using Kruskal's algorithm
    vector<Road> roadList;
    for (int i = 0; i < cityCount; ++i) {
        for (int j = i + 1; j < cityCount; ++j) {
            if (country[i][j]) {
                roadList.push_back({i, j, computeCost(destroyCostMatrix[i][j]), false});
            } else {
                roadList.push_back({i, j, computeCost(buildCostMatrix[i][j]), true});
            }
        }
    }

    // Sort roads by cost for MST construction
    sort(roadList.begin(), roadList.end(), [](const Road& a, const Road& b) {
        return a.cost < b.cost;
    });

    UnionFind uf(cityCount);
    int minimumCost = 0;
    int connectedEdges = 0;

    for (const Road& road : roadList) {
        if (uf.find(road.from) != uf.find(road.to)) {
            uf.unite(road.from, road.to);
            minimumCost += road.cost;
            connectedEdges++;

            // Stop once all cities are connected
            if (connectedEdges == cityCount - 1) break;
        }
    }

    cout << minimumCost << endl;
    return 0;
}

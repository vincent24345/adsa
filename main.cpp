#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

// Function to convert character to corresponding cost value
int charToCost(char c) {
    if ('A' <= c && c <= 'Z') {
        return c - 'A';
    } else if ('a' <= c && c <= 'z') {
        return c - 'a' + 26;
    }
    return INT_MAX;
}

// Function to parse the input string into matrices
void parseInput(const string& input, vector<vector<int>>& country, 
                vector<vector<int>>& build, vector<vector<int>>& destroy) {
    stringstream ss(input);
    string countryStr, buildStr, destroyStr;

    getline(ss, countryStr, ' ');
    getline(ss, buildStr, ' ');
    getline(ss, destroyStr, ' ');

    // Parse the country matrix
    stringstream countryStream(countryStr);
    string row;
    while (getline(countryStream, row, ',')) {
        vector<int> temp;
        for (char c : row) {
            temp.push_back(c - '0');
        }
        country.push_back(temp);
    }

    // Parse the build cost matrix
    stringstream buildStream(buildStr);
    while (getline(buildStream, row, ',')) {
        vector<int> temp;
        for (char c : row) {
            temp.push_back(charToCost(c));
        }
        build.push_back(temp);
    }

    // Parse the destroy cost matrix
    stringstream destroyStream(destroyStr);
    while (getline(destroyStream, row, ',')) {
        vector<int> temp;
        for (char c : row) {
            temp.push_back(charToCost(c));
        }
        destroy.push_back(temp);
    }
}

// Function to find connected components using DFS
void dfs(int node, vector<bool>& visited, vector<int>& component, const vector<vector<int>>& country) {
    visited[node] = true;
    component.push_back(node);
    for (int neighbor = 0; neighbor < country.size(); ++neighbor) {
        if (country[node][neighbor] == 1 && !visited[neighbor]) {
            dfs(neighbor, visited, component, country);
        }
    }
}

vector<vector<int>> findConnectedComponents(const vector<vector<int>>& country) {
    int N = country.size();
    vector<bool> visited(N, false);
    vector<vector<int>> components;

    for (int i = 0; i < N; ++i) {
        if (!visited[i]) {
            vector<int> component;
            dfs(i, visited, component, country);
            components.push_back(component);
        }
    }
    return components;
}

// Function to calculate the minimum cost to reconstruct the road system
int calculateCosts(const vector<vector<int>>& components, 
                   const vector<vector<int>>& build, 
                   const vector<vector<int>>& destroy, 
                   const vector<vector<int>>& country) {
    int totalCost = 0;

    for (const auto& component : components) {
        vector<pair<int, int>> edges; // (destroy_cost, city1)
        int destroyCost = 0;

        for (size_t i = 0; i < component.size(); ++i) {
            for (size_t j = i + 1; j < component.size(); ++j) {
                int u = component[i];
                int v = component[j];
                if (country[u][v] == 1) {
                    edges.push_back({destroy[u][v], u});
                    destroyCost += destroy[u][v];
                }
            }
        }

        // Sort edges by destroy cost
        sort(edges.begin(), edges.end());

        // Remove the most expensive road to ensure a single path
        if (!edges.empty()) {
            totalCost += destroyCost - edges.back().first; // Remove the most expensive road
        }
    }

    return totalCost;
}

// Main function to build the minimum cost road network
int buildMinimumCostRoadNetwork(const string& input) {
    vector<vector<int>> country, build, destroy;
    parseInput(input, country, build, destroy);
    
    vector<vector<int>> components = findConnectedComponents(country);
    int minCost = calculateCosts(components, build, destroy, country);

    return minCost;
}

int main() {
    // Sample input
    string input = "011000,101000,110000,000011,000101,000110 ABDFFF,BACFFF,DCAFFF,FFFABD,FFFBAC,FFFDCA ABDFFF,BACFFF,DCAFFF,FFFABD,FFFBAC,FFFDCA";
    cout << buildMinimumCostRoadNetwork(input) << endl; // Expected output: 7
    return 0;
}

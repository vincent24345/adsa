#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

int charToCost(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    else return c - 'a' + 26; // Assuming a-z maps to 26-51
}

int main() {
    int N;  // Number of cities
    cin >> N;

    if (N <= 0) {
        cout << "Invalid number of cities." << endl;
        return 1;
    }

    // Initialize matrices
    vector<vector<int>> country(N, vector<int>(N, 0));
    vector<vector<int>> build(N, vector<int>(N, INF));
    vector<vector<int>> destroy(N, vector<int>(N, INF));

    string country_input, build_input, destroy_input;

    // Read inputs for the country representation, building costs, and destruction costs
    cin >> country_input >> build_input >> destroy_input;

    if (country_input.length() != N * N || build_input.length() != N * N || destroy_input.length() != N * N) {
        cout << "Input string length mismatch." << endl;
        return 1;
    }

    // Parse the input strings into respective matrices
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int idx = i * N + j;
            country[i][j] = (country_input[idx] == '1') ? 1 : 0;
            build[i][j] = charToCost(build_input[j]);
            destroy[i][j] = (country[i][j] == 1) ? charToCost(destroy_input[j]) : INF;
        }
    }

    // Prim's Algorithm to find the minimum spanning tree
    vector<bool> inMST(N, false);
    vector<int> minEdge(N, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // Start from any city, say city 0
    minEdge[0] = 0;
    pq.push({0, 0}); // {cost, city}

    int totalCost = 0;

    while (!pq.empty()) {
        int cost = pq.top().first;
        int city = pq.top().second;
        pq.pop();

        // If the city is already in the MST, skip it
        if (inMST[city]) continue;

        // Include this city in the MST
        inMST[city] = true;
        totalCost += cost;

        // Update the minEdge for the neighboring cities
        for (int nextCity = 0; nextCity < N; ++nextCity) {
            // If there's a road, consider destruction cost
            if (country[city][nextCity] == 1) {
                int destroyCost = destroy[city][nextCity];
                if (!inMST[nextCity] && destroyCost < minEdge[nextCity]) {
                    minEdge[nextCity] = destroyCost;
                    pq.push({destroyCost, nextCity});
                }
            } else {
                // If no road exists, consider building cost
                int buildCost = build[city][nextCity];
                if (!inMST[nextCity] && buildCost < minEdge[nextCity]) {
                    minEdge[nextCity] = buildCost;
                    pq.push({buildCost, nextCity});
                }
            }
        }
    }

    // Output the total minimal cost
    cout << totalCost << endl;

    return 0;
}

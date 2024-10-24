#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

const int INF = 1e9;

// Helper function to convert a character to its corresponding cost
int charToCost(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    else return c - 'a' + 26;
}

int main() {
    int N;  // Number of cities
    cin >> N;

    vector<vector<int>> country(N, vector<int>(N, 0));
    vector<vector<int>> build(N, vector<int>(N, INF));
    vector<vector<int>> destroy(N, vector<int>(N, INF));

    string country_input, build_input, destroy_input;

    // Read inputs for the country representation, building costs, and destruction costs
    cin >> country_input >> build_input >> destroy_input;

    // Parse the input strings into respective matrices
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (country_input[i * N + j] == '1') {
                country[i][j] = 1;  // There is a road
                destroy[i][j] = charToCost(destroy_input[j]);
            }
            else {
                country[i][j] = 0;  // No road
                build[i][j] = charToCost(build_input[j]);
            }
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

import sys

# Available characters for input
input_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

# Function to determine cost of each character by its index in the input list
def char_cost(c):
    return input_chars.index(c)

# Create an adjacency matrix for cost, based on existing and desired roads
def create_cost_matrix(countries, build, destroy):
    rows = len(countries)
    cost_matrix = [[0] * rows for _ in range(rows)]

    for i in range(rows):
        for j in range(rows):
            if countries[i][j] == 0:  # No road exists
                cost_matrix[i][j] += char_cost(build[i][j])
            else:  # Road already exists
                cost_matrix[i][j] -= char_cost(destroy[i][j])
    return cost_matrix

# Disjoint set (Union-Find) structure with path compression and ranking
parent, rank = {}, {}

def find(node):
    if parent[node] != node:
        parent[node] = find(parent[node])
    return parent[node]

def union(x, y):
    root_x, root_y = find(x), find(y)
    if root_x != root_y:
        if rank[root_x] > rank[root_y]:
            parent[root_y] = root_x
        elif rank[root_x] < rank[root_y]:
            parent[root_x] = root_y
        else:
            parent[root_y] = root_x
            rank[root_x] += 1

# Implement Kruskal's algorithm to get MST and minimum cost
def kruskal_mst(cost_matrix, cities_count):
    mst_matrix = [[0] * cities_count for _ in range(cities_count)]
    total_cost = 0

    # Initialize the parent and rank for each city
    for i in range(cities_count):
        parent[i], rank[i] = i, 0

    edges_in_mst = 0
    while edges_in_mst < cities_count - 1:
        min_edge = sys.maxsize
        u, v = -1, -1
        for i in range(cities_count):
            for j in range(cities_count):
                if find(i) != find(j) and cost_matrix[i][j] < min_edge:
                    min_edge = cost_matrix[i][j]
                    u, v = i, j
        union(u, v)
        mst_matrix[u][v] = mst_matrix[v][u] = 1
        edges_in_mst += 1
        total_cost += min_edge

    return mst_matrix, total_cost

# Calculate the minimum cost after considering the MST and existing roads
def compute_min_cost(countries, mst, cost_matrix):
    min_cost = 0
    cities = len(countries)

    for i in range(cities):
        for j in range(cities):
            if countries[i][j] == 0 and mst[i][j] == 1:
                min_cost += cost_matrix[i][j]
            elif countries[i][j] == 1 and mst[i][j] == 0:
                min_cost += abs(cost_matrix[i][j])
    return min_cost // 2  # To account for double-counting bidirectional roads

# Convert a comma-separated string to a matrix of integers
def parse_int_matrix(s):
    rows = s.split(',')
    return [[int(ch) for ch in row] for row in rows]

# Convert a comma-separated string to a matrix of characters
def parse_char_matrix(s):
    rows = s.split(',')
    return [[ch for ch in row] for row in rows]

# Main function to read input and compute the result
def main():
    input_line = input().strip()
    parts = input_line.split()

    countries_matrix = parse_int_matrix(parts[0])
    build_matrix = parse_char_matrix(parts[1])
    destroy_matrix = parse_char_matrix(parts[2])

    cost_matrix = create_cost_matrix(countries_matrix, build_matrix, destroy_matrix)
    mst, _ = kruskal_mst(cost_matrix, len(countries_matrix))

    min_cost = compute_min_cost(countries_matrix, mst, cost_matrix)
    print(min_cost)

if __name__ == "__main__":
    main()

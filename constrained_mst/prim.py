# Modification of Prim's and Kruskal's MST algorithms to include degree constraints
# Minimum Spanning Tree using Boruvka's Algorithm
# CSCI-7432 - Algorithms and Data Structures
# Paul Bupe

parent, rank = {}, {}

# create adjacency matrix for use in prims algorithm
def create_adj_matrix(V, G):
    adj_matrix = []

    # create N x N matrix filled with 0 edge weights between all vertices
    for i in range(0, V):
        adj_matrix.append([])
        for j in range(0, V):
            adj_matrix[i].append(0)

    # populate adjacency matrix with correct edge weights
    for i in range(0, len(G)):
        adj_matrix[G[i][0]][G[i][1]] = G[i][2]
        adj_matrix[G[i][1]][G[i][0]] = G[i][2]
    # print DataFrame(adj_matrix)
    return adj_matrix

# Find and union implementations for the disjoint set data structure
def find(v):
    if parent[v] != v:
        parent[v] = find(parent[v])
    return parent[v]

def union(v1, v2):
    root1 = find(v1)
    root2 = find(v2)

    if rank[root1] < rank[root2]:
        parent[root1] = root2
    elif rank[root1] > rank[root2]:
        parent[root2] = root1
    else:
        parent[root2] = root1
        rank[root1] += 1

#
def kruskal(V, G, C):
    kruskal_mst = []
    edges = G
    edges.sort(key=lambda x: x[2])

    # Keep track of the degree of each vertex
    kruskal_degree_list = [0] * V

    # generate sets
    for v in range(0, V):
        parent[v] = v
        rank[v] = 0

    # Go through all edges and perform find and union operations.
    # Ignore any edge that connects to a "full" vertex
    for e in range(0, len(edges)):
        if kruskal_degree_list[edges[e][1]] >= C or kruskal_degree_list[edges[e][0]] >= C:
            continue

        if find(edges[e][0]) != find(edges[e][1]):
            union(edges[e][0], edges[e][1])

            kruskal_degree_list[edges[e][0]] += 1
            kruskal_degree_list[edges[e][1]] += 1

            kruskal_mst.append(edges[e][2])

    if len(kruskal_mst) < V -1:
        return [0]
    else:
        return sorted(kruskal_mst)


def prims(V, G, C):
    # create adj matrix from graph
    adj_matrix = create_adj_matrix(V, G)

    # arbitrarily choose initial vertex from graph
    vertex = 0

    # Keep track of the degree of each vertex
    degree_list = [0] * V

    # initialize empty edges array and empty mst
    mst = []
    edges = []
    visited = []
    min_edge = [None, None, float('inf')]

    # run prims algorithm until we create an mst
    # that contains every vertex from the graph
    while len(mst) != V - 1:

        # mark this vertex as visited
        visited.append(vertex)

        # add each edge to list of potential edges
        for r in range(0, V):
            if adj_matrix[vertex][r] != 0:
                edges.append([vertex, r, adj_matrix[vertex][r]])
        
        # find edge with the smallest weight to a vertex
        # that has not yet been visited
        # Ignore any edge that connects to a "full" vertex
        for e in range(0, len(edges)):
            if degree_list[edges[e][1]] >= C or degree_list[edges[e][0]] >= C:
                continue

            if edges[e][2] < min_edge[2] and edges[e][1] not in visited:
                min_edge = edges[e]
        
        # Algorithm has failed
        if min_edge[0] is None:
            return [0]

        degree_list[min_edge[0]] += 1
        degree_list[min_edge[1]] += 1

        # remove min weight edge from list of edges
        edges.remove(min_edge)
        
        # push min edge to mst
        mst.append(min_edge[2])

        # start at new vertex and reset min edge
        vertex = min_edge[1]
        min_edge = [None, None, float('inf')]
    # print degree_list

    if len(mst) < V -1:
        return [0]
    else:
        return sorted(mst)


# graph vertices are actually represented as numbers
# like so: 0, 1, 2, ... V-1
a, b, c, d, e, f, g, h, i, j = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9

# graph edges with weights
# diagram of graph is shown above
graph = [
    [a, b, 3],
    [a, f, 2],
    [b, c, 17],
    [b, d, 16],
    [c, d, 8],
    [c, i, 18],
    [d, i, 11],
    [d, e, 4],
    [e, f, 1],
    [e, g, 6],
    [e, h, 5],
    [e, i, 10],
    [f, g, 7],
    [g, h, 15],
    [h, i, 12],
    [h, j, 13],
    [i, j, 9]
]

# pass the # of vertices and the graph to run prims algorithm
mst = prims(10, graph, 2)
print("Spanning Tree: ", mst)
print("Total cost: ", sum(mst))

mst = kruskal(10, graph, 2)
print("Spanning Tree: ", mst)
print("Total cost: ", sum(mst))

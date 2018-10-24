# Modification of Prim's MST algorithm to include degree constraints 
# Minimum Spanning Tree using Boruvka's Algorithm
# CSCI-7432 - Algorithms and Data Structures
# Original code for Prim's algorithm from https://coderbyte.com/algorithm/find-minimum-spanning-tree-using-prims-algorithm
# Paul Bupe

# create adjacency matrix for use in prims algorithm
def createAdjMatrix(V, G):
    adjMatrix = []

    # create N x N matrix filled with 0 edge weights between all vertices
    for i in range(0, V):
        adjMatrix.append([])
        for j in range(0, V):
            adjMatrix[i].append(0)

    # populate adjacency matrix with correct edge weights
    for i in range(0, len(G)):
        adjMatrix[G[i][0]][G[i][1]] = G[i][2]
        adjMatrix[G[i][1]][G[i][0]] = G[i][2]
    # print DataFrame(adjMatrix)
    return adjMatrix


def prims(V, G, C):
    # create adj matrix from graph
    adjMatrix = createAdjMatrix(V, G)

    # arbitrarily choose initial vertex from graph
    vertex = 0

    # Keep track of the degree of each vertex
    degreeList = [0] * V

    # initialize empty edges array and empty MST
    MST = []
    edges = []
    visited = []
    minEdge = [None, None, float('inf')]

    # run prims algorithm until we create an MST
    # that contains every vertex from the graph
    while len(MST) != V - 1:

        # mark this vertex as visited
        visited.append(vertex)

        # add each edge to list of potential edges
        for r in range(0, V):
            if adjMatrix[vertex][r] != 0:
                edges.append([vertex, r, adjMatrix[vertex][r]])
        
        # find edge with the smallest weight to a vertex
        # that has not yet been visited
        # Ignore any edge that connects to a "full" vertex
        for e in range(0, len(edges)):
            if degreeList[edges[e][1]] >= C or degreeList[edges[e][0]] >= C:
                continue

            if edges[e][2] < minEdge[2] and edges[e][1] not in visited:
                minEdge = edges[e]
        
        # Algorithm has failed
        if minEdge[0] is None:
            return [0]

        degreeList[minEdge[0]] += 1
        degreeList[minEdge[1]] += 1

        # remove min weight edge from list of edges
        edges.remove(minEdge)
        
        # push min edge to MST
        MST.append(minEdge[2])

        # start at new vertex and reset min edge
        vertex = minEdge[1]
        minEdge = [None, None, float('inf')]
    # print degreeList
    return MST


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
mst = prims(10, graph, 3)
print "Spanning Tree: ", mst
print "Total cost: ", sum(mst)

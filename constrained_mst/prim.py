# create adjacency matrix for use in prims algorithm
# note: we could improve the running time of prims algorithm by
# implementing a priority queue data structure instead of a matrix
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

    return adjMatrix


def prims(V, G):
    # create adj matrix from graph
    adjMatrix = createAdjMatrix(V, G)

    # arbitrarily choose initial vertex from graph
    vertex = 0

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
        for e in range(0, len(edges)):
            if edges[e][2] < minEdge[2] and edges[e][1] not in visited:
                minEdge = edges[e]

        # remove min weight edge from list of edges
        edges.remove(minEdge)

        # push min edge to MST
        MST.append(minEdge)

        # start at new vertex and reset min edge
        vertex = minEdge[1]
        minEdge = [None, None, float('inf')]

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
    [d, i, 4],
    [d, e, 11],
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
print(prims(10, graph))

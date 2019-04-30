
> Network Science CPSC 8480 (Fall 2018)

> Books: Networks An Introduction, M.E.J. Newman
>        Network Analysis Methodological Foundations, Ulrik Brandes, Thomas Erlebach (Eds.)

### Mathematics of networks

* Adjacency matrix
* Weighted networks
* Directed networks
* Hypergraphs
* Bipartite networks
* Trees
* Degree
* Paths

### Measures and Metrics

* Degree centrality
* Eigenvector centrality
* Hubs and Authorities
* Closeness centrality
* Shortest paths are defined on vertices as well as on edges, stress and betweenness centrality of an element x (vertex  or edge),  are based on the (relative) number of shortest paths that contain x. They can be defined on weighted or unweighted and directed or undirected and simple or multi graphs.

#### Stress centrality

* based on enumeration of shortest paths.
* how much 'work' is done by each vertex in a communication network.
* counting the number of shortest path that contain an element x gives an approximation of the amount of 'work' or 'stress' the element has to sustain in the network.
* all shortest paths that either start or end in v are not accounted for this centrality index.
* not only calculates one shortest path but all shortest paths between any pair of vertices.

#### Betweenness centrality

* can be viewed as some kind of relative stress centrality.
* can be interpreted as the probability that vertex v is involved into any communication between s and t.
* betweenness centrality of a vertex measures the control over communication between others.
* It is problematic to apply the closeness centrality to a disconnected graph, the distance between two vertices in different components is usually set to infinity.
* Any pair of vertices s and t without any shortest path from s to t just will add zero to the betweennes centrality of every other vertex in the network.
* sum up the relative number of shortest paths for each pair of endvertices.

#### Traversal sets

* the edge's traversal set: the set of source-destination pairs where for every pair some shortest path contains this edge.
* the size of the traversal set would be an obvious measure for the importance of the edge.
* the traversal set graph is bipartite

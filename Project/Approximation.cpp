// CSE 6140 Final Project
// Traveling Salesman Problem
//
// Approximation.cpp

#include "Approximation.h"

// Constructor
Approximation::Approximation(string file_path, string algorithm, double time_cutoff, int seed) : TravelingSalesman(file_path, algorithm, time_cutoff, seed) {
    // Additional setup for Approximation can be added here if needed
}

// Function to solve the TSP with an approximation algorithm
void Approximation::solve() {
    int q; // Variable to store the current tour length
    int num_nodes = this->getSize(); // Get the number of nodes in the graph
    int min_path = INT_MAX; // Variable to store the minimum tour length found so far
    vector<int> min_path_nodes(num_nodes); // Vector to store nodes in the minimum tour
    
    // Generate a minimum spanning tree using Kruskal's algorithm
    kruskalMST();

    for (int i=1; i<=num_nodes; i++) {
        q = 0; // Initialize tour length for the current starting point
        DFS(i); // Perform depth-first search to construct a tour starting from node i
        this->tour.push_back(i); // Add the starting node to the tour

        // Calculate the tour length based on the constructed tour
        q = computeTourLength(this->getAdjacencyMatrix(), this->tour);

        // Update the minimum tour if the current tour is shorter
        if (q < min_path) {
            min_path = q;
            setQuality(q);
            setTour(this->tour);
        } 
    }
}

// Function to create disjoint sets
vector<int> Approximation::initializeParent(int n) {
    vector<int> parent(n); // Create a vector to store the parent of each element

    // Initialize each element as the representative of its own set
    for (int i=0; i<n; i++)
        parent[i] = i;
    return parent; // Return the initialized parent vector
}

// Function to find the root containing set u
int Approximation::find(int u, vector<int>& parent) {
    if (u != parent[u])
        parent[u] = find(parent[u], parent);
    return parent[u];
}

// Function to perform the union operation on two disjoint sets represented by their roots
void Approximation::unionSets(int u, int v, vector<int>& parent, vector<int>& rank) {
    int rootU = find(u, parent); // Find the root of the set containing u
    int rootV = find(v, parent); // Find the root of the set containing v

    // Check if u and v belong to different sets
    if (rootU != rootV) {
        // Attach the smaller rank tree to the larger rank tree
        if (rank[rootU] < rank[rootV])
            parent[rootU] = rootV;
        else if (rank[rootU] > rank[rootV])
            parent[rootV] = rootU;
        else {
            // If ranks are equal, arbitrarily choose rootU as the root and increment its rank
            parent[rootV] = rootU;
            rank[rootU]++;
        }
    }
}

// Function to check if a node already has been visited
bool Approximation::isVisited(int source) {
    for (int i=0; i<this->tour.size(); i++) {
        if (source == this->tour[i]) {
            return true;
        }
    }
    return false;
}

// Function to perform the depth-first search algorithm
void Approximation::DFS(int source) {
    vector<bool> visited(this->getSize(), false);
    stack<int> stack;

    // Clear the tour vector before starting DFS from a new source
    this->tour.clear();
    
    // Push the source node onto the stack
    stack.push(source);
    
    while (!stack.empty()) {
        // Pop a vertex from the stack and print it
        source = stack.top();
        stack.pop();
        
        // Skip if the vertex has already been visited
        if (visited[source]) {
            continue;
        }
        // Mark the vertex as visited and add it to the tour
        visited[source] = true;
        this->tour.push_back(source);

        // Get adjacent nodes from MST
        vector<Pair> nodes = this->mst[source - 1];
        
        // Iterate over adjacent nodes and push them onto the stack if not visited
        for (const Pair& node : nodes) {
            int neighbor = node.second;
            if (neighbor != 0 && !visited[neighbor]) {
                stack.push(neighbor);
            }
        }
    }
}

// Function to get the minimum-spanning tree using Kruskals algorithm
void Approximation::kruskalMST() {
    int size = this->getSize(); // Get the number of nodes in the graph
    vector<vector<int>> graph = this->getAdjacencyMatrix(); // Get the adjacency matrix
    vector<Edge> edges; // Vector to store edges sorted by weight

    // Convert the adjacency matrix to a vector of edges
    for (int i=0; i<size; ++i) {
        for (int j=i+1; j<size; j++) {
            if (graph[i][j] != 0) {
                edges.push_back(Edge(i, j, graph[i][j]));
            }
        }
    }
    // Sort edges in non-decreasing order based on weight
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        return a.weight < b.weight;
    });

    vector<int> parent = initializeParent(size); // Initialize disjoint sets parent array
    vector<int> rank(size, 0); // Initialize disjoint sets rank array
    vector<vector<Pair>> mst(size, vector<Pair>(size)); // Matrix to store the MST

    // Iterate through sorted edges
    for (const Edge &edge : edges) {
        int u = edge.pred; // Source vertex of the edge
        int v = edge.dest; // Destination vertex of the edge

        int rootPred = find(u, parent); // Find the root of the set containing u
        int rootDest = find(v, parent); // Find the root of the set containing v

        // Check if adding the edge creates a cycle in the MST
        if (rootPred != rootDest) {
            mst[u][v] = make_pair(edge.weight, v + 1); // Add edge to the MST
            mst[v][u] = make_pair(edge.weight, u + 1); // Add the reverse edge to the MST
            unionSets(rootPred, rootDest, parent, rank); // Union the sets of u and v
        }
    }
    // Output or store the MST as needed
    this->mst = mst;
}

// Function to print the minimum-spanning tree to the console
void Approximation::printMST(const vector<vector<Pair>>& mst) {
    int size = mst.size();
    cout << "Minimum Spanning Tree (MST):\n";
    for (int i=0; i<size; ++i) {
        for (int j=0; j<size; ++j) {
            if (mst[i][j].first != 0) {
                cout << "(" << i + 1 << " - " << mst[i][j].second << ") Weight: " << mst[i][j].first << "\n";
            }
        }
    }
}






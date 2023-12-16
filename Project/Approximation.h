#ifndef APPROXIMATION_H
#define APPROXIMATION_H

#include "TravelingSalesmanSolver.h"

// Define Pair and Edge types based on your needs
typedef pair<int, int> Pair;
class Edge {
public:
    int pred, dest, weight;

    Edge(int p, int d, int w) : pred(p), dest(d), weight(w) {}
};

class Approximation : public TravelingSalesman {
public:
    // Constructor
    Approximation(string file_path, string algorithm, double time_cutoff, int seed);
    
    // Main solving function
    void solve() override;

private:
    vector<vector<Pair> > mst;
    vector<int> tour;

    // Function to create disjoint sets
    vector<int> initializeParent(int n);

    // Function to find the root containing set u
    int find(int u, vector<int>& parent);

    // Function to perform the union operation on two disjoint sets represented by their roots
    void unionSets(int u, int v, vector<int>& parent, vector<int>& rank);

    // Function to check if a node already has been visited
    bool isVisited(int source);

    // Function to perform the depth-first search algorithm
    void DFS(int source);

    // Function to get the minimum-spanning tree using Kruskals algorithm
    void kruskalMST();

    // Function to print the minimum-spanning tree to the console
    void printMST(const vector<vector<Pair>>& mst);
};

#endif
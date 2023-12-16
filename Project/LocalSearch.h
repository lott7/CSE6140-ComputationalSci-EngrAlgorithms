#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "TravelingSalesmanSolver.h"

class LocalSearch : public TravelingSalesman {
public:
    // Constructor
    LocalSearch(string file_path, string algorithm, double time_cutoff, int seed);
    
    // Main solving function
    void solve() override;

private:
    // Function to perform the simulated annealing-based local search algorithm
    pair<vector<int>,int> runLocalSearch(vector<vector<int>> adj_matrix);

    // Function to generate a random tour by creating a vector of nodes from 1 to 'size'
    vector<int> select_random_tour(mt19937 engine);

    // Function to perform a random pair swap on the given tour by selecting two distinct indices and swapping the corresponding nodes
    vector<int> swap_random_pair(vector<int> tour, mt19937 engine);

    // Function to perform a random move on a given tour
    vector<int> move_random_node(vector<int> tour, mt19937 engine);
};

#endif
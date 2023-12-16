#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#include "TravelingSalesmanSolver.h"

class BruteForce : public TravelingSalesman {
public:
    // Constructor
    BruteForce(string file_path, string algorithm, double time_cutoff, int seed);

    // Main solving function
    void solve() override;

private:
    // Function to solve the traveling salesman problem with num_nodes <= 10 with the brute force algorithm
    pair<int, vector<int>> bruteForceTSP_10(const vector<vector<int>>& adj_matrix, int num_nodes, double time_constraint, unsigned seed);

    // Function to generate all possible permutations of node indices
    vector<vector<int>> generatePermutations(int num_nodes);

    // Function to shuffle permutations for randomized exploration 
    void shufflePermutations(vector<vector<int> >& permutations, unsigned seed);

    // Function to solve the traveling salesman problem with num_nodes > 10 with the brute force algorithm
    pair<int, vector<int>> bruteForceTSP_11(const vector<vector<int>>& adj_matrix, int num_nodes, double time_constraint, unsigned seed);

};

#endif

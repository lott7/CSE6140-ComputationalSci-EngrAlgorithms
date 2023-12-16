// CSE 6140 Final Project
// Traveling Salesman Problem
//
// BruteForce.cpp

#include "BruteForce.h"

// Constructor
BruteForce::BruteForce(string file_path, string algorithm, double time_cutoff, int seed) : TravelingSalesman(file_path, algorithm, time_cutoff, seed) {
    // Additional setup for BruteForce can be added here if needed
}

// Function to decide which BruteForce algorithm is utilized
void BruteForce::solve() {
    pair<int, vector<int>> sol;
    int num_nodes = this->getSize();
    if (num_nodes <= 10) {
        sol = bruteForceTSP_10(this->getAdjacencyMatrix(), num_nodes, this->getCutoff(), this->getSeed());
        setQuality(sol.first);
        setTour(sol.second);
    } else {
        sol = bruteForceTSP_11(this->getAdjacencyMatrix(), num_nodes, this->getCutoff(), this->getSeed());
        setQuality(sol.first);
        setTour(sol.second);
    }
}

// Function to solve the TSP with a Brute Force algorithm for small graphs (|V| <= 10)
pair<int, vector<int>> BruteForce::bruteForceTSP_10(const vector<vector<int>>& adj_matrix, int num_nodes, double time_constraint, unsigned seed) {
    // Generate all permutations
    vector<vector<int>> allPermutations = generatePermutations(num_nodes);

    // Shuffle the permutations with a specified seed
    shufflePermutations(allPermutations, seed);

    // Set an upper bound for the time in place (exit condition)
    double duration;
    auto start = high_resolution_clock::now();
    auto end = high_resolution_clock::now();
    
    // Store the minimum weight Hamiltonian Cycle and its vertices
    int min_path = INT_MAX;
    vector<int> min_path_nodes(num_nodes);

    // Counter to keep track of processed permutations
    int permutationCounter = 0;
   
    do {
        // Calculate the total weight of the current Hamiltonian Cycle
        int current_path = 0;       
        for (int j=0; j<num_nodes-1; j++) {
            int from = allPermutations[permutationCounter][j];
            int to = allPermutations[permutationCounter][j + 1];
            current_path += adj_matrix[from-1][to-1];
        }

        // Add the weight of the last edge to complete the cycle
        current_path += adj_matrix[allPermutations[permutationCounter][num_nodes - 1]-1][allPermutations[permutationCounter][0]-1];

        // Update the minimum path if the current path is smaller
        if (current_path < min_path) {
            min_path = current_path;
            min_path_nodes = allPermutations[permutationCounter];
        }
        
        // Increment the counter
        permutationCounter++;

        // Update the end time
        end = high_resolution_clock::now();

        // Calculate the elapsed time
        duration = duration_cast<seconds>(end - start).count();

    } while (duration < time_constraint && permutationCounter < allPermutations.size());

    // Add the last edge to complete the cycle
    min_path_nodes.push_back(min_path_nodes[0]);

    return {min_path, min_path_nodes};
}

// Function to generate all permutations of N nodes
vector<vector<int>> BruteForce::generatePermutations(int num_nodes) {
    // Create a vector with nodes [0, 1, ..., num_nodes-1]
    vector<int> nodes(num_nodes);
    for (int i=0; i<num_nodes; i++) {
        nodes[i] = i + 1;
    }
    // Initialize a vector to store all permutations
    vector<vector<int>> permutations;

    // Generate permutations and store them in the vector
    do {
        permutations.push_back(nodes);
    } while (next_permutation(nodes.begin(), nodes.end()));

    return permutations;
}

// Function to shuffle a vector of vectors with a specified seed
void BruteForce::shufflePermutations(vector<vector<int>>& permutations, unsigned seed) {
    // Shuffle the generated permutations to increase randomness
    mt19937 g(seed);
    shuffle(permutations.begin(), permutations.end(), g);
}

// Function to solve the TSP with a Brute Force algorithm for large graphs (|V| > 10)
pair<int, vector<int>> BruteForce::bruteForceTSP_11(const vector<vector<int>>& adj_matrix, int num_nodes, double time_constraint, unsigned seed) {
    // Generate an initial permutation
    vector<int> permutation;
    for (int i=0; i<num_nodes; i++)
        permutation.push_back(i + 1);

    // Use the provided seed for randomness
    mt19937 g(seed);

    // Set an upper bound for the time in place (exit condition)
    double duration;
    auto start = high_resolution_clock::now();
    auto end = high_resolution_clock::now();

    // Store the minimum weight Hamiltonian Cycle and its vertices
    int min_path = INT_MAX;
    vector<int> min_path_nodes(num_nodes);

    // Counter to keep track of processed permutations
    int permutationCounter = 0;

    // Total number of permutations (upper bound)
    double permutationUpperBound = tgamma(num_nodes + 1);

    do {
        // Randomize the order of vertices using the provided seed
        shuffle(permutation.begin(), permutation.end(), g);

        // Calculate the total weight of the current Hamiltonian Cycle
        int current_path = 0;      
        for (int j=0; j<num_nodes-1; j++) {
            int from = permutation[j];
            int to = permutation[j + 1];
            current_path += adj_matrix[from-1][to-1];
        }

        // Add the weight of the last edge to complete the cycle
        current_path += adj_matrix[permutation[num_nodes - 1]-1][permutation[0]-1];

        // Update the minimum path if the current path is smaller
        if (current_path < min_path) {
            min_path = current_path;
            min_path_nodes = permutation;
        }

        // Increment the counter
        permutationCounter++;

        // Update the end time
        end = high_resolution_clock::now();

        // Calculate the elapsed time
        duration = duration_cast<seconds>(end - start).count();

    } while (duration < time_constraint && permutationCounter < permutationUpperBound && next_permutation(permutation.begin(), permutation.end()));

    // Add the last edge to complete the cycle
    min_path_nodes.push_back(min_path_nodes[0]);

    return {min_path, min_path_nodes};
}
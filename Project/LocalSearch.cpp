// CSE 6140 Final Project
// Traveling Salesman Problem
//
// LocalSearch.cpp

#include "LocalSearch.h"

// Constructor
LocalSearch::LocalSearch(string file_path, string algorithm, double time_cutoff, int seed) : TravelingSalesman(file_path, algorithm, time_cutoff, seed) {
    // Additional setup for LocalSearch can be added here if needed
}

// Main solving function
void LocalSearch::solve() {
    // Run the local search algorithm and retrieve the best tour and quality
    pair<vector<int>,int> bestTour = runLocalSearch(getAdjacencyMatrix());

    // Set the tour and quality as the last found best tour and quality
    setTour(bestTour.first);
    setQuality(bestTour.second);
}

// Function to perform the simulated annealing-based local search algorithm
pair<vector<int>, int> LocalSearch::runLocalSearch(vector<vector<int>> adj_matrix) {
    using namespace std;
    using namespace chrono;

    // Parameters and constants for the local search algorithm
    double probSize = getSize();
    int nsteps = 10000000;
    double T = max(0.1, 0.016 * (probSize - 50) + 0.2);  // Increase T with problem size
    int M = 5;  // Lower temperature every M steps
    double coolingFactor = min(1.25e-5 * (probSize - 50) + 0.99995, 0.99999);  // Ratio to reduce temperature
    int maxCountNoChange = max(1000. * (probSize - 10 + 1), 1000.);  // Stop if no change for a certain amount of steps
    int nNewTours = 3;
    double swapPairProb = 0.9;  // Probability to swap pair over changing node

    // Higher number of maxCountNoChange for higher dimension problems
    int countNoChange = 0;

    // Declare variables to hold the current tour and its cost
    vector<int> tour;
    int costTour;

    // Declare variables to hold the best tour and its cost
    vector<int> bestTour;
    int costBestTour;

    int seed = getSeed();
    std::mt19937 engine(seed);  // RNG engine

    // Initialize the tour with a random permutation
    tour = select_random_tour(engine);
    costTour = computeTourLength(adj_matrix, tour);

    auto start = high_resolution_clock::now();  // Initialize chronometer
    int t = 0;
    double execTime;

    while ((t < nsteps) && (execTime < getCutoff()) && (countNoChange < maxCountNoChange)) {
        uniform_real_distribution<> uniform(0.0, 1.0);
        double r = uniform(engine);
        vector<int> newTour;

        // Decide whether to swap pair or move a random node
        if (r < swapPairProb) {
            newTour = swap_random_pair(tour, engine);
        } else {
            newTour = move_random_node(tour, engine);
        }

        // Add extra swaps at the beginning
        for (int otherswaps=0; otherswaps<=(nNewTours - t * 2. / maxCountNoChange); otherswaps++) {
            uniform_real_distribution<> uniform(0.0, 1.0);
            double r = uniform(engine);
            if (r < swapPairProb) {
                newTour = swap_random_pair(tour, engine);
            } else {
                newTour = move_random_node(tour, engine);
            }
        }

        int costNewTour = computeTourLength(adj_matrix, newTour);

        // Accept new tour if it has lower cost, else accept with a probability
        double deltaE = costNewTour - costTour;

        if (deltaE < 0) {
            tour = newTour;
            costTour = costNewTour;
            bestTour = tour;
            costBestTour = costTour;
            countNoChange = 0;
        } else {  // deltaE >= 0
            deltaE = deltaE / costTour;  // Normalize deltaE
            double prob = exp(-deltaE / (T));
            uniform_real_distribution<> uniform(0.0, 1.0);
            double r = uniform(engine);

            if (r < prob) {
                tour = newTour;
                costTour = costNewTour;

                // Update countNoChange based on cost comparison
                if (costNewTour == costTour) {
                    countNoChange++;
                } else {
                    countNoChange = 0;
                }
            } else {
                countNoChange++;
            }
        }

        // Decrease Temperature parameter to explore solution space more selectively
        if (t % M == 0) {
            T = T * coolingFactor;
        }
        t++;

        // Update execution time
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        execTime = duration.count() / 1e6;
    }

    // Return the best tour and its cost as a pair
    pair<vector<int>, int> tourCostPair = make_pair(bestTour, costBestTour);
    return tourCostPair;
}

// Function to generate a random tour by creating a vector of nodes from 1 to 'size'
vector<int> LocalSearch::select_random_tour(std::mt19937 engine) {
    int size = getSize();
    vector<int> path;

    // Create a vector of nodes from 1 to 'size'
    for (int v=1; v<size+1; v++)
        path.push_back(v);

    // Randomly shuffle the vector
    shuffle(path.begin(), path.end(), engine);

    // Connect the last node to the first to complete the tour
    path.push_back(path[0]);
    return path;
}

// Function to perform a random pair swap on the given tour by selecting two distinct indices and swapping the corresponding nodes
vector<int> LocalSearch::swap_random_pair(vector<int> tour, std::mt19937 engine) {   
    tour.pop_back();  // Ignore the last node (same as the first) to select 2 random indices
    
    // Generate two distinct random indices
    uniform_int_distribution<int> distribution(0, tour.size()-1);
    int idx1 = distribution(engine);
    int idx2 = distribution(engine);
    
    // Ensure idx2 is distinct from idx1
    while (idx2 == idx1)
        idx2 = distribution(engine);

    // Swap the nodes at idx1 and idx2 in the tour vector
    iter_swap(tour.begin() + idx1, tour.begin() + idx2);

    tour.push_back(tour[0]); // Add the last node back to maintain a closed tour
    return tour;
}

// Function to perform a random move on a given tour
vector<int> LocalSearch::move_random_node(vector<int> tour, std::mt19937 engine) {
    tour.pop_back();  // Ignore the last node (same as the first) to select 2 random indices
    
    // Generate two distinct random indices
    uniform_int_distribution<int> distribution(0, tour.size()-1);
    int idx1 = distribution(engine);
    int idx2 = distribution(engine);
    
    // Ensure idx2 is distinct from idx1
    while (idx2 == idx1)
        idx2 = distribution(engine);

    // Extract the node at idx1
    int nodeToMove = tour[idx1];

    // Erase the node at idx1
    tour.erase(tour.begin() + idx1);

    // Insert the node at idx2
    tour.insert(tour.begin() + idx2, nodeToMove);

    tour.push_back(tour[0]); // Add the last node back to maintain a closed tour
    return tour;
}
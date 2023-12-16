// CSE 6140 Final Project
// Traveling Salesman Problem
//
// main.cpp

#include "TravelingSalesmanSolver.h"
#include "BruteForce.h"
#include "Approximation.h"
#include "LocalSearch.h"

#include <chrono>
using namespace std;
using namespace std::chrono;

// Main program to solve the Traveling Salesman Problem (TSP) using one of the implemented algorithms.
//
// Inputs:
//   argc: Number of command-line arguments
//   argv: Array containing command-line parameters
//       argv[1]: Filepath to the dataset (e.g., data/Atlanta.tsp)
//       argv[2]: Algorithm used to solve the TSP (BF, APPROX, LS)
//       argv[3]: Cutoff time in seconds
//       argv[4]: Random seed
//
// Output:
//   Each method generates a solution file named "argv[1]_argv[2]_argv[3]_argv[4].sol"
//
int main(int argc, char** argv) {
    // Create the pointer to a solver
    TravelingSalesman* solver = nullptr;

    // Create the selected solver
    if (argc < 5) {
        cerr << "\033[1;33m"; // Set text color to orange
        cerr << "Insufficient number of arguments.\n";
        cerr << "Usage: " << argv[0] << " <filename> <method> <cutoff> <seed>\n";
        cerr << "Example: " << argv[0] << " ./data/Atlanta.tsp Approx 100 0\n";
        cerr << "\033[0m"; // Reset text color to default
        return 1;
    }

    if (strcmp(argv[2], "BF") == 0) {
        solver = new BruteForce(argv[1], argv[2], stod(argv[3]), stoi(argv[4]));
    } else if (strcmp(argv[2], "Approx") == 0) {
        solver = new Approximation(argv[1], argv[2], stod(argv[3]), stoi(argv[4]));
    } else if (strcmp(argv[2], "LS") == 0) {
        solver = new LocalSearch(argv[1], argv[2], stod(argv[3]), stoi(argv[4]));
    } else {
        cerr << "\033[1;33m"; // Set text color to orange
        cerr << "A correct algorithm name was not provided.\n";
        cerr << "Please select \"BF\", \"Approx\", or \"LS\".\n";
        cerr << "Example of input: " << argv[0] << " ./data/Atlanta.tsp Approx 100 0\n";
        cerr << "\033[0m"; // Reset text color to default
        return 1;
    }

    // Initialization for execution time measurement
    double execTime;

    // Solve the problem
    auto start = high_resolution_clock::now();
    solver->solve();
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start);
    execTime = duration.count() / 1e6;

    // Write solution and print results
    solver->writeSolution();
    cout << "Execution Time: " << fixed << setprecision(5) << execTime << " seconds" << endl;
    cout << "Quality: " << fixed << setprecision(7) << solver->getQuality() << endl;
    cout << "Tour: ";
    for (int node : solver->getTour()) {
        cout << node << " ";
    }
    cout << endl;

    // delete solver; // Cleanup: free the memory allocated for the solver

    return 0;
}

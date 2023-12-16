#ifndef TRAVELING_SALESMAN_SOLVER_H
#define TRAVELING_SALESMAN_SOLVER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip> 
#include <cstring>
#include <numeric>
#include <stack>
#include <climits>

using namespace std;
using namespace std::chrono;


class TravelingSalesman {
public:
    // Constructor
    TravelingSalesman(string file_path, string algorithm, double time_cutoff, int seed);
    TravelingSalesman() {};

    // Destructor
    ~TravelingSalesman();

    // Function to calculate the Euclidean distance given input variables
    int calculateEuclideanDistance(int x1, int x2, int y1, int y2);

    // Function to create an adjacency matrix
    void createAdjacencyMatrix();

    // Function to calculate the total tour length
    int computeTourLength(const vector<vector<int>>& adj_matrix, const vector<int>& tour);

    // Function to display the adjacency matrix
    void printAdjacencyMatrix(vector<vector<int>> adjMatrix);

    // Function to read the input data
    void readFile();

    // Function to write the information to the output file
    void writeSolution();

    // Setter functions
    void setInFile(string file_path) {
        this->in_file = file_path;
    }

    void setCity(string city_name) {
        this->city_name = city_name;
    }

    void setAlgorithm(string algorithm) {
        this->algorithm = algorithm;
    }

    void setCutoff(double time_cutoff) {
        this->cutoff = time_cutoff;
    }

    void setSeed(int seed) {
        this->seed = seed;
    }

    void setSize(int num_nodes) {
        this->num_nodes = num_nodes;
    }

    void setTour(vector<int> tour) {
        this->tour = tour;
    }
    
    void setQuality(double quality) {
        this->quality = quality;
    }

    void setOutFile() {
        this->out_file = "./output/" + this->getCity() + '_' + this->getAlgorithm() + '_' + to_string(static_cast<int>(this->getCutoff())) + '_' + to_string(this->getSeed()) + ".sol";
    }


    // Getter functions
    string getInFile() const {
        return this->in_file;
    }

    string getCity() {
        return this->city_name;
    }

    string getAlgorithm() {
        return this->algorithm;}

    double getCutoff() {
        return this->cutoff;
    }

    int getSeed() {
        return this->seed;
    }

    int getSize() {
        return this->num_nodes;
    }

    vector<int> getTour() {
        return this->tour;
    }
    
    double getQuality() {
        return this->quality;
    }

    vector<vector<int>> getAdjacencyMatrix() {
        return this->adjMatrix;
    }

    // Function to start the algorithms
    virtual void solve() = 0;


private:
    // Private member variables
    string in_file; // Input file path
    string out_file; // Output file path
    string city_name; // City name
    string algorithm; // Algorithm which is used
    int num_nodes; // Dimensions 
    int seed; // Random seed
    double cutoff; // Time constraints
    double quality; // Total cost (lenght) of the traveling route
    vector<int> ids; // IDs
    vector<int> xaxis; // X coordinates
    vector<int> yaxis; // Y coordinates
    vector<int> tour; // Travel route of the salesman
    vector<vector<int>> adjMatrix; // Adjacency matrix
};

#endif // TRAVELING_SALESMAN_H


// CSE 6140 Final Project
// Traveling Salesman Problem
//
// TravelingSalesmanSolver.cpp

#include "TravelingSalesmanSolver.h"

// Constructor
TravelingSalesman::TravelingSalesman(string file_path, string algorithm, double time_cutoff, int seed) {
    this->setInFile(file_path);
    this->setAlgorithm(algorithm);
    this->setCutoff(time_cutoff);
    this->setSeed(seed);
    this->readFile();
    this->createAdjacencyMatrix();
    this->setOutFile();
}

// Destructor
TravelingSalesman::~TravelingSalesman() {
    // Destructor implementation (if needed)
}

//Function to calculate the Euclidean distance given input variables
int TravelingSalesman::calculateEuclideanDistance(int x1, int x2, int y1, int y2) {
    double x = x1 - x2;
    double y = y1 - y2;
    double dist = sqrt(pow(x, 2) + pow(y, 2));
    return int(dist + 0.5);
}

// Function to read the input data
void TravelingSalesman::readFile() {
    ifstream input;
    input.open(this->getInFile()); // open .tsp file
    vector<int> ids, xaxis, yaxis; // vectors to store IDs, X coords, and Y coords
    vector<string> info; // vector to store general info about city
    string line;
    while (getline(input, line)) {
        if (isalpha(line[0])) { // if the first character is alphabetic, it's city info
            size_t div = line.find(' '); // find the first space character
            info.push_back(line.substr(div + 1)); // add content after the space character
        } else {
            stringstream linestream(line);
            string idx, x, y;
            if (linestream >> idx >> x >> y) {
                ids.push_back(stoi(idx)); // convert ID to int
                xaxis.push_back(stoi(x)); // convert X coord to int
                yaxis.push_back(stoi(y)); // convert Y coord to int
            }
        }
    }
    // Close the input file
    input.close();

    // store private variables
    this->city_name = info[0]; // store city name
    this->num_nodes= stoi(info[2]); // store dimensions
    this->ids = ids; // store IDs
    this->xaxis = xaxis; // store X coords
    this->yaxis = yaxis; // store Y coords
}

// Function to create an adjacency matrix
void TravelingSalesman::createAdjacencyMatrix() {
    vector<vector<int>> adj_matrix(this->num_nodes, vector<int>(this->num_nodes));
    long long int x1, x2, y1, y2;
    for (int i=0; i<this->num_nodes; i++) {    
        x1 = this->xaxis[i], y1 = this->yaxis[i];
        for (int j=0; j<this->num_nodes; j++) {
            x2 = this->xaxis[j], y2 = this->yaxis[j];
            if (i == j) {
                adj_matrix[i][j] = 0;
            } else {
                adj_matrix[i][j] = calculateEuclideanDistance(x1, x2, y1, y2);
            }
        }
    }
    this->adjMatrix = adj_matrix; // store adjMatrix as a private variable
}

// Function to write an output file
void TravelingSalesman::writeSolution() {
    try {
        ofstream outfile(this->out_file); // open file
        if (!outfile.is_open()) {
            throw runtime_error("Could not open file.");
        }
        outfile << fixed << this->quality << setprecision(7) << endl; // first line is tour quality
        for (int i = 0; i < this->tour.size(); i++) {
            outfile << this->tour[i]; // add location from tour
            if (i != this->tour.size() - 1) {outfile << ",";} // add comma except to last element
        }
        outfile.close();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl; // Log the error
    }
}

// Function to calculate the total tour length
int TravelingSalesman::computeTourLength(const vector<vector<int>>& adj_matrix, const vector<int>& tour) {
    int q = 0; 
    int num_nodes = tour.size();
 
    for (int i=0; i<num_nodes-1; i++) {
        int from = tour[i];
        int to = tour[i+1];
        q += adj_matrix[from-1][to-1];

    }
    return q;
}


// Function to display the adjacency matrix
void TravelingSalesman::printAdjacencyMatrix(vector<vector<int>> adjMatrix) {
    cout << "Adjacency Matrix:" << endl;
    for (int i=0; i<this->num_nodes; i++) {
        for (int j=0; j<this->num_nodes; j++) {
            cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }
}





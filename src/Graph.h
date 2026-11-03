#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// The edge values that go in the list
struct Edge {
    int neighborMovieID;
    double weight;
};

//holds movie information that goes in movies map
struct Movie {
    int movieID;
    string title;
    vector<string> genres;
    int fanCount = 0;
};

// graph implementation
class Graph {
private:
    // Adjacency list
    unordered_map<int, vector<Edge>> adjList;

    //maps handeling data from files
    unordered_map<int, Movie> movies;
    unordered_map<int, vector<int>> fanList;

public:
    // Constructor
    Graph() {}

    // Functions to load and build the graph
    void buildGraph();
    void loadMovies(const string& movieFile);
    void loadRatings(const string& ratingFile);

    // Functions to access movie connections
    //idk yet

    // Functions to retrieve movie information
    //idl yet
};
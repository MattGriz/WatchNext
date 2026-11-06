#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// The edge values that go in the list
struct Edge {
    int neighborMovieID;
    int sharedFans = 0;
    int sharedGenres = 0;
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

    // Functions to retrieve movie information
    const vector<Edge>& getNeighbors(int movieID) const;

    const Movie* getMovie(int movieID) const;

    int getMovieID(const string& title) const;
};
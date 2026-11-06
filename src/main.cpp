/*
WatchNext - A movie recommendation system that uses a graph data structure to recommend movies based on user ratings and movie connections.
Authors: Matthew Grizio, Ethan Bond, Nicolas Liway Avendano

*/
#include <iostream>
#include "Graph.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "ERROR|Usage: watchnext <title> <bfs|dijkstra>\n";
        return 0;
    }
    std::string title = argv[1];
    std::string algo  = argv[2];
    Graph movieGraph;

    movieGraph.loadMovies("data/movies.dat");
    movieGraph.loadRatings("data/ratings.dat");
    movieGraph.buildGraph();

    //recommender here:
    return 0;
}
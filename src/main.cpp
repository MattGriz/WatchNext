/*
WatchNext - A movie recommendation system that uses a graph data structure to recommend movies based on user ratings and movie connections.
Authors: Matthew Grizio, Ethan Bond, Nicolas Avendao 

*/
#include <iostream>
#include "Graph.h"

int main() {

    Graph movieGraph;

    movieGraph.loadMovies("data/movies.dat");
    movieGraph.loadRatings("data/ratings.dat");
    movieGraph.buildGraph();

    return 0;
}
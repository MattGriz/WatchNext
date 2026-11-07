#pragma once

#include "Graph.h"
#include <string>
#include <vector>
using namespace std;

// Information about one recommended movie
struct Recommendation {
    int movieID;
    double score;
    vector<int> path;
};

// Results and statistics from one algorithm run
struct RecommendationResult {
    vector<Recommendation> recommendations;
    double runtimeMs = 0.0;
    int nodesVisited = 0;
};

class Recommender {
private:
    const Graph& graph;

public:
    // Use the graph that main.cpp has already built
    Recommender(const Graph& graph);
    // Find recommendations using BFS
    RecommendationResult recommendBFS(
        int startMovieID,
        int count
    ) const;

    // Find recommendations using Dijkstra
    RecommendationResult recommendDijkstra(
        int startMovieID,
        int count
    ) const;
};

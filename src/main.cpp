/*
WatchNext - A movie recommendation system that uses a graph data structure to recommend movies based on user ratings and movie connections.
Authors: Matthew Grizio, Ethan Bond, Nicolas Liway Avendano

*/
#include <iostream>
#include "Graph.h"
#include "Recommender.h"

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
    int movieID = movieGraph.getMovieID(title);
    if(movieID == -1) {
        std::cout << "ERROR|Movie not found: " << title << "\n";
        return 0;
    }
    Recommender recommender(movieGraph);
    RecommendationResult result;
    if (algo == "bfs") result = recommender.recommendBFS(movieID, 10);
    else if (algo == "dijkstra") result = recommender.recommendDijkstra(movieID, 10);
    else{
        std::cout << "ERROR|Unknown " << algo << "\n";
        return 0;
    } 
    
    std::cout << "STATS|" << result.runtimeMs << "|" << result.nodesVisited << "\n";
    for (const Recommendation& rec : result.recommendations) {
        std::cout << movieGraph.getMovie(rec.movieID)->title << "|" << rec.score << "|";
        // path as "Title A -> Title B -> Title C"
        for (size_t i = 0; i < rec.path.size(); i++) {
            if (i > 0) std::cout << " -> ";
            std::cout << movieGraph.getMovie(rec.path[i])->title;
        }
        std::cout << "\n";
    }
    return 0;
}
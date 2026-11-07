
#include "Recommender.h"
#include "MinHeap.h"

#include <queue>
#include <unordered_map>
#include <chrono>
#include <algorithm>

Recommender::Recommender(const Graph& graph): graph(graph) {

}

RecommendationResult Recommender::recommendBFS(int startMovieID, int count) const {
    RecommendationResult result;
    queue<int> q;
    unordered_map<int, int> parent;
    unordered_map<int, int> distance;
    unordered_map<int, int> viaFans;
    if (graph.getMovie(startMovieID) == nullptr || count <= 0) {
        return result;
    }
    auto start = chrono::high_resolution_clock::now();
    q.push(startMovieID);
    distance[startMovieID] = 0;
    while(!q.empty()){
        int currentMovie = q.front();
        q.pop();
        result.nodesVisited++;
        const vector<Edge>& neighbors = graph.getNeighbors(currentMovie);
        for(const Edge& edge : neighbors){
            int nID = edge.neighborMovieID;
            if(distance.find(nID) != distance.end()) continue;
            parent[nID] = currentMovie;
            viaFans[nID] = edge.sharedFans;
            distance[nID] = distance[currentMovie] + 1;
            q.push(nID);
        }
    }
    vector<Recommendation> candidates;
    for(const auto& entry: distance){
        int curr_id = entry.first;
        if(curr_id == startMovieID) continue;
        Recommendation rec{curr_id, static_cast<double>(viaFans.at(curr_id)), {}};
        candidates.push_back(rec);

    }

    // fewest hops first, then most shared fans, then lowest ID
    sort(candidates.begin(), candidates.end(),
        [&](const Recommendation& a, const Recommendation& b) {
            int distA = distance.at(a.movieID);
            int distB = distance.at(b.movieID);
            if (distA != distB) return distA < distB;
            if (a.score != b.score) return a.score > b.score;
            return a.movieID < b.movieID;
        });
    if((int)candidates.size() > count){
        candidates.resize(count);
    }

    for(Recommendation& rec : candidates){
        int cur = rec.movieID;
        while(cur != startMovieID){
            rec.path.push_back(cur);
            cur = parent.at(cur);
        }
        rec.path.push_back(startMovieID);
        reverse(rec.path.begin(), rec.path.end());
    }
    result.recommendations = candidates;
    auto end = chrono::high_resolution_clock::now();
    result.runtimeMs = chrono::duration<double, milli>(end-start).count();
    return result;
}

RecommendationResult Recommender::recommendDijkstra(
    int startMovieID,
    int count
) const {
    RecommendationResult result;

    // 1. Start runtime timer
    // 2. Initialize MinHeap and distance map
    // 3. Traverse graph using graph.getNeighbors()
    // 4. Relax edges using edge.weight
    // 5. Rank movies by shortest-path distance
    // 6. Reconstruct paths
    // 7. Record runtime and nodes visited

    return result;
}

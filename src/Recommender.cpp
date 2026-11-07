
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

RecommendationResult Recommender::recommendDijkstra(int startMovieID, int count) const {
    RecommendationResult result;
    result.nodesVisited = 0;

    // make sure the movie actually exists
    if (graph.getMovie(startMovieID) == nullptr || count <= 0) {
        return result;
    }

    auto start = chrono::high_resolution_clock::now();

    //track the lowest distance to each movie and where we came from
    unordered_map<int, double> distance;
    unordered_map<int, int> parent;
    
    // plug in cusotm min-heap
    MinHeap pq;

    // setup start node
    distance[startMovieID] = 0.0;
    pq.push(startMovieID, 0.0);

    while (!pq.isEmpty()) {
        HeapNode current = pq.extractMin();
        int u = current.movieID;
        double d = current.distance;

        // lazy deletion
        if (distance.find(u) != distance.end() && d > distance[u]) {
            continue;
        }

        result.nodesVisited++;

        // check out all the neighbors
        for (const Edge& edge : graph.getNeighbors(u)) {
            int v = edge.neighborMovieID;
            double newDist = d + edge.weight;

            // if its a brand new movie or we found a shorter path to it
            if (distance.find(v) == distance.end() || newDist < distance[v]) {
                distance[v] = newDist;
                parent[v] = u;
                pq.push(v, newDist);
            }
        }
    }

    //gather all the movies we reached 
    vector<Recommendation> candidates;
    for (const auto& entry : distance) {
        int movieID = entry.first;
        if (movieID == startMovieID) continue; 
        
        // save the movie and its total path cost
        candidates.push_back({movieID, entry.second, {}});
    }

    // sort by the lowest distance cost first, then by ID to break ties
    sort(candidates.begin(), candidates.end(), [&](const Recommendation& a, const Recommendation& b) {
        if (a.score != b.score) return a.score < b.score; 
        return a.movieID < b.movieID;
    });

    // chop off the list so we only keep the top 'count'
    if ((int)candidates.size() > count) {
        candidates.resize(count);
    }

    // trace back the path for the winning movies
    for (Recommendation& rec : candidates) {
        int cur = rec.movieID;
        while (cur != startMovieID) {
            rec.path.push_back(cur);
            cur = parent.at(cur);
        }
        rec.path.push_back(startMovieID);
        reverse(rec.path.begin(), rec.path.end()); // flip it so it reads start -> end
        
        result.recommendations.push_back(rec);
    }

    //stop the clock
    auto end = chrono::high_resolution_clock::now();
    result.executionTimeMs = chrono::duration<double, std::milli>(end - start).count();

    return result;
}

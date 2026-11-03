#include "Graph.h"

Graph::Graph() {}

void Graph::loadMovies(const string& movieFile) {
    // creates the map of the movies
    ifstream file(movieFile);
    string line;

    while (getline(file, line)) {
        size_t position = line.find("::");
        string movieIDStr = line.substr(0, position);
        size_t nextPosition = line.find("::", position + 2);

        string title = line.substr(position + 2, nextPosition - position - 2);
        string genresStr = line.substr(nextPosition + 2);

        int movieID = stoi(movieIDStr);

        vector<string> genres;
        string genre;
        stringstream genreStream(genresStr);

        while (getline(genreStream, genre, '|')) {
            genres.push_back(genre);
        }

        movies[movieID] = {movieID, title, genres, 0};
    }
}

void Graph::loadRatings(const string& ratingFile) {
    // creates the map of the ratings
    ifstream file(ratingFile);
    string line;

    while (getline(file, line)) {
        size_t position = line.find("::");
        string userIDStr = line.substr(0, position);
        size_t nextPosition = line.find("::", position + 2);

        string movieIDStr = line.substr(position + 2, nextPosition - position - 2);
        string ratingStr = line.substr(nextPosition + 2, 1);

        // only considers ratings of 4 or higher
        //appends a fanList and updates fan counts 
        if (stoi(ratingStr) >= 4) {
            int userID = stoi(userIDStr);
            int movieID = stoi(movieIDStr);
            if (movies.find(movieID) != movies.end()) {
                fanList[userID].push_back(movieID);
                movies[movieID].fanCount++;
            }
        }        
    }
}

void Graph::buildGraph() {
    // Implementation for building the graph from movie and rating files
    //hello
}
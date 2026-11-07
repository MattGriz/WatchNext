#include "Graph.h"
#include <algorithm>



void Graph::loadMovies(const string& movieFile) {
    // creates the map of the movies
    ifstream file(movieFile);
    string line;

    while (getline(file, line)) {
        unsigned int position = line.find("::");
        string movieIDStr = line.substr(0, position);
        unsigned int nextPosition = line.find("::", position + 2);

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
        unsigned int position = line.find("::");
        string userIDStr = line.substr(0, position);
        unsigned int nextPosition = line.find("::", position + 2);

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

    // Store the number of shared fans for each movie pair
    map<pair<int, int>, int> sharedFanCounts;
    for (const auto& user : fanList) {

        const vector<int>& userMovies = user.second;

        for (unsigned int i = 0; i < userMovies.size(); ++i) {
            for (unsigned int j = i + 1; j < userMovies.size(); ++j) {
                // Make pairs for the movies and increment shared fan count
                int movieA = userMovies[i];
                int movieB = userMovies[j];

                int first = min(movieA, movieB);
                int second = max(movieA, movieB);

                sharedFanCounts[{first, second}]++;
            }
        }
    }

    // Build  adjacency list / graph !!!!! finally
    adjList.clear();

    for (const auto& entry : sharedFanCounts) {

        // get all movie information
        int movieA = entry.first.first;
        int movieB = entry.first.second;
        int sharedFans = entry.second;

        const Movie& A = movies.at(movieA);
        const Movie& B = movies.at(movieB);

        // handle genre counts
        int sharedGenres = 0;

        const vector<string>& genresA = A.genres;
        const vector<string>& genresB = B.genres;

        for (const string& genre : genresA) {
            if (find(genresB.begin(), genresB.end(), genre) != genresB.end()) {
                sharedGenres++;
            }
        }

        // IMPORTANT: IF THEY DO NOT SHARE A GENRE, THEY DO NOT CONNECT
        if (sharedGenres == 0) continue;

        // Calulate weights: 1. standardize fan counts, 2. standardize genre count, 3. combine
        double fanStandard = static_cast<double>(sharedFans) / (A.fanCount + B.fanCount - sharedFans);
        double genreStandard = static_cast<double>(sharedGenres) / (genresA.size() + genresB.size() - sharedGenres);
        double standard = 0.7 * fanStandard + 0.3 * genreStandard;

        // calulate the weight
        double weight = 1.0 - standard;

        // ACTUALLy create the adjacency list
        Edge edgeAB{movieB, sharedFans, sharedGenres, weight};
        Edge edgeBA{movieA, sharedFans, sharedGenres, weight};

        adjList[movieA].push_back(edgeAB);
        adjList[movieB].push_back(edgeBA);
    }
}

    const vector<Edge>& Graph::getNeighbors(int movieID) const {
        if (adjList.find(movieID) != adjList.end()) {
            return adjList.at(movieID);
        } else {
            static const vector<Edge> empty;
            return empty;
        }
    }

    const Movie* Graph::getMovie(int movieID) const {
        if (movies.find(movieID) != movies.end()) {
            return &movies.at(movieID);
        } else {
            return nullptr;
        }
    }

    int Graph::getMovieID(const string& title) const {
        for (const auto& entry : movies) {
            if (entry.second.title == title) {
                return entry.first;
            }
        }
        return -1; 
    }
    //hello

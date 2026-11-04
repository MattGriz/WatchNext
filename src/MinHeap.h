#pragma once
#include <vector>
#include <stdexcept>

// simple struct to hold the movie and its current path distance
struct HeapNode {
    int movieID;
    double distance;
};

class MinHeap {
private:
    std::vector<HeapNode> heap;

    // quick index helpers
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    // bubble up when inserting
    void heapifyUp(int index);

    // bubble down after popping root
    void heapifyDown(int index);

public:
    MinHeap();

    // push a new movie node with its distance
    void push(int movieID, double distance);

    // remove and return the node with the smallest distance
    HeapNode extractMin();

    // check what's at the top
    HeapNode peek() const;

    bool isEmpty() const;
    int size() const;
};

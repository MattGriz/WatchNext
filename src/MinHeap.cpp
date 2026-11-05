#include "MinHeap.h"

MinHeap::MinHeap() {}

void MinHeap::heapifyUp(int index) {
    // keep swapping with parent if our distance is smaller
    while (index > 0 && heap[index].distance < heap[parent(index)].distance) {
        std::swap(heap[index], heap[parent(index)]);
        index = parent(index);
    }
}

void MinHeap::heapifyDown(int index) {
    int smallest = index;
    int left = leftChild(index);
    int right = rightChild(index);

    // see if left child has a smaller distance
    if (left < (int)heap.size() && heap[left].distance < heap[smallest].distance) {
        smallest = left;
    }

    // see if right child is even smaller
    if (right < (int)heap.size() && heap[right].distance < heap[smallest].distance) {
        smallest = right;
    }

    // if one of the kids was smaller, swap and keep bubbling down
    if (smallest != index) {
        std::swap(heap[index], heap[smallest]);
        heapifyDown(smallest);
    }
}
void MinHeap::push(int movieID, double distance) {
    // stick it at the very end then let it float up
    heap.push_back({movieID, distance});
    heapifyUp(heap.size() - 1);
}

HeapNode MinHeap::extractMin() {
    if (heap.empty()) {
        throw std::runtime_error("tried to pop from an empty heap");
    }

    // root is always the smallest
    HeapNode minNode = heap[0];

    // move the last element to the top and shrink the vector
    heap[0] = heap.back();
    heap.pop_back();

    // restore the min-heap property if there's still stuff left
    if (!heap.empty()) {
        heapifyDown(0);
    }

    return minNode;
}

HeapNode MinHeap::peek() const {
    if (heap.empty()) {
        throw std::runtime_error("heap is empty");
    }
    return heap[0];
}

bool MinHeap::isEmpty() const {
    return heap.empty();
}

int MinHeap::size() const {
    return heap.size();
}

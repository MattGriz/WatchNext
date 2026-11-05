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

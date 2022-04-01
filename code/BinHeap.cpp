#include "BinHeap.h"

void BinHeap::Insert(const int& key, const int &val) {
    BinNode X;
    X.dist = key;
    X.vertex = val;

    // percolate up
    int i;
    for (i = ++this->size; heap->at(i / 2).dist > X.dist; i /= 2) {
        indexMapping[heap->at(i / 2).vertex] = i;
        heap->at(i) = heap->at(i / 2);
    }
    indexMapping[X.vertex] = i; // update the mapping of index
    heap->at(i) = X;
}

int BinHeap::DeleteMin() {
    int i, child;
    BinNode deleted, last;

    // empty
    if (this->size == 0) return 0;

    deleted = heap->at(1); // save min BinNode
    indexMapping.erase(deleted.vertex); // erase the mapping of the min node
    last = heap->at(this->size--); // take last and reset size

    // percolate down
    for (i = 1; i * 2 <= this->size; i = child) {
        child = 2 * i;
        // find smaller child
        if (child != this->size && heap->at(child + 1).dist < heap->at(child).dist) {
            child++;
        }
        if (last.dist > heap->at(child).dist) { // find proper position
            indexMapping[heap->at(child).vertex] = i;
            heap->at(i) = heap->at(child);
        }
        else break;
    }
    indexMapping[last.vertex] = i;
    heap->at(i) = last;

    return deleted.vertex;
}

void BinHeap::DecreaseKey(const int& pos, const int& newKey) {

    int vRestrore = heap->at(pos).vertex; // restore vertex
    int i;
    // percolate up
    for (i = pos; heap->at(i / 2).dist > newKey; i /= 2) {
        indexMapping[heap->at(i / 2).vertex] = i;
        heap->at(i) = heap->at(i / 2);
    }
    indexMapping[vRestrore] = i;
    heap->at(i).dist = newKey;
    heap->at(i).vertex = vRestrore;
}

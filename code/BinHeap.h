#ifndef SHORTESTPATH_BINHEAP_H
#define SHORTESTPATH_BINHEAP_H

#include "Graph.h"
#include <vector>
#include <unordered_map>

using namespace std;

class BinNode {
public:
    int dist, vertex;
};

class BinHeap {
private:
    vector<BinNode> *heap;
    int size;
private:
    BinHeap(); // not allowed

public:
    unordered_map<int, int> indexMapping; // direct entry in heap

    BinHeap(int capacity) {
        heap = new vector<BinNode>(capacity + 1);
        BinNode dummyHead;
        dummyHead.dist = dummyHead.vertex = -1;
        heap->at(0) = dummyHead;
        size = 0;
    }

    ~BinHeap() { delete heap; }

    void Insert(const int &key, const int &val);

    int DeleteMin();

    void DecreaseKey(const int &pos, const int &newKey);

    int Size() { return size; }
};

#endif //SHORTESTPATH_BINHEAP_H

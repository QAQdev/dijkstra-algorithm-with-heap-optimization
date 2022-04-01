#ifndef SHORTESTPATH_FIBONACCIHEAP_H
#define SHORTESTPATH_FIBONACCIHEAP_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;

class FibNode {
public:
    FibNode *leftSibling, *rightSibling;
    FibNode *parent, *firstChild;
    bool mark;
    int dist; // key
    int vertex; // vertex id
    int degree;
public:
    FibNode() {
        leftSibling = rightSibling = nullptr;
        parent = firstChild = nullptr;
        degree = 0;
        mark = false;
    }
};

class FibHeap {
private:
    FibNode *min;
    int size;
private:
    void Insert2RootList(FibNode *x);

    void InsertHelper(FibNode*x);

    void MergeRootList(FibNode *x, FibNode *y);

    void RemoveFromList(FibNode *x);

    void AdjustHeap();

    void FibonacciLink(FibNode *x, FibNode *y);

    void Cut(FibNode *x, FibNode *par);

    void CascadeCut(FibNode *x);

public:
    unordered_map<int, FibNode *> directEntryInHeap;

    FibHeap() {
        min = nullptr;
        size = 0;
    }

    ~FibHeap() = default;

    void Insert(const int &x, const int &y);

    FibHeap *Union(FibHeap *H);

    FibNode *DeleteMin();

    void DecreaseKey(FibNode *x, const int &y);

    int Size() const { return size; }
};

#endif //SHORTESTPATH_FIBONACCIHEAP_H

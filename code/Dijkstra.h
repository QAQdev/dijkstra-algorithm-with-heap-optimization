#ifndef SHORTESTPATH_DIJKSTRA_H
#define SHORTESTPATH_DIJKSTRA_H

#include "Graph.h"
#include "FibonacciHeap.h"
#include "BinHeap.h"
#define NOT_VERTEX -1


extern ProgBar mainProgress;
extern bool hasInit;

class Dijkstra {
    struct TableEntry {
        bool visited;
        int dist;
        int path;
    };

private:
    unordered_map<int, TableEntry *> Table;
private:
    int FindSmallestUnvisited();

public:
    Dijkstra() = default;

    ~Dijkstra() = default;

    void InitTable(const Graph *graph);

    static bool ExistenceCheck(const int &point, const Graph *graph);

    void OrdinaryDij(const int &start, const Graph *graph);

    void BinHeapDij(const int &start, const Graph *graph);

    void FibonacciHeapDij(const int &start, const Graph *graph);

    void PrintPath(const int &start, const int &dest);
};

#endif //SHORTESTPATH_DIJKSTRA_H

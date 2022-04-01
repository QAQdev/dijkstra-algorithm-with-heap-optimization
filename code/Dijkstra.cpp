#include "Dijkstra.h"

// private functions
int Dijkstra::FindSmallestUnvisited() {
    // used in ordinary dijkstra to find the smallest unknown vertex
    // linear search
    int min = INT_MAX, minID = NOT_VERTEX;
    for (auto &itr: Table) {
        if (!itr.second->visited && min > itr.second->dist) {
            min = itr.second->dist;
            minID = itr.first;
        }
    }
    return minID;
}

// public functions
void Dijkstra::InitTable(const Graph*graph) {
    // initialize table
    for (auto &itr: *graph->array) {
        if (!hasInit) { // first use, allocate memory
            TableEntry *pEntry;
            pEntry = new TableEntry{false, INT_MAX, NOT_VERTEX};
            Table.insert(pair<int, TableEntry *>(itr.first, pEntry));
        }
        else { // has initialized, just reset
            Table[itr.first]->dist = INT_MAX;
            Table[itr.first]->visited = false;
            Table[itr.first]->path = NOT_VERTEX;
        }
    }
    hasInit = true;
}

bool Dijkstra::ExistenceCheck(const int &point,const Graph*graph) {
    // check if point exists in current connected component
    if (graph->array->find(point) == graph->array->end()) return false;
    else return true;
}


void Dijkstra::OrdinaryDij(const int &start, const Graph*graph) {
    // dijkstra algorithm without heap optimization
    int v, w, progressed = 0;
    double ratio;
    list<AdjListNode>::iterator itr;

    Table[start]->dist = 0;
    while (true) {
        v = FindSmallestUnvisited(); // find smallest unvisited
        if (v == NOT_VERTEX) break;
        Table[v]->visited = true;

        mainProgress.ProgressBar(++progressed, Table.size());

        for (itr = graph->array->at(v)->head->begin(); itr != graph->array->at(v)->head->end(); itr++) {
            w = itr->end;
            if (!Table[w]->visited) {
                if (Table[v]->dist + itr->weight < Table[w]->dist) {
                    Table[w]->dist = Table[v]->dist + itr->weight; // update
                    Table[w]->path = v;
                }
            }
        }
    }
}

void Dijkstra::BinHeapDij(const int& start,const Graph*graph) {
    // dijkstra algorithm using binary heap optimization

    Table[start]->dist = 0;
    BinHeap *binHeap = new BinHeap(graph->nV); // binary heap
    for (auto &itr: Table) { //initial heap
        binHeap->Insert(itr.second->dist, itr.first);
    }

    int v, w;
    list<AdjListNode>::iterator itr;

    while (binHeap->Size() > 0) {
        v = binHeap->DeleteMin();
        Table[v]->visited = true;

        mainProgress.ProgressBar(Table.size() - binHeap->Size(), Table.size());

        for (itr = graph->array->at(v)->head->begin(); itr != graph->array->at(v)->head->end(); itr++) {
            w = itr->end;
            if (!Table[w]->visited) {
                if (Table[v]->dist + itr->weight < Table[w]->dist) {
                    binHeap->DecreaseKey(binHeap->indexMapping[w],
                                         Table[v]->dist + itr->weight); // decrease_key as update
                    Table[w]->dist = Table[v]->dist + itr->weight;
                    Table[w]->path = v;
                }
            }
        }
    }
}

void Dijkstra::FibonacciHeapDij(const int &start,const Graph*graph) {
    // dijkstra algorithm using fibonacci heap optimization

    Table[start]->dist = 0;
    FibHeap *fibHeap = new FibHeap;// fibonacci heap
    for (auto &itr: Table) { //initial heap
        fibHeap->Insert(itr.second->dist, itr.first);
    }

    FibNode *V;
    int v, w;
    list<AdjListNode>::iterator itr;

    while (fibHeap->Size() > 0) {
        V = fibHeap->DeleteMin(); // get smallest unvisited
        v = V->vertex;
        Table[v]->visited = true;

        mainProgress.ProgressBar(Table.size() - fibHeap->Size(), Table.size());

        for (itr = graph->array->at(v)->head->begin(); itr != graph->array->at(v)->head->end(); itr++) {
            w = itr->end;
            if (!Table[w]->visited) {
                if (Table[v]->dist + itr->weight < Table[w]->dist) {
                    fibHeap->DecreaseKey(fibHeap->directEntryInHeap[w],
                                         Table[v]->dist + itr->weight); // decrease_key as update
                    Table[w]->dist = Table[v]->dist + itr->weight;
                    Table[w]->path = v;
                }
            }
        }
    }
}

void Dijkstra::PrintPath(const int &start, const int &dest) {
    // print the path from start to the dest
    int v;
    cout << "The pathLen is " << Table[dest]->dist << ", path from " << start << " to " << dest << " is:\n";
    for (v = dest; v != start; v = Table[v]->path) {
        cout << v << " <- ";
    }
    cout << v << endl;
}

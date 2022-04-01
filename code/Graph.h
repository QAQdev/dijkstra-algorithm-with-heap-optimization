#ifndef SHORTESTPATH_GRAPH_H
#define SHORTESTPATH_GRAPH_H

#include <map>
#include <windows.h>
#include <list>
#include <string>
#include <chrono>
#include <fstream>
#include <iostream>

#define PBSTR "############################################################"
#define PBWIDTH 60

using namespace std;

struct AdjListNode {
    int end, weight;
};

struct AdjList {
    list<AdjListNode> *head;

    AdjList() { head = new list<AdjListNode>; }

    ~AdjList() { delete head; }
};

class Graph {
public:
    int nV;
    map<int, AdjList *> *array;
public:
    Graph() { array = new map<int, AdjList *>; }

    ~Graph() { delete array; }

    void ReadGraph();
	void ReadGraphTest(ifstream &fp, ofstream &dat);
};

class ProgBar {
public:
	int ProgressCount;
	int ProgressTotal;
public:
	void ResetProgress(int segTotal);
	void PrintProgress(double percentage);
	void ProgressBar(int current, int total);
};

extern ProgBar mainProgress;

#endif //SHORTESTPATH_GRAPH_H
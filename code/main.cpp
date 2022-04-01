#include <iostream>
#include <chrono>
#include "Graph.h"
#include "Dijkstra.h"

//#define __TEST_MODE__

using namespace std;

ProgBar mainProgress;
bool hasInit = false;

/*
 * -1 means NOT_VERTEX, which is used to exit query
 */
void Query(int &src, int &dest, Dijkstra &d, Graph *graph ) {
    cout << "Please input your query( dest ): ";
    cin >> dest;
    while (dest != NOT_VERTEX) {
        while (dest != NOT_VERTEX && !Dijkstra::ExistenceCheck(dest, graph)) {
            cout << "This point does not exist in this connected component, try again\n"
                 << "Please input your query( dest ): ";
            cin >> dest;
        }
        if (dest == NOT_VERTEX) break;
        d.PrintPath(src, dest);
        cout << "Please input your query( dest ): ";
        cin >> dest;
    }
}

//#ifndef __TEST_MODE__
int main() {

    cout << "\n"
            "   ____   __       __  ___          ____   _    _  __          __              \n"
            "  / __ \\ / /_     /  |/  /__  __   / __ \\ (_)  (_)/ /__ _____ / /_ _____ ____ _\n"
            " / / / // __ \\   / /|_/ // / / /  / / / // /  / // //_// ___// __// ___// __ `/\n"
            "/ /_/ // / / /  / /  / // /_/ /  / /_/ // /  / // , < (__  )/ /_ / /   / /_/ / \n"
            "\\____//_/ /_/  /_/  /_/ \\__, /  /_____//_/__/ //_/|_|/____/ \\__//_/    \\__,_/  \n"
            "                       /____/            /___/                                 \n\n";

    Graph *graph = new Graph;
    graph->ReadGraph(); // read graph

    Dijkstra d; // dijkstra algorithm
    chrono::high_resolution_clock::time_point startTime, endTime;
    chrono::duration<double, ratio<1, 1000>> fp_ms;
    int src, dest; // query
    cout << "Please input your query( src ): ";
    cin >> src;
    while (src != NOT_VERTEX) {
        // check if vertex exists in current file
        if (src != NOT_VERTEX && !Dijkstra::ExistenceCheck(src, graph)) {
            cout << "This point does not exist in this connected component, try again\n"
                 << "Please input your query( src ): ";
            cin >> src;
        }
        if (src == NOT_VERTEX) break;

        cout << "Start to do Binary_Heap_Dijkstra\n";
        // binary heap optimization
        d.InitTable(graph); // initialize dij table
	    mainProgress.ResetProgress(100);
        startTime = chrono::high_resolution_clock::now();
        d.BinHeapDij(src, graph);
        endTime = chrono::high_resolution_clock::now();
        fp_ms = endTime - startTime;
        cout << "Binary_Heap_Dijkstra cost: " << fp_ms.count() << "ms" << endl;
        Query(src, dest, d, graph);  // query for path

        cout << "Start to do Fibonacci_Heap_Dijkstra\n";

        // fibonacci heap optimization
        d.InitTable(graph);
	    mainProgress.ResetProgress(100);
        startTime = chrono::high_resolution_clock::now();
        d.FibonacciHeapDij(src, graph);
        endTime = chrono::high_resolution_clock::now();
        fp_ms = endTime - startTime;
        cout << "Fibonacci_Heap_Dijkstra cost: " << fp_ms.count() << "ms" << endl;
        Query(src, dest, d, graph); // query for path

        // if you want to try ordinary dijkstra, just remove comments, but DO NOT try large cases
//        cout << "Start to do Ordinary_Dijkstra\n";
//        // ordinary dijkstra
//        d.InitTable(graph);
//        mainProgress.ResetProgress(100);
//        startTime = chrono::high_resolution_clock::now();
//        d.OrdinaryDij(src, graph);
//        endTime = chrono::high_resolution_clock::now();
//        fp_ms = endTime - startTime;
//        cout << "Ordinary_Dijkstra cost: " << fp_ms.count() << "ms" << endl;
//        Query(src, dest, d, graph); // query for path

        cout << "Please input your query( src ): ";
        cin >> src;
    }
    return 0;
}

//#endif

/*
 * TEST MODE BELOW !!!
 */

/*
#ifdef __TEST_MODE__

char testDataList[12][36] = {"../USA-road-d.BAY.gr","../USA-road-d.CAL.gr","../USA-road-d.COL.gr",
                             "../USA-road-d.CTR.gr","../USA-road-d.E.gr","../USA-road-d.FLA.gr",
                             "../USA-road-d.LKS.gr","../USA-road-d.NE.gr","../USA-road-d.NW.gr",
                             "../USA-road-d.NY.gr","../USA-road-d.USA.gr","../USA-road-d.W.gr"};

int main(){
	Graph *graph = new Graph;
	Dijkstra *d = new Dijkstra; // dij class for performing with

	ofstream fib("Fib.txt", ios::app);  // appending, record more data points.
	ofstream bin("Bin.txt", ios::app);
	ofstream ord("Ord.txt", ios::app);
	ofstream read("Read.txt", ios::app);
	chrono::high_resolution_clock::time_point startTime, endTime;
	chrono::duration<double, ratio<1, 1000>> fp_ms;
	if(fib.is_open()&&bin.is_open()&&ord.is_open()&&read.is_open()){
		cout << "Journal Ready." << endl;
	}
	else{
		cout << "Journal Error. Test Terminated." << endl;
		return 0;
	}

	// traverse all 12 testing files
	for(int testNo=0; testNo < 12; testNo++){
//		if(testNo != 0 && testNo != 2 && testNo != 9){      // read only the small files
//			continue;
//		}
// 		if(testNo != 0){      // read only file #3
//			continue;
//		}
		free(graph);    // reconstruct the two items to avoid crushes
		graph = new Graph;
		free(d);
		d = new Dijkstra;
		hasInit = false;
		ifstream src(testDataList[testNo]);
		if(src.is_open()){
			cout << "Test file no." << testNo+1 << ' ' << testDataList[testNo] << " Opened Successfully." << endl;
		}
		else{
			cout << "Could not open file " << testDataList[testNo] << endl;
			continue;
		}
		graph->ReadGraphTest(src, read); // read graph for test and save arcs read
		src.close();
		for(int caseNo=0; caseNo < 1; caseNo++) {
			cout << "*** FILE NO " << testNo+1 << ", CASE NO " << caseNo+1 << " ***" << endl;
			int srcVertex, terVertex;
			d->InitTable(graph);
			srcVertex = rand()%(graph->nV);
			while(!(d->ExistenceCheck(srcVertex, graph))){
				srcVertex = rand()%(graph->nV);
			}
			terVertex = rand()%(graph->nV);
			while(!(d->ExistenceCheck(terVertex, graph))){
				terVertex = rand()%(graph->nV);
			}

			d->InitTable(graph);
			startTime = chrono::high_resolution_clock::now();   // one round of binary heap dij
			mainProgress.ResetProgress(100);
			d->BinHeapDij(srcVertex, graph);
			endTime = chrono::high_resolution_clock::now();
			fp_ms = endTime - startTime;
			bin << graph->nV << ',' << fp_ms.count() << endl;
			cout << graph->nV << " vertices (Bin), " << fp_ms.count() << "ms" << endl;
//			d->PrintPath(srcVertex, terVertex);

			d->InitTable(graph);
			startTime = chrono::high_resolution_clock::now();   // one round of fib heap dij
			mainProgress.ResetProgress(100);
			d->FibonacciHeapDij(srcVertex, graph);
			endTime = chrono::high_resolution_clock::now();
			fp_ms = endTime - startTime;
			fib << graph->nV << ',' << fp_ms.count() << endl;
			cout << graph->nV << " vertices (Fib), " << fp_ms.count() << "ms" << endl;
//			d->PrintPath(srcVertex, terVertex);


//			if (graph->nV <= 5000000) {  // for relatively small map, run ordinary heap dij
//				d->InitTable(graph);
//				startTime = chrono::high_resolution_clock::now();
//				mainProgress.ResetProgress(100);
//				d->OrdinaryDij(srcVertex, graph);
//				endTime = chrono::high_resolution_clock::now();
//				fp_ms = endTime - startTime;
//				ord << graph->nV << ',' << fp_ms.count() << endl;
//				cout << graph->nV << " vertices (Ord), " << fp_ms.count() << "ms" << endl;
//				d->PrintPath(srcVertex, terVertex);
//			}
		}
	}
	return 0;
}

#endif*/

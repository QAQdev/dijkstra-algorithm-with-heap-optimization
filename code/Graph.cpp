#include "Graph.h"

void Graph::ReadGraph() {
    string path; // path of data
    cout << "Please input the data file path: ";
    cin >> path;

    cout << "Reading graph...\n";
    ifstream fp(path);
    if (!fp.is_open()) {
        cout << "Open failed";
        exit(1);
    }
    int v1=0, v2=0, weight=0;
    struct AdjListNode edge;

	int count = 0, vertices = 0, arcs = 0;
	char head;

    auto startTime = chrono::high_resolution_clock::now();
	mainProgress.ResetProgress(100);
	while (true) {
		fp >> head;
		if(head == 'p'){
			fp >> skipws >> head >> head >> vertices >> arcs;
			continue;
		}
		if(head == 'a'){    // data line being marked with leading 'a'
			fp >> skipws >> v1 >> v2>> weight;      // skip white characters
			while (!fp.eof()) {
				mainProgress.ProgressBar(++count, arcs);
				edge.end = v2;
				edge.weight = weight;

				// not exist, allocate memory
				if (array->find(v1) == array->end()) {
					AdjList *pList = new AdjList;
					pList->head->push_front(edge);
					array->insert(pair<int, AdjList *>(v1, pList));
				}
				else {
					array->at(v1)->head->push_front(edge);
				}
				fp >> head >> v1 >> v2 >> weight;
			}
			break;
		}
		else{
			while(head!='\n'){
				fp >> noskipws >> head;
			}
		}
	}
    nV = array->size(); // set number of vertex
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> fp_ms = endTime - startTime;
    cout << "Reading graph cost: " << fp_ms.count() << "ms" << endl;
}

void Graph::ReadGraphTest(ifstream &fp, ofstream &dat) {

	int v1=0, v2=0, weight=0;
	struct AdjListNode edge;

	int count = 0, vertices = 0, arcs = 0;
	char head;

	auto startTime = chrono::high_resolution_clock::now();
	mainProgress.ResetProgress(100);
	while (true) {
		fp >> head;
		if(head == 'p'){
			fp >> skipws >> head >> head >> vertices >> arcs;
			continue;
		}
		if(head == 'a'){    // data line being marked with leading 'a'
			fp >> skipws >> v1 >> v2>> weight;      // skip white characters
			while (!fp.eof()) {
				mainProgress.ProgressBar(++count, arcs);
				edge.end = v2;
				edge.weight = weight;

				// not exist, allocate memory
				if (array->find(v1) == array->end()) {
					AdjList *pList = new AdjList;
					pList->head->push_front(edge);
					array->insert(pair<int, AdjList *>(v1, pList));
				}
				else {
					array->at(v1)->head->push_front(edge);
				}
				fp >> head >> v1 >> v2 >> weight;
			}
			break;
		}
		else {
			while (head != '\n') {
				fp >> noskipws >> head;
			}
		}
	}
	nV = array->size(); // set number of vertex
	auto endTime = chrono::high_resolution_clock::now();
	chrono::duration<double, milli> fp_ms = endTime - startTime;
	cout << "Reading graph cost: " << fp_ms.count() << "ms" << endl;
	cout << arcs << " arcs\t" << vertices << " vertices" << endl;
	dat << arcs << '\t' << fp_ms.count() << endl;
}

void ProgBar::ResetProgress(int segTotal){
	ProgressCount = 0;
	ProgressTotal = segTotal;
}

void ProgBar::PrintProgress(double percentage) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
    if (ProgressCount == ProgressTotal) {
        cout << endl;
    }
}

void ProgBar::ProgressBar(int current, int total) {
	if(current > total / ProgressTotal * ProgressCount){
		PrintProgress((double)ProgressCount/ProgressTotal);
		ProgressCount++;
	}
}

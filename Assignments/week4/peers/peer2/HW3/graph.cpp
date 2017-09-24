#include "graph.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <iomanip>
//main code file
//begin with last homework of graph part
//focus on long paragraph of comments,
//which lead to code for HW3
//first part is in about the fifth paragraph.
//and the other two are at the end of the code.

using namespace std;

bool DOrder::operator()(pair<int, int*> vp, pair<int, int*> up) {
    if ((*(vp.second) == MAX_INT) && (*(up.second) != MAX_INT))
        return true;
    else if ((*(vp.second) != MAX_INT) && (*(up.second) == MAX_INT))
        return false;
    else
        return *(vp.second) > *(up.second);
}

int** graph::initM() {
    int** w = new int*[n];
    for (int v = 0; v < n; v ++) {
        w[v] = new int[n];
        for (int u = 0; u < n; u ++)
            w[v][u] = MAX_INT;
    }
    return w;
}

int* graph::initA(const int value) {
    int* A = new int[n];
    A[0] = 0;
    for (int v = 1; v < n; v ++)
        A[v] = value;
    return A;
}

graph::graph(const int density):
n(50),
w(initM()){
    int count = 50 * 49 / 2 * density / 100;
    srand(time(NULL));
    while (count) {
        int v = rand() % 50, u = rand() % 50;
        if ((v != u) && (w[v][u] == MAX_INT)) {
            w[v][u] = w[u][v] = rand() % 10 + 1;
            count --;
        }
    }
}

//First Part of HW3
//input and read the file "h2.in"(which is specified in "h2.cpp"
//return a graph with connected nodes
//and the corrospending cost of connected edges
//It looks that some of compiler will misunderstand the input file,
//eventhough I test code well in vs, but it doesnot work in Xcode.
//Please focus on and grade my code and my result,"result.jpeg" in the package
//I am really confusing about different compilers.

graph::graph(const char* ifn) {
    int n, v, u, cost;
    ifstream ifile(ifn);
    ifile >> n;
    this->n = n;
    w = initM();
    while (ifile >> v >> u >> cost) {
        w[v][u] = cost;
        w[u][v] = cost;
    }
}

graph::~graph() {
    for (int v = 0; v < n; v ++)
        if (w[v])
            delete [] w[v];
    if (w)
        delete [] w;
    if (D)
        delete [] D;
    if (P)
        delete [] P;
    if (L)
        delete [] L;
    if (Q)
        delete [] Q;
}

double graph::Dijkstra() {
    D = initA(MAX_INT);
    D[0] = 0;
    P = initA(MAX_INT);
    int* isReached = initA(0);
    priority_queue<pair<int, int*>, vector<pair<int, int*> >, DOrder> R;
    for (int v = 0; v < n; v ++) {
        pair<int, int*> vp = make_pair(v, &D[v]);
        R.push(vp);
    }
    while (!R.empty()) {
        int v;
        do {
            v = R.top().first;
            R.pop();
        } while ((!R.empty()) && (isReached[v]));
        if (D[v] == MAX_INT)
            break;
        isReached[v] = 1;
        for (int u = 0; u < n; u ++)
            if ((w[v][u] != MAX_INT)
                && (!isReached[u])
                && ((D[u] == MAX_INT) || ((D[v] != MAX_INT) && (D[u] > D[v] + w[v][u])))) {
                D[u] = D[v] + w[v][u];
                P[u] = v;
                pair<int, int*> up = make_pair(u, &D[u]);
                R.push(up);
            }
    }
    delete [] isReached;
    int count = n - 1;
    double total = 0.0;
    for (int v = 1; v < n; v ++)
        if (D[v] != MAX_INT) {
            total += D[v];
        }
        else
            count --;
    if (!count)
        return 0;
    else
        return total / count;
}

//Second part of HW3
//Here I sperate all nodes into two set
//one contains the origin, and the other contains the rest
//begin with the origin, select one connected node with least-cost
//and store it into the set containing the origin and fresh sets
//repeat previous process
//until get the whole connected graph with least costs
//and compute the total cost of the mst.

int graph::MST() {
    L = initA(MAX_INT);
    L[0] = 0;
    Q = initA(MAX_INT);
    int* isReached = initA(0);
    priority_queue<pair<int, int*>, vector<pair<int, int*> >, DOrder> T;
    for (int v = 0; v < n; v ++) {
        pair<int, int*> vp = make_pair(v, &L[v]);
        T.push(vp);
    }
    while (!T.empty()) {
        int v;
        do {
            v = T.top().first;
            T.pop();
        } while ((!T.empty()) && (isReached[v]));
        if (L[v] == MAX_INT)
            break;
        isReached[v] = 1;
        for (int u = 0; u < n; u ++)
            if ((w[v][u] != MAX_INT)
                && (!isReached[u])
                && ((L[u] == MAX_INT) || ((L[v] != MAX_INT) && (L[u] > w[v][u])))) {
                L[u] = w[v][u];
                Q[u] = v;
                pair<int, int*> up = make_pair(u, &L[u]);
                T.push(up);
            }
    }
    delete [] isReached;
    int total = 0;
    for (int v = 1; v < n; v ++)
        if (L[v] != MAX_INT)
            total += L[v];
        else
            return MAX_INT;
    return total;
}

//similar to the previous part
//select least-cost connected nodes,
//fresh set of nodes in order
//store the order and return as output.

void graph::printTree() const {
    for (int v = 1; v < n; v ++)
        if (Q[v] == MAX_INT) {
            cout << "Not found!" << endl;
            return;
        }
    for (int v = 1; v < n; v ++)
        cout << Q[v] << " -> " << v << endl;
}

void graph::printGraph() const {
    for (int v = 0; v < n; v ++) {
        for (int u = 0; u < n; u ++) {
            cout << setw(3) << w[v][u];
        }
        cout << endl;
    }
}

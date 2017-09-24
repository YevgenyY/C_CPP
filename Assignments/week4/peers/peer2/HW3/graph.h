//
//  graph.h
//  HW3
//
//  Created by Zhang Yuxin on 2017/9/18.
//  Copyright © 2017年 Zhang Yuxin. All rights reserved.
//


#ifndef _graph_h_
#define _graph_h_

#include <vector>
#include <queue>
#include <utility>

#define MAX_INT -1

using namespace std;

//OVERVIEW: A functor that returns whether node "up" is prior to
//          node "vp" in "D" for the priority queue;
struct DOrder {
    bool operator()(pair<int, int*> vp, pair<int, int*> up);
};

//OVERVIEW: A class that represents a graph as a connectivity matrix,
//          including the node size "n", the matrix "w", the array of
//          distance to source "D" and "L" and the array of previous
//          nodes "P" and "Q" for shortest path and minimum spanning
//          tree.
class graph {
    int n;
    int** w;
    int* D, * L, * P, * Q;
    
    //EFFECTS:  Returns an all-zero matrix with size "n" * "n".
    //MODIFIES: this
    int** initM();
    int* initA(const int value);
    
public:
    //EFFECTS:  Construct a graph of 50 nodes with density "density"%
    //          and distances ranges from 1.0 to 10.0.
    //MODIFIES: this
    graph(const int density);
    
    //EFFECTS:  Construct a graph according to the file "ifile".
    //REQUIRES: File named "ifn" exists in pwd and is in the format
    //          node size followed by triples (i, j, cost).
    //MODIFIES: this
    graph(const char* ifn);
    
    //EFFECTS:  Destruct "this".
    //MODIFIES: this
    ~graph();
    
    //EFFECTS:  Returns the average shortest path length of any other
    //          nodes to the first node.
    //MODIFIES: this
    double Dijkstra();
    
    //EFFECTS:  Returns the cost of the minimum spanning tree. Stores
    //          the list of edges of the MST to be used in printTree().
    //MODIFIES: this
    int MST();
    
    //EFFECTS:  Prints the list of edges of the MST.
    //REQUIRES: MST() has already be called.
    void printTree() const;
    
    //EFFECTS:  Prints the connectivity matrix of the graph.
    void printGraph() const;
};

#endif /*_graph_h_*/

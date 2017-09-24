//
//  h2.cpp
//  HW3
//
//  Created by Zhang Yuxin on 2017/9/18.
//  Copyright © 2017年 Zhang Yuxin. All rights reserved.
//


#include "graph.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    graph* G20 = new graph(20);
    //cout << "The connectivity matrix for the 20% tree is:" << endl;
    //G20->printGraph();
    cout << "The average shortest path length for the 20% tree is " << G20->Dijkstra() << "." << endl;
    graph* G40 = new graph(40);
    //cout << "The connectivity matrix for the 40% tree is:" << endl;
    //G40->printGraph();
    cout << "The average shortest path length for the 40% tree is " << G40->Dijkstra() << "." << endl;
    //cout << "The connectivity matrix for the tree read is:" << endl;
    graph* Gread = new graph("h2.txt");
    //Gread->printGraph();
    cout << "The cost for the minimum spanning tree is " << Gread->MST() << "." << endl;
    cout << "The tree edges are:" << endl;
    Gread->printTree();
}

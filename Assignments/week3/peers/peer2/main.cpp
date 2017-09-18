//Assignment 2
//Implement Dijkstra's Algorithm
//
//I use edge list to implement the graph. As the edge density used, 0.2 or 0.4,
//is not very big, the grash is relatively sparse.
//
//As a beginner on C++, I learned the fundamental class manipulation, usage of
//simple containers and also dijkstra algorithm via this assignment.

#include <iostream>
#include <limits>
#include "Graph.h"
#include "PriorityQueue.h"
#include "ShortestPath.h"

using namespace std;

//Calculate the average shortest path from a given node to other nodes.
double average_shortest_path(Graph &g, ShortestPath &sp, int src) {
	int cnt = g.V();
	int total = 0;
	double sum = 0.0;

	for (int i = 0; i < cnt; ++i) {
		if (i == src) {
			continue;
		}

		double size = sp.path_size(src, i);

		//Check if there is a path between node i and node j.
		if (numeric_limits<double>::max() - size < 0.00001) {
			continue;
		}

		sum += size;
		++total;
	}

	return sum / total;
}

int main() {
	int cnt = 50;	//number of verticle
	double dens1 = 0.2, dens2 = 0.4;	//edge density
	double edge_min = 1.0, edge_max = 10.0;	//edge distance range
	double average;

	//Print out the take-away from this assignment. :)
	cout << "As a beginner on C++, I learned the fundamental class "
	    "manipulation, usage of simple containers and also dijkstra"
	    " algorithm via this assignment." << endl << endl;

	//Create the graph with edge density 0.2.
	Graph g1(cnt, dens1, edge_min, edge_max);
	ShortestPath sp1(g1);
	average = average_shortest_path(g1, sp1, 0);

	cout << "The average shortest path size from node 1 to " << cnt <<
	    ": " << "1 to 2, 1 to 3, 1 to 4, …, 1 to " << cnt << ", is " <<
	    average << ", in a graph with density " << dens1 <<
	    ", edge range [" << edge_min << " ," << edge_max << "]." << endl;

	//Create the graph with edge density 0.4.
	Graph g2(cnt, dens2, edge_min, edge_max);
	ShortestPath sp2(g2);
	average = average_shortest_path(g2, sp2, 0);

	cout << "The average shortest path size from node 1 to " << cnt <<
	    ": " << "1 to 2, 1 to 3, 1 to 4, …, 1 to " << cnt << ", is " <<
	    average << ", in a graph with density " << dens2 <<
	    ", edge range [" << edge_min << " ," << edge_max << "]." << endl;

	return 0;
}

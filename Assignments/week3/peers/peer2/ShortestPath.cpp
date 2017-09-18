//Implementation of dijkstra shortest path algorithm.

#include "ShortestPath.h"
#include "Graph.h"
#include "PriorityQueue.h"
#include <vector>
#include <iostream>
#include <list>
#include <limits>

using namespace std;

ShortestPath::ShortestPath(Graph &gg): g(gg) {
}

//Internal function to calculate the shortest path distance and previous
//node list using dijkstra algorithm.
vector<int> ShortestPath::get_path(int src, int dst, double &dis) {
	int cnt = g.V();
	vector<int> prev(cnt, -1);
	vector<bool> found(cnt, false);
	vector<double> distance(cnt, numeric_limits<double>::max());
	PriorityQueue pq;

	distance[src] = 0.0;
	edge e = {src, src, 0.0};
	pq.insert(e);

	//Perform dijkstra algorithm.
	while (pq.size() > 0) {
		//Obtain the shortest distance from the priority queue.
		edge emin = pq.top();
		int u = emin.end;
		pq.minPriority();
		found[u] = true;

		//Iterate all the adjacent nodes.
		list<int> neighbor = g.neighbors(u);
		for (list<int>::iterator iter = neighbor.begin();
		    iter != neighbor.end(); ++iter) {
			int v = *iter;
			if (found[v]) {
				continue;
			}

			double newdis = distance[u] +
			    g.get_edge_value(u, v);

			//The new distance is shorter than that prior found.
			if (distance[v] > newdis) {
				distance[v] = newdis;

				edge en = {src, v, newdis};

				//Update the path's value if it already exists
				//in the priority queue, otherwise insert it.
				if (pq.contains(en)) {
					pq.chgPriority(en, newdis);
				} else {
					pq.insert(en);
				}

				prev[v] = u;
			}
		}
	}

	dis = distance[dst];

	return prev;
}

//Find shortest path between u-w and returns the sequence of vertices
//representing shorest path.
list<int> ShortestPath::path(int src, int dst) {
	double size;
	list<int> l;
	int cur = dst;

	vector<int> prev = get_path(src, dst, size);

	//Check if there is a path between the source and destination.
	if (numeric_limits<double>::max() - size < 0.00001) {
		return l;
	}

	l.push_front(cur);
	while (prev[cur] != -1) {
		l.push_front(prev[cur]);
		cur = prev[cur];
	}

	return l;
}

//Return the path cost associated with the shortest path.
double ShortestPath::path_size(int src, int dst) {
	double size;

	(void) get_path(src, dst, size);

	return size;
}

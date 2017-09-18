//Dijkstra shortest path class

#ifndef _SHORTEST_PATH
#define _SHORTEST_PATH

#include "Graph.h"
#include "PriorityQueue.h"
#include <list>
#include <vector>

class ShortestPath {
	private:
		Graph &g;	//Reference to the given graph
		std::vector<int> get_path(int src, int dst, double &dis);
	public:
		ShortestPath(Graph &gg);
		std::list<int> path(int src, int dst);
		double path_size(int src, int dst);
};


#endif

//Implementation of graph operation.

#include "Graph.h"
#include <iostream>
#include <list>
#include <vector>
#include <random>

using namespace std;

Graph::Graph(): density(0.0), distance_min(0.0), distance_max(0.0) {
}

//Create a graph according to these given conditions:
//	*number of verticles
//	*edge density between [0, 1]
//	*Distance of an edge in the range [dis_min, dis_max]
Graph::Graph(int nvtcl, double dens, double dis_min, double dis_max) {
	verticles.resize(nvtcl);
	edges.resize(nvtcl);
	density = dens;
	distance_min = dis_min;
	distance_max = dis_max;

	random_device v_rd;
	default_random_engine v_re(v_rd());
	uniform_real_distribution<double> v_rgen(0.0, 1.0);

	random_device e_rd;
	default_random_engine e_re(e_rd());
	uniform_real_distribution<double> e_rgen(distance_min, distance_max);

	for (int i = 0; i < nvtcl; ++i) {
		set_node_value(i, i);
	}

	for (int i = 0; i < nvtcl; ++i) {
		for (int j = i + 1; j < nvtcl; ++j) {
			// Create an edge if we're in the probability range.
			if (v_rgen(v_re) < density) {
				double dis = e_rgen(e_re);

				add(i, j);
				set_edge_value(i, j, dis);
				add(j, i);
				set_edge_value(j, i, dis);
			}
		}
	}
}

//Returns the number of vertices in the graph.
int Graph::V() {
	return verticles.size();
}

//Returns the number of edges in the graph.
int Graph::E() {
	int cnt = 0;

	for (int i = 0; i < verticles.size(); ++i) {
		list<edge>& l = edges[verticles[i]];
		list<edge>::iterator iter;

		for (iter = l.begin(); iter != l.end(); ++iter) {
			++cnt;
		}
	}

	return cnt;
}

//Tests whether there is an edge from node x to node y.
bool Graph::adjacent(int x, int y) {
	list<edge>& lx = edges[x];
	list<edge>::iterator i;

	for (i = lx.begin(); i != lx.end(); ++i) {
		if ((*i).end == y)
			return true;
	}

	return false;
}

//Lists all nodes y such that there is an edge from x to y.
list<int> Graph::neighbors (int x) {
	list<int> res;
	list<edge>& lx = edges[x];
	list<edge>::iterator i;

	for (i = lx.begin(); i != lx.end(); ++i) {
		res.push_front((*i).end);
	}

	return res;
}

//Adds to G the edge from x to y, if it is not there.
void Graph::add(int x, int y) {
	bool exist = false;

	list<edge>& lx = edges[x];
	list<edge>::iterator i;

	for (i = lx.begin(); i != lx.end(); ++i) {
		if ((*i).end == y)
			exist = true;
	}

	if (!exist) {
		edge e;
		e.start = x;
		e.end = y;
		e.dis = 0.0;
		lx.push_front(e);
	}

	return;
}

//Removes the edge from x to y, if it is there.
void Graph::remove(int x, int y) {
	list<edge>& lx = edges[x];
	list<edge>::iterator i;

	for (i = lx.begin(); i != lx.end(); ++i) {
		if ((*i).end == y) {
			i = lx.erase(i);
			break;
		}
	}

	return;
}

//Returns the value associated with the node x.
int Graph::get_node_value (int x) {
	return verticles[x];
}

//Sets the value associated with the node x to a.
void Graph::set_node_value(int x, int a) {
	verticles[x] = a;
	return;
}

//Returns the value associated to the edge (x,y).
double Graph::get_edge_value (int x, int y) {
	list<edge>& l = edges[x];
	list<edge>::iterator i;

	for (i = l.begin(); i != l.end(); ++i) {
		if ((*i).end == y)
			return (*i).dis;
	}

	return -1.0;
}

//Sets the value associated to the edge (x,y) to v.
void Graph::set_edge_value(int x, int y, double v) {
	list<edge>& lx = edges[x];
	list<edge>::iterator i;

	for (i = lx.begin(); i != lx.end(); ++i) {
		if ((*i).end == y)
			(*i).dis = v;
	}

	return;
}

//Helper function: print the graph's verticles and edges.
void Graph::print() {
	int vcnt = V();
	cout << "Print the Graph's total number of verticles: " << vcnt <<
	    endl << "total number of edges: " << E() <<
	    " Max number allowed: " << vcnt * (vcnt - 1) << endl;

	cout << "Density " << density << ", expected number of edges: " <<
	    density * vcnt * (vcnt - 1) << endl;
	cout << "Print edges." << endl;

	for (int i = 0; i < vcnt; ++i) {
		cout << "Edges of node " << i << ": ";

		list<edge>& l = edges[i];
		list<edge>::iterator iter;

		for (iter = l.begin(); iter != l.end(); ++iter) {
			cout << i << " -> " << (*iter).end << " ("
			    << (*iter).dis << "), ";
		}
		cout << endl;
	}

	return;
}

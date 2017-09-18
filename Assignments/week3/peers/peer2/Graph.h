//Graph class and edge structure.

#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>
#include <list>

typedef struct _edge {
	int	start;
	int	end;
	double	dis;	//distance

	bool operator >(const struct _edge &rhs) const {
		return (dis < rhs.dis);
	}

	bool operator <(const struct _edge &rhs) const {
		return (dis > rhs.dis);
	}
} edge;

class Graph {
	private:
		std::vector<int> verticles;
		std::vector<std::list<edge> > edges;
		double density;
		double distance_min;
		double distance_max;

	public:
	Graph();
	Graph(int nvtcl, double dens, double dis_min, double dis_max);
	int V(void);
	int E(void);
	bool adjacent(int x, int y);
	std::list<int> neighbors(int x);
	void add(int x, int y);
	void remove(int x, int y);
	int get_node_value(int x);
	void set_node_value(int x, int a);
	double get_edge_value(int x, int y);
	void set_edge_value(int x, int y, double v);
	void print(void);
};

#endif

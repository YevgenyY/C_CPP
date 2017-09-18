//Priority queue class

#ifndef _PRIO_QUE_H
#define _PRIO_QUE_H

#include <vector>
#include "Graph.h"

class PriorityQueue {
	private:
		std::vector<edge> queue;	//inner container

	public:
	PriorityQueue();
	void chgPriority(edge e, double dis);
	void minPriority(void);
	bool contains(edge e);
	void insert(edge e);
	edge top(void);
	int size(void);
	void print(void);
};

#endif

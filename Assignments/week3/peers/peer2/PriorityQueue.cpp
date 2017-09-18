//Implementation of priority queue used for dijkstra algorithm.

#include "PriorityQueue.h"
#include "Graph.h"
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

PriorityQueue::PriorityQueue() {
	make_heap(queue.begin(), queue.end(), greater<edge>());
}

//Changes the priority (node value) of queue element.
void PriorityQueue::chgPriority(edge e, double dis) {
	for (auto &ele : queue) {
		if (ele.start == e.start && ele.end == e.end) {
			ele.dis = dis;
			break;
		}
	}

	//Restore the heap after change the element.
	make_heap(queue.begin(), queue.end());

	return;
}

//Removes the top element of the queue.
void PriorityQueue::minPriority() {
	pop_heap(queue.begin(), queue.end());
	queue.pop_back();
}

//Return true if the priority queue already contains the given edge.
bool PriorityQueue::contains(edge e) {
	for (auto &ele : queue) {
		if (ele.start == e.start && ele.end == e.end) {
			return true;
		}
	}

	return false;
}

//Insert edge into the priority queue.
void PriorityQueue::insert(edge e) {
	queue.push_back(e);
	push_heap(queue.begin(), queue.end());
}

//Returns the edge on the top of the priority queue.
edge PriorityQueue::top() {
	vector<edge>::iterator iter = queue.begin();

	return (*iter);
}

//Return the number of edges in this priority queue.
int PriorityQueue::size() {
	return queue.size();
}

//Helper function: print the queue array.
void PriorityQueue::print() {
	cout << "Print priority queue:" << endl;
	for (auto &ele : queue) {
		cout << "edge: " << ele.start << "-> " << ele.end << ": "
		    << ele.dis << endl;
	}

	return;
}

//
// This is an implementation of Dijkstra
// the shortest path algorythm
// https://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%94%D0%B5%D0%B9%D0%BA%D1%81%D1%82%D1%80%D1%8B

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

template <class D, class W> // density, weight: float, int, double etc...
class Graph
{
	public:
		Graph();
		~Graph();
		Graph(int size, D density, W range);

		void print_graph();

	private:
		W **mGraph; // "W" means the distance weight, 0 means - no edge
		const int mSize;
		const D mDensity;
		const W mRange;
};

// generate random values for density and distanse weight
inline int prob()
{
	return (rand() % 100);
}
template <class W> 
W generate_weight(W range)
{
	return ( rand() % range );
}
/* we use different method of a weight generation
 * fo float type 
 */
float generate_weight(float range)
{
	return static_cast <float> (rand()) / static_cast <float> (range);
}

/* Generate a graph with given size, densty
 * and weight values
 */
template <class D, class W> 
Graph<D, W>::Graph (int size=10, D density=10, W range=10) :
	mSize(size),
	mDensity(density),
	mRange(range)
{
	srand(time(0)); // seed rand
	mGraph = new W*[size];

	for(int i = 0; i < size; ++i)
	{
		mGraph[i] = new W[size];
	}

	for(int i=0; i < size; ++i)
		for(int j=0; j < size; ++j)
			if (i==j)
				mGraph[i][j] = 0; // no loops
			else
			{
				// it is easier to use 0-100
				// probability values than 0.0-1.0
				mGraph[i][j] = mGraph[j][i] = (prob() < mDensity) ? 0 
					: generate_weight(range); 
			}
}
template <class D, class W>
Graph<D, W>::~Graph()
{
	for(int i = 0; i < mSize; ++i)
	{
		delete mGraph[i];
	}
}
template <class D, class W>
void Graph<D, W>::print_graph()
{
	cout << "The graph is here: " << endl;
	for (int i=0; i < mSize; ++i)
	{
		for(int j=0; j < mSize; ++j)
			cout << mGraph[i][j] << " ";

		cout << endl;
	}
}
#if 0
bool is_connected(bool *graph[], int size)
{
	int old_size = 0, c_size = 0;
	bool* close = new bool[size];
	bool* open = new bool[size];

	// init arrays
	for(int i=0; i < size; ++i)
		open[i] = close[i] = false;

	// init the first node
	open[0] = true;

	while(c_size < size)
	{
		for(int i=0; i < size; ++i)
		{
			old_size = c_size;
			if(open[i] && (close[i]==false))
			{
				close[i] = true;
				c_size++;

				for(int j=0; j < size; ++j)
					open[j] = open[j] || graph[i][j];
			}
		}

		if(old_size = c_size)
			return false;
	}

	if(c_size == size)
		return true;

	// this should be never reached
	return true;
}
#endif
int main( void )
{
	Graph<int, int> myGraph = Graph<int, int>(10, 40, 10);
	myGraph.print_graph();

#if 0
	bool isConnected = is_connected(graph, 3);
	cout << "is connected: " << isConnected << endl;
#endif
	return 0;
}

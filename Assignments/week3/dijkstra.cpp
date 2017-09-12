//
// This is an implementation of Dijkstra
// the shortest path algorythm
// https://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%94%D0%B5%D0%B9%D0%BA%D1%81%D1%82%D1%80%D1%8B

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;
// Auxilary class for the convenience
class Vertex
{
public:
	Vertex(int x, int y) : x(x), y(y) {}; // constructor
	int getx()
	{
		return x;
	};
	int gety()
	{
		return y;
	};
private:
	const int x;
	const int y;
};

template <class W> // W-weight: float, int, double etc...
class Graph
{
public:
	Graph();
	~Graph();
	Graph(unsigned int size=10, float density=0.1, W range=10);

	W osp_dijkstra(int x1, int y1, int x2, int y2);
	void print_graph();

private:
	W **mGraph; // "W" means the distance weight, 0 means - no edge
	const int mSize;
	const int mDensity;
	const W mRange;
};

// generate random values for density and distanse weight
inline int probe()
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

/* Constructor.
 * Generate a graph with given size, density
 * and distance weights values
 */
template <class W>
Graph<W>::Graph(unsigned int size, float density, W range) :
	mSize(size),
	mDensity(abs(density)),
	mRange(abs(range))
{
	unsigned int lDensity = static_cast<unsigned int>(mDensity * 100);
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
				mGraph[i][j] = mGraph[j][i] = (probe() < lDensity) ? 0
				                              : generate_weight(range);
			}
}
template <class W>
Graph<W>::~Graph()
{
	for(int i = 0; i < mSize; ++i)
	{
		delete mGraph[i];
	}
}
template <class W>
void Graph<W>::print_graph()
{
	cout << "The graph is here: " << endl;
	for (int i=0; i < mSize; ++i)
	{
		for(int j=0; j < mSize; ++j)
			cout << mGraph[i][j] << " ";

		cout << endl;
	}
}
#if 1
template <class W>
W Graph<W>::osp_dijkstra(Vertex origin)
{
	vector<Vertex> lVertexVisited; // visited vertexs
	vector<Vertex> lVertexTesting; // ongoing vertexs
	vector<Vertex> lMinPath;	   // min path vertexs
	W lMinPathWeight;
}
#endif
int main( void )
{
	Graph<int> myGraph = Graph<int>(10, 0.4, 10);
	myGraph.print_graph();

#if 0
	bool isConnected = is_connected(graph, 3);
	cout << "is connected: " << isConnected << endl;
#endif
	return 0;
}

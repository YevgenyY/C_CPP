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

template <typename W> // W-weight: float, int, double etc...
class Graph
{
public:
	Graph();
	~Graph();
	Graph(unsigned int size=10, float density=0.1, W range=10);

	void osp_dijkstra(int);
	void print_graph();
	W minDistance(W dist[], bool sptSet[]);
	int printSolution(W dist[], int src);
	int saveSolution(W dist[], int src);

private:
	// "W" means the distance weight, 0 means - no edge
	W **mGraph; 		// connectivity matrix
	W **mOSP;		// open shortest path matrix
	const int mSize;
	const int mDensity;
	const W mRange;
};

// generate random values for density and distanse weight
inline int probe()
{
	return (rand() % 100);
}
template <typename W>
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
template <typename W>
Graph<W>::Graph(unsigned int size, float density, W range) :
	mSize(abs(size)),
	mDensity(abs(density)),
	mRange(abs(range))
{
	unsigned int lDensity = static_cast<unsigned int>(density * 100);
	srand(time(0)); // seed rand
	mGraph = new W*[size];
	mOSP = new W*[size];

	cout << "lDensity " << lDensity << endl;

	for(int i = 0; i < size; ++i)
	{
		mGraph[i] = new W[size];
		mOSP[i] = new W[size];
	}

	for(int i=0; i < size; ++i)
		for(int j=0; j < size; ++j)
		{
			mOSP[i][j] = mOSP[j][i] = static_cast<W>INFINITY;
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
}
template <typename W>
Graph<W>::~Graph()
{
	for(int i = 0; i < mSize; ++i)
	{
		delete mGraph[i];
		delete mOSP[i];
	}
}
template <typename W>
void Graph<W>::print_graph()
{
	cout << "The connectivity matrix: " << endl;
	for (int i=0; i < mSize; ++i)
	{
		for(int j=0; j < mSize; ++j)
			cout << mGraph[i][j] << " ";

		cout << endl;
	}
	cout << "The osp matrix: " << endl;
	for (int i=0; i < mSize; ++i)
	{
		for(int j=0; j < mSize; ++j)
			cout << mOSP[i][j] << " ";

		cout << endl;
	}
}
// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
template <typename W>
W Graph<W>::minDistance(W dist[], bool sptSet[])
{
	// Initialize min value
	W min = static_cast<W>INFINITY, min_index;

	for (int v = 0; v < mSize; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;

	return min_index;
}

// A utility function to print the constructed distance array
template <typename W>
int Graph<W>::printSolution(W dist[], int src)
{
	cout << "Vertex Distance from Source " << src << endl;
	cout << "vertex   "; 
	for (int i = 0; i < mSize; i++)
		cout << i << "\t";
	cout << endl;

	cout << "distance ";
	for (int i = 0; i < mSize; i++)
		cout << dist[i] << "\t";
	cout << endl;
}

template <typename W>
int Graph<W>::saveSolution(W dist[], int src)
{
	cout << "Savinf OSP for vertex " << src  << endl;
	for (int i = 0; i < mSize; i++)
		cout << i << "\t\t" << dist[i] << endl;
}



#if 1
template <typename W>
void Graph<W>::osp_dijkstra(int src)
{
	W dist[mSize];  // The output array.  dist[i] will hold the shortest
					// distance from src to i

	bool sptSet[mSize];// sptSet[i] will true if vertex i is included in shortest
					   // path tree or shortest distance from src to i is finalized

	// Initialize all distances as INFINITE and stpSet[] as false
	for (int i = 0; i < mSize; i++)
		dist[i] = static_cast<W>INFINITY, sptSet[i] = false;

	// Distance of source vertex from itself is always 0
	dist[src] = 0;

	// Find shortest path for all vertices
	for (int count = 0; count < mSize-1; count++)
	{
		// Pick the minimum distance vertex from the set of vertices not
		// yet processed. u is always equal to src in first iteration.
		W u = minDistance(dist, sptSet);

		// Mark the picked vertex as processed
		sptSet[u] = true;

		// Update dist value of the adjacent vertices of the picked vertex.
		for (int v = 0; v < mSize; v++)

			// Update dist[v] only if is not in sptSet, there is an edge from
			// u to v, and total weight of path from src to  v through u is
			// smaller than current value of dist[v]
			if (!sptSet[v] && mGraph[u][v] && dist[u] != INFINITY
			        && dist[u]+mGraph[u][v] < dist[v])
				dist[v] = dist[u] + mGraph[u][v];
	}

	// print the constructed distance array
	printSolution(dist, src);
	saveSolution(dist, src);
}
#endif
int main( void )
{
	Graph<int> myGraph = Graph<int>(4, 0.5, 10);
#if 1
	myGraph.osp_dijkstra( 0 ); // start with vertex #0
#endif

	myGraph.print_graph();
	return 0;
}




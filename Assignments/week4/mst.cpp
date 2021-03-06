//
// This is an implementation of Dijkstra
// the shortest path algorythm
// and
// Jarnik-Prm MST algorythm implementations
// 
// Please compile the file with the following command
// $ g++ -std=c++11 -g -o main mst.cpp
//

#include <iostream>
#include <iterator>
#include <fstream>
#include <algorithm>
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
	Graph(const char filename[]);// construct graph from a file

	void ospDijkstra(int src); // src is the starting point
	W ospAverage(const unsigned int num); // num is the number of paths which are
	// involved in average osp calculation

	void mstJarnikPrim(); // Jarnik-Prim minimum spanning tree algorythm implementation

	void printGraph();
	void printSolution(W dist[], int src);
	void printMSTSolution(); // print minimum spanning tree connectivity matrix
	void saveSolution(W dist[], int src);

	int getSize()
	{
		return mSize;
	};
	W minDistance(W dist[], bool sptSet[]);

private:
	// "W" means the distance weight, 0 means - no edge
	W **mGraph;		// connectivity matrix
	W **mOSP;		// open shortest path matrix
	W **mMST;		// minimum spanning tree matrix
	int mSize;
	int mDensity;
	W mRange;
};

// generate random values for density and distanse
// weight calculations
inline int probe()
{
	return (rand() % 100);
}
// function to test probe()
// it prints random sequence for manual checking
inline void testprobe(int num)
{
	cout << "Random sequence testing..." << endl;
	for (int i=0; i < num; ++i)
		cout << probe() << " ";
	cout << endl;
}
// generic distance weight generating
// it suits for signed and unsigned numeric types
// i.e. int, long, double, etc...
template <typename W>
W generate_weight(W range)
{
	return ( rand() % range );
}

// if W is float then
// I use different method of a weight generation
float generate_weight(float range)
{
	return static_cast <float> (rand()) / static_cast <float> (range);
}

// Constructor.
// Generate a graph with given size, density
// and distance weights values
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

	cout << endl;
	cout << "Size: " << size << " Density: " << lDensity << "% Range: " << range << endl;
	cout << "Initializing graph..." << endl;

	testprobe(10);
	cout << endl;

	// allocate conectivity matrix
	// and open shortest path (OSP)
	// matrix
	for(int i = 0; i < size; ++i)
	{
		mGraph[i] = new W[size];
		mOSP[i] = new W[size];
		mMST[i] = new W[size];
	}

	for(int i=0; i < size; ++i)
		for(int j=0; j < size; ++j)
		{
			// fill in OSP matrix with INFINITY weights
			mOSP[i][j] = mOSP[j][i] = static_cast<W>(INFINITY);
			if (i==j)
				mGraph[i][j] = 0; // no loops
			else
			{
				// it is easier to use 0-100
				// probability values than 0.0-1.0
				mGraph[i][j] = mGraph[j][i] = (probe() > lDensity) ? 0
				                              : generate_weight(range);
			}
			mMST[i][j] = 0; // initialize MST matrix with 0
		}
}

// Constructor
// build a graph from a file
template <typename W>
Graph<W>::Graph(const char filename[])
{
	ifstream graph_file(filename);
	istream_iterator<int> start(graph_file), end;

	vector<int> ints(start, end);

	// set Graph size
	mSize = ints[0];
	ints.erase(ints.begin() + 0);
#if 0
	// print values for manual checking
	for (auto str : ints)
		cout << str << " ";
	cout << endl;
#endif

	cout << endl << "Number of vertexies in a graph is: " << mSize << endl;

	// allocate memory for the conectivity matrix
	mGraph = new W*[mSize];
	mOSP = new W*[mSize];
	mMST = new W*[mSize];

	// allocate conectivity matrix
	// and open shortest path (OSP)
	// matrix
	for(int i = 0; i < mSize; ++i)
	{
		mGraph[i] = new W[mSize];
		mOSP[i] = new W[mSize];
		mMST[i] = new W[mSize];
	}
	// set initial zeros
	for( int i=0; i < mSize; ++i)
		for( int j=0; j < mSize; ++j)
			mGraph[i][j] = mOSP[i][j] = mMST[i][j] = 0;


	// now we can read triplets: first co-ordinate, second co-ordinate and the weight
	int x,y;
	W w;
	while ( !ints.empty() )
	{
		w = ints.back();
		ints.pop_back();
		y = ints.back();
		ints.pop_back();
		x = ints.back();
		ints.pop_back();

		//cout << x << " " << y << " " << w << endl;

		// set link between vertexies and it's weight
		mGraph[x][y] = w;
	}
}
// Destructor
// frees memories
template <typename W>
Graph<W>::~Graph()
{
	for(int i = 0; i < mSize; ++i)
	{
		delete mGraph[i];
		delete mOSP[i];
		delete mMST[i];
	}
}
// Prints connectivity and OSP matrixies
template <typename W>
void Graph<W>::printGraph()
{
	cout << "The connectivity matrix (the graph): " << endl;
	for (int i=0; i < mSize; ++i)
	{
		for(int j=0; j < mSize; ++j)
			cout << mGraph[i][j] << " ";

		cout << endl;
	}
	cout << endl;
	return;
	cout << "The osp matrix (the open shortest paths weights): " << endl;
	for (int i=0; i < mSize; ++i)
	{
		for(int j=0; j < mSize; ++j)
			cout << mOSP[i][j] << " ";

		cout << endl;
	}
}
// finds the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
template <typename W>
W Graph<W>::minDistance(W dist[], bool sptSet[])
{
	// Initialize min value
	W min = static_cast<W>(INFINITY), min_index;

	for (int v = 0; v < mSize; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;

	return min_index;
}

// prints the constructed distance array
template <typename W>
void Graph<W>::printSolution(W dist[], int src)
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
// saves the solution into OSP matrix
template <typename W>
void Graph<W>::saveSolution(W dist[], int src)
{
	// cout << "Saving OSP for vertex " << src  << endl;
	// use 0 instead of INFINITY for marking "no path"
	for (int i = 0; i < mSize; i++)
		mOSP[src][i] = (dist[i] == static_cast<W>(INFINITY)) ? 0 : dist[i];
}
// Dijkstra algorythm implementation
#if 1
template <typename W>
void Graph<W>::ospDijkstra(int src)
{
	W dist[mSize];  // The output array.  dist[i] will hold the shortest
	// distance from src to i

	bool sptSet[mSize];// sptSet[i] will true if vertex i is included in shortest
	// path tree or shortest distance from src to i is finalized

	// Initialize all distances as INFINITE and stpSet[] as false
	for (int i = 0; i < mSize; ++i)
		dist[i] = static_cast<W>(INFINITY), sptSet[i] = false;

	// Distance of source vertex from itself is always 0
	dist[src] = 0;

	// Find shortest path for all vertices
	for (int count = 0; count < mSize-1; ++count)
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
			if (!sptSet[v] && mGraph[u][v] && dist[u] != static_cast<W>(INFINITY)
			        && dist[u]+mGraph[u][v] < dist[v])
				dist[v] = dist[u] + mGraph[u][v];
	}

	// print the constructed distance array
	//printSolution(dist, src);
	saveSolution(dist, src);
}
#endif
# if 1
// Calculates OSP averages for given number of paths
template <typename W>
W Graph<W>::ospAverage(unsigned int num)
{
	int counter = 0; // paths counter
	W weight = static_cast<W> (0);

	for (int i=0; i < mSize; ++i)
	{
		for (int j=0; j < mSize; ++j)
		{
			if (mOSP[i][j] > 0)
				weight += mOSP[i][j], counter++;
			if (counter == num)
				break;
		}
		if (counter == num)
			break;
	}

	if (counter != num)
	{
		cout << endl << "Too few paths in a graph" << endl;
		return 0;
	}
	else
		return weight/static_cast<W>(counter);
}
#endif
//
// Returns minimum weight index in vector v
int getMinWeightIdx(int *v, int size)
{
	int idx = -1;

	for (int i=0; i < size; ++i)
	{
		if (v[i] != 0 && idx == -1)
			idx = i;
		else if (v[i] != 0 && v[i] < v[idx])
			idx = i;
	}

	return idx;
}
//
// prints the constructed distance array
template <typename W>
void Graph<W>::printMSTSolution()
{
	cout << endl;
	cout << "The MST connectiity matrix: " << endl;
	for (int i=0; i < mSize; ++i)
	{
		for(int j=0; j < mSize; ++j)
			cout << mMST[i][j] << " ";

		cout << endl;
	}

}

//
// Jarnik-Prim minimum spanning tree algorythm implementation
template <typename W>
void Graph<W>::mstJarnikPrim()
{
	vector<int> visited; // visited vertexies
	vector<int> vertexies; // not visited

	// initialize vertexies
	// visited.size() = 0
	// vertexies.size() = mSize
	for (int i=0; i < mSize; ++i)
		vertexies.push_back(i);

	// we use vertex 0 as a starting point of MST algo
	visited.push_back(0);
	vertexies.erase(vertexies.begin() + 0); // remove starting point

	// start MST lookup
	while( !vertexies.empty() )
	{
		int idx = 0;
		W minWeight = static_cast<W>INFINITY;
		int minIdx = -1; // link from visited vertexies to unvisited vertex
		int minIdxVisited = -1;
		// with minimum weight

		//cout << "Lookup a link with minimal weight from visited vertexies..." << endl;
		for (int i : visited)
		{
			// find vertex id with minimum link weight
			idx = getMinWeightIdx(mGraph[i], mSize);
			if ( idx != -1 && mGraph[i][idx] < minWeight && \
			        !(find(visited.begin(), visited.end(), idx) != visited.end()) )
			{
				minWeight = mGraph[i][idx];
				minIdx = idx;
				minIdxVisited = i;
			}
		}

		if (minIdx != -1)
		{
			mMST[minIdxVisited][minIdx] = mMST[minIdx][minIdxVisited] = mGraph[minIdxVisited][minIdx];
			visited.push_back(minIdx);
			// remove idx from unvisited vertexies
			vertexies.erase(remove(vertexies.begin(), vertexies.end(), minIdx),
			                vertexies.end());
			cout << "Add link: " << minIdxVisited << "->" << minIdx << " " << minWeight << endl;

			// unset all links from idx to visited vertexies
			for ( int j : visited )
				mGraph[minIdx][j] = mGraph[j][minIdx] = 0;
		}
#if 0
		// print visited vertexies vector
		for (int i : visited)
			cout << i << " ";
		cout << endl;
		for (int i : vertexies)
			cout << i << " ";
		cout << endl;
#endif
	}
}
int main( void )
{
	/* graph initialization constants */
	const int size = 50;
	const float density = 0.4;
	const int range = 10;
	const int montecarloNum = 50;

#if 0
	/* Dijkstra algorythm implementation */

	// make a graph with it's constructor
	Graph<int> myGraph = Graph<int>(size, density, range);

	// calculate all the OSPs in the graph
	for (int i=0; i < myGraph.getSize(); ++i)
		myGraph.ospDijkstra( i );

	// print connectivity matrix ans OSP matrix
	//myGraph.printGraph();

	// calculate and print average OSP length
	cout << "Average path length for " << montecarloNum << \
	     " paths is:"	<< myGraph.ospAverage(50) << endl;
#endif
#if 1
	// make a graph with it's new constructor
	Graph<int> myGraph = Graph<int>("SampleTestData_mst_data.txt");
	myGraph.printGraph();
	myGraph.mstJarnikPrim();
	myGraph.printMSTSolution();

	cout << "end" << endl;

#endif

	return 0;
}



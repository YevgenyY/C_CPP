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

	void osp_dijkstra(int);
	void print_graph();

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
template <class W>
Graph<W>::~Graph()
{
	for(int i = 0; i < mSize; ++i)
	{
		delete mGraph[i];
		delete mOSP[i];
	}
}
template <class W>
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
#if 1
//TODO: use a recursion
template <class W>
void Graph<W>::osp_dijkstra(int origin)
{
	W weights[mSize];
	int visited[mSize];
	for (int i=0; i < mSize; ++i) 
	{
		// initialize with zero
		weights[i] = static_cast<W> INFINITY;
		visited[i] = 0; // initialize with not visited
	}
	weights[origin] = 0;

	/* fill in current/reachable vertexs
	 * for vertex "i"
	 */
	for (int j=0; j < mSize; ++j)
	{
		if (mGraph[origin][j] > 0)
		{
			W dw = mGraph[origin][j];
			if ( weights[origin] + dw < weights[j] )
				weights[j] = weights[origin] + dw;
		}
	}

	cout << "Weights are:" << endl;
	for (int i=0; i < mSize; ++i)
		cout << weights[i] << " ";
	cout << endl;


#if 0
	for (int i=0; i < mSize; ++i)
	{
		/* fill in current/reachable vertexs
		 * for vertex "i"
		 */
		for (int j=0; j < mSize; ++j)
		{
			if (mGraph[i][j] > 0)
			{
				/* collect all connected verticies */
				lVertexCurrent.push_back(j);
				//cout << "i=" << i <<" push_back " << j << endl;
			}
		}

		if (!lVertexCurrent.empty())
		{
			int index;
			vector<int>::iterator it = lVertexCurrent.begin();
			for (; it != lVertexCurrent.end(); )
			{
				cout << *it << " ";
				index = *it;

				if (i == origin) // if it's origin W=0
				{
					mOSP[i][index] = mGraph[i][index];
				}



				lVertexCurrent.erase(it);
			}
			cout << endl;
		}
	}
#endif
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


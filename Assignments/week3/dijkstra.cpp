//
// This is an implementation of Dijkstra
// the shortest path algorythm
// https://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%94%D0%B5%D0%B9%D0%BA%D1%81%D1%82%D1%80%D1%8B

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

bool **graph;
const int size = 8;

// generate random value in a range 0 - 100
inline int prob()
{
	return (rand() % 100);
}
/* Generate a graph with given size 
 * and density values
 */
void init_graph(int size=10, int probe=10, int range = 10)
{
	srand(time(0)); // seed rand
	graph = new bool*[size];

	for(int i = 0; i < size; ++i)
	{
		graph[i] = new bool[size];
	}

	for(int i=0; i < size; ++i)
		for(int j=0; j < size; ++j)
			if (i==j)
				graph[i][j] = false; // no loops
			else
			{
				// it is easier to use 0-100
				// probability values than 0.0-1.0
				graph[i][j] = graph[j][i] = (prob() < probe); 
			}
}
void print_graph(int size)
{
	cout << "The graph is here: " << endl;
	for (int i=0; i < size; ++i)
	{
		for(int j=0; j < size; ++j)
			cout << graph[i][j] << " ";

		cout << endl;
	}
}
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
int main( void )
{
	init_graph(size, 110);
	print_graph(size);

	bool isConnected = is_connected(graph, 3);
	cout << "is connected: " << isConnected << endl;

	return 0;
}

/**	Author: BO-YEN, HUANG.						**/
/**	Date: 2017/08/30.							**/
/**	Title: Using Kruskal's algorithm to solve minimum spanning tree.			**/
/**	Suggest compiler: >=gcc7.0 version or you can use online compiler : http://cpp.sh/	**/
/**	Not suggest compiler: dev C & Virtiual studio.				**/

#include <iostream>
#include <ctime>	// Use time variables to form random seed.
#include <cstdlib>	// Rand function to generate probabilities of edge forming.
#include <fstream>	// Output result.
#include <iomanip>
#include <vector>	// Deploy dynamic array to sort map by value through sort function (algorithm package).
#include <tuple>	// Can load multiple classes, not only 2 (pair). This package suggest (>=gcc7.0) version.
		// Avoid using dev C or Virtiual studio. To save lots of grief directly, you can use this 
		// online compiler: http://cpp.sh/
#include <algorithm>	// Only use sort function.
#include <map>	// Key-value container sorted key by red-black algorithm.

using namespace std;

class Kruskal_processing;

// Generate probabilities.
double prob() {
	return (static_cast<double>(rand()) / RAND_MAX);
}

tuple<int, bool**, int**> generate_graph(Kruskal_processing& DJ);

class Kruskal_processing {
private:
// Announce 2d-dynamic arrays.
	bool** graph;
	int** cost;
public:
	Kruskal_processing() {}

	friend tuple<int, bool**, int**> generate_graph(Kruskal_processing& DJ);
	void print_generate_graph(int _size, bool** _graph, int** _cost);
	Kruskal_processing(int _size, bool** _graph, int** _cost);
};

typedef  pair<pair<int, int>,  int > PAIR;
// Define comparing function in sort function.
int cmp( const  PAIR &x,  const  PAIR &y) {  
	return  x.second < y.second;  
}

Kruskal_processing::Kruskal_processing(int _size, bool** _graph, int** _cost) {
	// Use multimap and map to save coordinates & edges.
    	multimap<int, pair<int, int>> temp_set;
	map<pair<int, int>, int> tree_set;
	vector<PAIR> pair_vec, temp_vec;
	
	for (int i = 0; i < (_size - 1); ++i) {
		for (int j = (i + 1); j < _size; ++j) {
			if (_graph[i][j]) {
				tree_set.emplace(make_pair(i, j), _cost[i][j]);
			}
		}
	}
	
	// pair_vec can load all information from tree_set.
	for (auto &iter : tree_set) {
	    pair_vec.push_back(make_pair(make_pair(iter.first.first, iter.first.second), iter.second));
	}
	
	// Classic Kruskal's algorithm: To detect where existing potetial closing-loop & delete the maximum edge from tree_set.
	// The advanced details show as below:
	// 1. Find potetial closing-loop coordinates, for example, (0, 1) = 3, (0, 3) = 5 & (1, 3) = 4. Amog these coordinates, there contain both 0
	// in (0, 1) and (0, 3). We can assume that there must exist (1, 3) if (1, 3) really exist edge forming  closing-loop with 
	// (0, 1) and (0, 3).
	// 2. We delete the maximum edge from tree set, so we remove (0, 3) = 5.
	// 3. Find & delete all potetial closing-loop coordinates. Finally, sort tree_set & accumulate n-1 edge values.
	
	for (auto &iter : pair_vec) {
		for (auto &iter2 : pair_vec) {
            		if ((iter.first.first == iter2.first.first) && (_graph[iter2.first.second][iter.first.second] || _graph[iter.first.second][iter2.first.second])) {
            		    temp_set.emplace(iter.second, make_pair(iter.first.first, iter.first.second));
            			temp_set.emplace(_cost[iter.first.first][iter2.first.second], make_pair(iter.first.first, iter2.first.second));
            			
            			if (iter.first.second < iter2.first.second) {
            			    temp_set.emplace(_cost[iter.first.second][iter2.first.second], make_pair(iter.first.second, iter2.first.second));
            			} else {
            			    temp_set.emplace(_cost[iter2.first.second][iter.first.second], make_pair(iter2.first.second, iter.first.second));
            			}
            			
            			tree_set.erase(make_pair(temp_set.rbegin()->second.first, temp_set.rbegin()->second.second));
            			temp_set.clear();
            		}
		}
	}
	
	pair_vec.clear();
	
	for (auto &iter : tree_set) {
	    pair_vec.push_back(make_pair(make_pair(iter.first.first, iter.first.second), iter.second));
	}
	
	sort(pair_vec.begin(), pair_vec.end(), cmp);
	
	// Print result.
	int count = 0;
	cout << "=========================" << endl;
	cout << "--Minimum-spanning-tree--" << endl;
	cout << "=========================" << endl;
	cout << setw(8) << "Edge      |" << setw(4) << "Distance" << endl;
	cout << "=========================" << endl;
	for (auto iter = pair_vec.begin(); iter != (pair_vec.begin() + _size - 1); iter++) {
	    cout << setw(3) << iter->first.first << "<->" << setw(3) << iter->first.second << " |" << iter->second << endl;
	    count += iter->second;
	}
	cout << "=========================" << endl;
	cout << setw(8) << "Summary  :" << setw(3) << count << endl;
	cout << "=========================" << endl;
	
	// Output file.
	ofstream output("MST.csv", ios::out|ios::trunc);
	if(output.is_open()) {
	    output << "MST," << count << endl;
	    output << "Node_1," << "Node_2," << "Distance" << endl;
		for (auto iter = pair_vec.begin(); iter != (pair_vec.begin() + _size - 1); iter++) {
	        output << iter->first.first << "," << iter->first.second << "," << iter->second << endl;
	    }
	}
	
	output.close();
}

// Print & save the graph.
void Kruskal_processing::print_generate_graph(int _size, bool** _graph, int** _cost) {
	cout << "===================" << endl;
	cout << "--Generate--graph--" << endl;
	cout << "===================" << endl;
	cout << setw(8) << "Edge      |" << setw(4) << "Distance" << endl;
	cout << "===================" << endl;
	for (int i = 0; i < (_size - 1); ++i) {
		for (int j = (i + 1); j < _size; ++j) {
			if (_graph[i][j])
				cout << setw(3) << i << "<->" << setw(3) << j << " |" << setw(8) << _cost[i][j] << endl;
		}
		cout << "===================" << endl;
	}
	
	ofstream output("Graph.csv", ios::out|ios::trunc);
	if(output.is_open()) {
	    output << "Node_1," << "Node_2," << "Distance" << endl;
		for(int i = 0; i < (_size - 1); ++i) {
        			for(int j = (i + 1); j < _size; ++j) {
        				if(_graph[i][j]) {
            				output << i << "," << j << "," << _cost[i][j] << endl;
        				}
        			}
    		}
	}
	
	output.close();
}

tuple<int, bool**, int**> generate_graph(Kruskal_processing& DJ) {
	int size = 10;
	double density = 0.5;
	cout << "========================================" << endl;
	cout << "--Set-graph-size-(Integer > 0; e.g. 6)--" << endl;
	cin >> size;
	cout << "--Set-graph-density-(0 to 1; e.g. 0.5)--" << endl;
	cin >> density;
	cout << "========================================" << endl;
	
	cout <<  endl;
	
	srand(time(0));
	DJ.graph = new bool*[size];
	DJ.cost = new int*[size];

	for (int i = 0; i < size; i++) {
		DJ.graph[i] = new bool[size];
		DJ.cost[i] = new int[size];
	}
	
	// To determine where edge can form by random function.
	for (int i = 0; i < (size - 1); ++i) {
		for (int j = (i + 1); j < size; ++j) {
				DJ.graph[i][j] = (prob() < density);
		}
		DJ.graph[i][(i + 1)%size] = DJ.graph[(i + 1)%size][i] = true;
	}
	
	// To endue edge values by random function.
	for (int i = 0; i < (size - 1); ++i) {
		for (int j = (i + 1); j < size; ++j) {
			if (DJ.graph[i][j]) {
				DJ.cost[i][j] = DJ.cost[j][i] = prob()*30 + 1;
			}
		}
	}
	
	return make_tuple(size, DJ.graph, DJ.cost);
}

int main() {
	Kruskal_processing DJ;
	auto gg = generate_graph(DJ);
	
	DJ.print_generate_graph(get<0>(gg), get<1>(gg), get<2>(gg));
	
	cout <<  endl;
	
	Kruskal_processing(get<0>(gg), get<1>(gg), get<2>(gg));
	//system("Pause");
	return 0;
}

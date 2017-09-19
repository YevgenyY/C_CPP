//
// This is an implementation of MST algorythm

#include <iostream>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

int main()
{
	ifstream graph_file("SampleTestData_mst_data.txt");
	istream_iterator<int> start(graph_file), end;

	vector<int> words(start, end);

	for (auto str : words)
		cout << str << " ";
	cout << endl;

	sort(words.begin(), words.end());
	for (auto str : words)
		cout << str << " ";
	cout << endl;

	int data[4] = {1,2,3,4};
	cout << endl << accumulate(data, data + 1,5) << endl;
}

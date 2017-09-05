//* Convert this program to C++
 
// * change to C++ io
 
// * change to one line comments
 
// * change defines of constants to const
 
// * change array to vector<>
 
// * inline any short function
 
//

#include <iostream>
#include <vector>
using namespace std;

// inline function to sum up all elements of a vector

inline void sum(int* p, int n, vector<int> d)

{
    
    int i;
    *p = 0;
    for(i = 0; i < n; ++i)
        *p = *p + d[i];
}

int main()

{
    int i;
    int accum = 0;
    const int N = 40; // * change defines of constants to const
    vector<int> data(N);
    for (i = 0; i < N; ++i)
        data[i] = i;
    sum(&accum, N, data);
    cout << "sum is " << accum << endl;
    return 0;
}

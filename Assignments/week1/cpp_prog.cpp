#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int N = 40; // it's a constant variable instead of
				  // preprocessor definition in C
				  
template <class T>
inline void sum(T &acc, T n, const T d[])
{
	acc = 0;
	for (T i = 0; i < n; ++i)
	{
		acc = acc + d[i];
	}
}

int main(void)
{
	int accum = 0;
	int data[N];

	for(int i=0; i < N; ++i)
		data[i] = i;

	sum(accum, N, data);

	cout << "sum is " << accum << endl;
}

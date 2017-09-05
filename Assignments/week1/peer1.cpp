// 
// Compute sum of array elements
// 
// Execution result example for 40 elements:
// 
// C:\Users\vs\source\repos\summary\Debug>summary.exe
//  sum is 780
//

#include <vector>
#include <numeric>
#include <iostream>

// in separate namespace we declare:
//   (a) base_type as our base data type
//   (b) vector_type using std::vector<base_type> 
//   (c) vector_type::size_type as size_type
//   (d) vector_size is equal to the 'N' constant in legacy C code

namespace homework {                                    
    typedef int base_type;                              
    typedef std::vector<base_type> vector_type;         
    typedef vector_type::size_type size_type;           
    const size_type vector_size = 40U;           
}

using namespace homework;

int main()
{
    // define vector of vector_size elements and sum variable
    vector_type vector(vector_size);                           
    base_type sum;

    // initialize vector elements with [0..vector_size)
    std::iota(vector.begin(), vector.end(), static_cast<base_type>(0));

    // compute sum of all vector elements and display it
    sum = std::accumulate(vector.begin(), vector.end(), static_cast<base_type>(0));

    std::cout << "sum is " << sum;						
    std::cout << std::endl;

    // clean return
    return (0);
}

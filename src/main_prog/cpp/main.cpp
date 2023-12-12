#include <iostream>

#include "xuzy_math.h"

using std::cout;
using std::endl;

int main(){
    int result = xuzy::add(12,56);
    cout << "12 + 56 = " << result << endl;
    return EXIT_SUCCESS;
}
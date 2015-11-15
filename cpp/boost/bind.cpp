#include <iostream>
#include <boost/bind.hpp>
using namespace std;

int f(int x, int y) {
    return x + y;
}
int main() {
    cout << boost::bind(f, _1, _2) (1, 2) << endl;

    // C++11
    auto g = boost::bind(f, _1, _2);
    cout << g(2, 3) << endl;

    return 0;
}


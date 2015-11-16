#include <iostream>
#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace std;

int f(int x, int y) {
    return x + y;
}
int main() {
    cout << boost::bind(f, _1, _2) (1, 2) << endl;

    // C++11 auto
    auto g = boost::bind(f, _1, _2);
    cout << g(2, 3) << endl;

    // or boost function
    boost::function<int (int, int)> h = boost::bind(f, _1, _2);
    cout << h(3, 3) << endl;

    return 0;
}


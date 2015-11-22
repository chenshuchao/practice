#include<stdio.h>
#include<iostream>
#include<memory>

using namespace std;
class Foo
{
 public:
  Foo(string key) {
    name = key;
  }
  ~Foo() {
    cout << "Foo die" << endl;
  }
  string name;
};

void test_shared_ptr() {
  shared_ptr<int> p1;
  if (p1 == NULL) {
    printf("p1 is null.\n");
  }

  shared_ptr<int> p2 = make_shared<int>(1);
  printf("%s\n", p2.unique() ? "true" : "false");
}

int main() {
  test_shared_ptr();
  
  shared_ptr<Foo> sp(new Foo("hahah"));
  sp.reset();

  cout << "The End" << endl;
}


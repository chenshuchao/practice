#include<memory>
#include<stdio.h>

using namespace std;

void destruct_callback() {
  printf("I am dying ...\n");
}
void test1() {
  shared_ptr<int> p1(new int(1), bind(destruct_callback));
}
int main() {
  test1();
}


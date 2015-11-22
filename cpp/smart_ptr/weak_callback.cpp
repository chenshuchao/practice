#include<iostream>
#include<memory>
#include<assert.h>
#include<string>

using namespace std;
using namespace std::placeholders;

class Soldier
{
 public:
  Soldier(const string name) : name_(name){}
  string getName() { return name_;}
 private:
  string name_;
};

class Battalion : public enable_shared_from_this<Battalion>
{
 public:
  Battalion() : count_(0) {}
  shared_ptr<Soldier> dispatch(const string& name)
  {
    shared_ptr<Soldier> s(new Soldier(name),
                             bind(&Battalion::decrease,
                                  weak_ptr<Battalion>(shared_from_this()),
                                  _1)); 
    count_ ++;
    return s;
  }
  int getCount() { return count_; }
  void reduce() { count_ --; } 
  static void decrease(weak_ptr<Battalion>& battalion, Soldier *soldier)
  {
    shared_ptr<Battalion> sp = battalion.lock();
    if (sp)
    {
      cout << soldier->getName() << "已经壮烈牺牲了！" << endl;
      sp->reduce();
    }
  }
 private:
  int count_;
};


void test1()
{
  shared_ptr<Battalion> battalion(new Battalion);
  shared_ptr<Soldier> s1 = battalion->dispatch("s1");
  cout << battalion->getCount() << endl;
  shared_ptr<Soldier> s2 = battalion->dispatch("s2");
  cout << battalion->getCount() << endl;

  // 士兵阵亡
  s1.reset();  
  cout << battalion->getCount() << endl;
  s2.reset();
  cout << battalion->getCount() << endl;

  cout << "The End" << endl;
}
void test2()
{
  shared_ptr<Battalion> battalion(new Battalion);
  shared_ptr<Soldier> s1 = battalion->dispatch("s1");
  cout << battalion->getCount() << endl;
  shared_ptr<Soldier> s2 = battalion->dispatch("s2");
  cout << battalion->getCount() << endl;

  // 军营先毁灭, 实际上battalion指向对象没有销毁
  battalion.reset();

  // 士兵阵亡 true
  s1.reset();  
  s2.reset();  
  
  cout << "The End" << endl;
}

int main() 
{
  test1(); // true
  test2(); // true;
}


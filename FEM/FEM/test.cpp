#include <iostream>
#include <fstream>
#include <string>

using namespace std;

  class A
  {
  protected:
    int x;
  public:
    A() {setx(5);}
    int printx() {return x;}
    void setx(int d) {x = d;}
    void multiple(int d) {x = x * d;}
  };

  class B : public A
  {
  public:
    void add(int d) {x = x + d;}
    int getx() {return x;}
  };

int main()
{
    A a;
    a.setx(5);
    cout << "Base: " << a.printx() << endl;

    B b;
    b.setx(5);
    cout << "Derived: " << b.printx() << endl;
    b.multiple(3);
    cout << "Derived: " << b.printx() << endl;
    b.add(11);
    cout << "Derived: " << b.printx() << endl;
    cout << "Derived: " << b.getx() << endl;
    A c;
    cout << "Base: " << c.printx() << endl;
}

// Result, change made in derived class wont' affect base class's instance, or other derived class's instance!

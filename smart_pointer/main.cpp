/*
#include <iostream>
#include <memory>

class A {
    public:
        A(int a_, int b_): a(a_), b(b_) {

        }

        int a;
        int b;
};

void func(std::shared_ptr<A> &ptr) {
    std::cout << ptr->a << ", " << ptr->b << std::endl;
    
    ptr->a = 100;
    ptr->b = 200;

    return;
}

int main() {
    std::shared_ptr<A> ptr = std::make_shared<A>(10, 20);

    func(ptr);

    std::cout << ptr->a << ", " << ptr->b << std::endl;

    return 0;
}
*/

#include <iostream>
#include <memory>

using namespace std;

struct Person {
  int age;
  string name;
  int a;
  int b;
};

int main() {
  // 创建一个智能指针
  shared_ptr<Person> ptr = make_shared<Person>();
  shared_ptr<Person> p1 = nullptr;

  // 使用 operator[]() 操作符访问智能指针指向的对象的成员
  ptr->age = 10;
  ptr->name = "John Doe";
  ptr->a = 100;

  auto p2 = make_shared<Person>(*ptr);

  // 输出智能指针指向的对象的成员值
  cout << p2->age << endl; // 10
  cout << p2->name << endl; // John Doe
  cout << p2->a << endl; // John Doe

  return 0;
}

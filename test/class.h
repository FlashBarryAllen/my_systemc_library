#ifndef __CLASS_H__
#define __CLASS_H__

#include <iostream>


class AA {
public:
    AA() {}
    ~AA() {}

    virtual int fun_a() {
        std::cout << "base AA: fun_a" << std::endl;
        return a;
    }

public:
    int a;
};

class BB {
public:
    BB() {}
    ~BB() {}

public:
    int b;
};

class A {
public:
    A() {}
    ~A() {}

    void set_val(int v) {
        val = v;
    }

    void set_b(int v) {
        bb.b = v;
    }

    int get_b() {
        return bb.b;
    }

    void set_aa() {
        aa.a = bb.b;
    }

public:
    AA aa;
    BB bb;
    int val;
};

class B : public A {
public:
    B() {}
    ~B() {}

    /*
    void fun() {
        std::cout << "B: fun()" << std::endl;
    }
    */

public:

};

class Outer {
public:
  int a;
  void f() {}

private:
  int b;
  void g() {}

  class Inner {
  public:
    void h() {
      // 直接访问外部类的成员
      //std::cout << a << std::endl;
      //f();

      // 使用作用域解析运算符访问外部类的成员
      //std::cout << Outer::b << std::endl;
      //Outer::g();
    }
  };
};

#endif
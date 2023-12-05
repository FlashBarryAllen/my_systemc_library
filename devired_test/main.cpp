#include <iostream>

class A {
    public:
        virtual void f1() {
            std::cout << "A::f1()" << std::endl;
            int* a = new int(10);
            f2(a);
            int b = 10;
            //f2(b);
        }

        virtual void f2(void* payload) {
            std::cout << "A::f2()" << std::endl;
        }
};

class AA : public A {
    public:
        /*
        virtual void f1() {
            std::cout << "AA::f1()" << std::endl;
        }
        */

        virtual void f2(void* payload) {
            std::cout << "AA::f2()" << std::endl;
        }
};

int main() {
    AA* aa = new AA();
    aa->f1();
    return 0;
}
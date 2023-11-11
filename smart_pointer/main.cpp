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
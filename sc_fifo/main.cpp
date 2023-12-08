#include <iostream>
#include "scc_sc_fifo.h"

int main() {
    sc_component::sc_fifo<int> test;

    test.nb_write(1);
    test.nb_write(12);
    test.nb_write(9);

    int val = 0;
    test.nb_read(val);
    std::cout << val << std::endl;
    test.nb_read(val);
    std::cout << val << std::endl;
    test.nb_read(val);
    std::cout << val << std::endl;

    return 0;
}
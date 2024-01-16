#include <iostream>
#include <memory>
#include "scc_stat.h"

int main() {
    std::string str;
    my_scc::stat_pps test;

    test.stat(1);
    test.stat(1);
    test.stat(1);
    test.stat(1);
    test.stat(1);

    str = test.c_str();

    std::cout << str << std::endl;

    test.stat(1);
    test.stat(1);
    test.stat(1);
    test.stat(1);
    test.stat(1);

    str = test.c_str();

    std::cout << str << std::endl;

    return 0;
}
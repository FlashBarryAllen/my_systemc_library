#include <systemc>

int sc_main(int, char**) {
    sc_dt::sc_int<32> x = -1;
    std::cout << x << std::endl;

    int a = 10;

    while (true)
    {
        a = a + 2;
    }

    return 0;
}
#include "top.h"

int sc_main(int args, char* argv[])
{
    std::cout << "hello server cpu" << std::endl;

    //TEST_islip();

    top srv_top("srv_top");

    sc_start(10, sc_core::SC_NS);
    
    return 0;
}
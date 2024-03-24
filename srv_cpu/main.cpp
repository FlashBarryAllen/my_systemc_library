#include "top.h"

int sc_main(int args, char* argv[])
{
    std::cout << "hello server cpu" << std::endl;
    top srv_top("srv_top");

    sc_start(10, sc_core::SC_NS);
    
    return 0;
}
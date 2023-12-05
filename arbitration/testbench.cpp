#include "top.h"

using namespace sc_core;

int sc_main(int argc, char* argv[]) {
    top my_top("my_top");
    
    sc_start(10, SC_NS);

    return 0;
}
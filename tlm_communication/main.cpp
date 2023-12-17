#include "top.h"

int sc_main(int argc, char* argv[]) {
    tlm_communication::top* t = new tlm_communication::top("top");

    sc_core::sc_start(10, sc_core::SC_NS);

    return 0;
}
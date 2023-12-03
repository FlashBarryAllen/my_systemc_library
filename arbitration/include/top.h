#ifndef TOP_H
#define TOP_H

#include "src.h"
#include "dst.h"

class top : public sc_core::sc_module {
public:
    SC_HAS_PROCESS(top);
    top(sc_core::sc_module_name name) {
        a = new src("a");
        b = new dst("b");

        a->m_clk(clk);
        b->m_clk(clk1);

        a->src_socket.bind(b->dst_socket);
    }

    src* a;
    dst* b;
    sc_core::sc_clock clk = sc_core::sc_clock("clock", 1, sc_core::SC_NS);
    sc_core::sc_clock clk1 = sc_core::sc_clock("clock1", 2, sc_core::SC_NS);
};

#endif
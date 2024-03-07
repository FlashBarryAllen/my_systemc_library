#ifndef TOP_H
#define TOP_H

#include "class_a.h"
#include "class_b.h"

namespace tlm_communication
{

class top : public sc_core::sc_module {
public:
    SC_HAS_PROCESS(top);
    top(sc_core::sc_module_name name) {
        aa = new A("aa");
        bb = new B("bb");
        m_clk = new sc_core::sc_clock("clock", 1, sc_core::SC_NS);

        aa->src.bind(bb->dst);
        bb->src.bind(aa->dst);

        aa->m_clk(*m_clk);
        bb->m_clk(*m_clk);
    }

public:
    A* aa;
    B* bb;
    sc_core::sc_clock* m_clk;
};

}

#endif
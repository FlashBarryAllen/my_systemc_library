#ifndef TOP_H
#define TOP_H

#include "initiator.h"
#include "target.h"

SC_MODULE(Top)
{
    Initiator* init;
    Target*    target;
    sc_clock clk = sc_clock("clock", 1, SC_NS);

    SC_CTOR(Top)
    {
        init = new Initiator("init");
        target = new Target("target");
        target->m_clk(clk);

        init->socket.bind( target->socket );
    }
};

#endif

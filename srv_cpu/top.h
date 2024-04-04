#ifndef TOP_H
#define TOP_H

#include <memory>
#include "src.h"
#include "tgt.h"

class my_src : public src
{
public:
    my_src(sc_core::sc_module_name name) : src(name) {

    }
    /*
    virtual void mth_entry() {
        std::cout << "my_src" << std::endl;
    }
    */
};

class top : public sc_core::sc_module
{
    public:
        SC_HAS_PROCESS(top);
        top(sc_core::sc_module_name name) {
            m_clk = new sc_core::sc_clock("clk", 1, sc_core::SC_NS);
            s = std::make_shared<my_src>("my_src");
            t = std::make_shared<tgt>("tgt");
            s->m_clk(*m_clk);
            t->m_clk(*m_clk);
            s->m_src_sk.bind(t->m_tgt_sk);
            s->m_credit = 2;
        }

    public:
        sc_core::sc_clock* m_clk;
        std::shared_ptr<my_src> s;
        std::shared_ptr<tgt> t;
};

#endif
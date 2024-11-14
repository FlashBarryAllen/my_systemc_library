#ifndef TOP_H
#define TOP_H

#include <memory>
#include "islip.h"
#include "src.h"
#include "test.h"
#include "tgt.h"

class my_src : public src {
   public:
    my_src(sc_core::sc_module_name name) : src(name) {}
    /*
    virtual void mth_entry() {
        std::cout << "my_src" << std::endl;
    }
    */
};

class top : public sc_core::sc_module {
   public:
    SC_HAS_PROCESS(top);
    top(sc_core::sc_module_name name) {
        m_clk = new sc_core::sc_clock("clk", 1, sc_core::SC_NS);
        s = std::make_shared<my_src>("my_src");
        t = std::make_shared<tgt>("tgt");
        s->m_clk(*m_clk);
        t->m_clk(*m_clk);
        s->m_src_tx.bind(t->m_tgt_rx);
        t->m_tgt_tx.bind(s->m_src_rx);
        s->m_credit = 2;
    }

   public:
    sc_core::sc_clock* m_clk;
    std::shared_ptr<my_src> s;
    std::shared_ptr<tgt> t;
};

class A : public sc_core::sc_module {
   public:
    sc_core::sc_in_clk clock;
    sc_fifo_out<int> tx;
    int cnt = 0;

   public:
    SC_HAS_PROCESS(A);
    A(sc_core::sc_module_name name) {
        SC_METHOD(mth_entry);
        sensitive << clock.pos();
    }

    void mth_entry() {
        cnt++;
        static int i = 0;
        tx->nb_write(i);
        std::cout << "cnt: " << cnt << ", AA: " << i << std::endl;
        i++;
    }
};

class BB : public sc_core::sc_module {
   public:
    sc_core::sc_in_clk clock;
    sc_fifo_in<int> rx;
    int cnt = 0;

   public:
    SC_HAS_PROCESS(BB);
    BB(sc_core::sc_module_name name) {
        SC_METHOD(mth_entry);
        sensitive << clock.pos();
    }

    void mth_entry() {
        cnt++;
        int i = 0;
        while (rx->num_available()) {
            // rx->nb_read(i);
            i = rx->read();
            std::cout << "cnt: " << cnt << ", BB: " << i << std::endl;
        }
    }
};

#endif
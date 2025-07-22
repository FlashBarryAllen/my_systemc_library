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

class CC : public sc_core::sc_module {
    public:
        SC_HAS_PROCESS(CC);
        CC(sc_core::sc_module_name name)
        {
            SC_METHOD(mth_entry);
            sensitive << clk.pos();
            dont_initialize();
        }

    public:
        sc_in_clk clk;
        tlm_utils::simple_initiator_socket<CC> tx;
        int cnt = 0;
    
    public:
        void mth_entry()
        {
            static int i = 100;
            int* data = new int(i);
            tlm::tlm_generic_payload payload;
            payload.set_data_ptr((unsigned char*)data);
            int* snd_data = (int*)payload.get_data_ptr();
            tlm::tlm_phase phase = tlm::BEGIN_REQ;
            sc_core::sc_time time = sc_core::SC_ZERO_TIME;

            std::cout << "cnt: " << cnt << ", CC: snd " << *snd_data << std::endl;

            tx->nb_transport_fw(payload, phase, time);

            i++;
            cnt++;
        }
};

class DD : public sc_core::sc_module {
    public:
        SC_HAS_PROCESS(DD);
        DD(sc_core::sc_module_name name)
        {
            SC_METHOD(mth_entry);
            sensitive << clk.pos();
            dont_initialize();

            rx.register_nb_transport_fw(this, &DD::nb_transport_fw);
        }

    public:
        sc_in_clk clk;
        tlm_utils::simple_target_socket<DD> rx;
        int cnt = 0;
        little_end::peq<int> m_peq;
    
    public:
        void mth_entry()
        {
            std::shared_ptr<int> data = nullptr;
            if ((data = m_peq.get_next_transaction()) != nullptr)
            {
                std::cout << "cnt: " << cnt << ", DD peq pop: " << *data << std::endl;
            }

            cnt++;
        }

        tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& payload,
            tlm::tlm_phase& phase, sc_core::sc_time& time)
        {
            int cycle = 1;
            int* data = (int*)payload.get_data_ptr();

            std::cout << "cnt: " << cnt << ", DD: rcv " << *data << " and delay cycle: "
                << cycle << std::endl;
            m_peq.delay(*data, cycle);

            delete data;
            return tlm::TLM_COMPLETED;
        }
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
        s->m_credit = 1000;
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
        dont_initialize();
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
        dont_initialize();
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
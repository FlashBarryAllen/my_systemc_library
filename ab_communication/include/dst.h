#ifndef DST_H
#define DST_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <queue>

class dst : public sc_core::sc_module {
public:    
    SC_HAS_PROCESS(dst);
    dst(sc_core::sc_module_name name) : dst_socket("dst_socket") {
        dst_socket.register_nb_transport_fw(this, &dst::nb_transport_fw);
        SC_METHOD(mth_entry);
        dont_initialize();
        sensitive << m_clk.pos();
    }

    void mth_entry() {
        tlm::tlm_generic_payload* trans = m_queue.front();
        m_queue.pop_front();
        tlm::tlm_phase phase = tlm::BEGIN_REQ;
        sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

        dst_socket->nb_transport_bw(*trans, phase, delay);
    }

    virtual tlm::tlm_sync_enum nb_transport_fw( tlm::tlm_generic_payload& trans,
                                                tlm::tlm_phase& phase, sc_core::sc_time& delay ) {
        m_queue.emplace_back(&trans);

        return tlm::TLM_COMPLETED;
    }

public:
    sc_core::sc_in_clk m_clk;
    tlm_utils::simple_target_socket<dst> dst_socket;
    std::deque<tlm::tlm_generic_payload*> m_queue;
};

#endif
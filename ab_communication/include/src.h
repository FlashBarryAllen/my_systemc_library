#ifndef SRC_H
#define SRC_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <memory>

class src : public sc_core::sc_module {
public:    
    SC_HAS_PROCESS(src);
    src(sc_core::sc_module_name name) : src_socket("src_socket"), val(0) {
        src_socket.register_nb_transport_bw(this, &src::nb_transport_bw);
        SC_METHOD(mth_entry);
        sensitive << m_clk.pos();
    }

    void mth_entry() {
        val++;
        int* dat = new int(10 + val);

        tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload();
        trans->set_command(tlm::TLM_WRITE_COMMAND);
        trans->set_data_ptr((unsigned char*)dat);
        trans->set_data_length(4);

        tlm::tlm_phase phase = tlm::BEGIN_REQ;
        sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

        int status = src_socket->nb_transport_fw(*trans, phase, delay);
    }

    virtual tlm::tlm_sync_enum nb_transport_bw( tlm::tlm_generic_payload& trans,
                                                tlm::tlm_phase& phase, sc_core::sc_time& delay ) {
        tlm::tlm_command cmd = trans.get_command();
        unsigned char* dat   = trans.get_data_ptr();

        std::cout << sc_core::sc_time_stamp() << " dat = " << *reinterpret_cast<int*>(dat) << std::endl;

        delete dat;
        delete &trans;
        
        return tlm::TLM_COMPLETED;
    }

public:
    int val;
    sc_core::sc_in_clk m_clk;
    tlm_utils::simple_initiator_socket<src> src_socket;
};

#endif
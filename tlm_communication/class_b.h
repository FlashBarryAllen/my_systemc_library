#ifndef CLASS_B_H
#define CLASS_B_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

namespace tlm_communication
{

class B : public sc_core::sc_module
{
public:
    SC_HAS_PROCESS(B);
    B(sc_core::sc_module_name name) : src("src"), dst("dst") {
        dst.register_nb_transport_fw(this, &B::nb_transport_fw);

        SC_METHOD(mth_entry);
        sensitive << m_clk.pos();
    }

    void mth_entry() {
        static int val = 200;
        val++;
        int* data = new int(val);
        tlm::tlm_generic_payload trans;
        tlm::tlm_phase phase = tlm::BEGIN_REQ;
        sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

        trans.set_data_ptr((unsigned char*) data);

        src->nb_transport_fw(trans, phase, delay);
    }

    virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase,
        sc_core::sc_time& delay) {
        unsigned char* data = trans.get_data_ptr();

        std::cout << *(int*)data << std::endl;
        //std::cout << static_cast<int>(data) << std::endl;

        return tlm::TLM_COMPLETED;
    }

    ~B() {

    }

public:
    tlm_utils::simple_initiator_socket<B> src;
    tlm_utils::simple_target_socket<B>    dst;

    sc_core::sc_in_clk  m_clk;
};

}

#endif
#ifndef SRC_H
#define SRC_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

class src : public sc_core::sc_module
{
    public:
        SC_HAS_PROCESS(src);
        src(sc_core::sc_module_name name) : m_src_sk("src_sk") {
            m_src_sk.register_nb_transport_bw(this, &src::nb_transport_bw);
            SC_METHOD(mth_entry);
            sensitive << m_clk.pos();
        }

        void mth_entry() {

        }

        virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase,
            sc_core::sc_time& time) {
            return tlm::TLM_COMPLETED; 
        }

    public:
        sc_core::sc_in_clk m_clk;
        tlm_utils::simple_initiator_socket<src> m_src_sk;
};

#endif
#ifndef TGT_H
#define TGT_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

class tgt : public sc_core::sc_module
{
    public:
        SC_HAS_PROCESS(tgt);
        tgt(sc_core::sc_module_name name) : m_tgt_sk("tgt_sk") {
            m_tgt_sk.register_nb_transport_fw(this, &tgt::nb_transport_fw);
            SC_METHOD(mth_entry);
            sensitive << m_clk.pos();
        }

        void mth_entry() {

        }

        virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase,
            sc_core::sc_time& time) {
            return tlm::TLM_COMPLETED;
        }

    public:
        sc_core::sc_in_clk m_clk;
        tlm_utils::simple_target_socket<tgt> m_tgt_sk;
};

#endif
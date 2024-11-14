#ifndef SRC_H
#define SRC_H

#include <time.h>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "common_type.h"

class src : public sc_core::sc_module
{
    public:
        SC_HAS_PROCESS(src);
        src(sc_core::sc_module_name name);

        virtual void mth_entry();
        void cal_speed();
        void set_credit(int credit);
        virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase,
            sc_core::sc_time& time);

    public:
        int m_cycle_cnt;
        int m_val;
        int m_credit;
        struct timespec m_now_ts;
        sc_core::sc_in_clk m_clk;
        tlm_utils::simple_initiator_socket<src> m_src_tx;
        tlm_utils::simple_target_socket<src> m_src_rx;
        std::shared_ptr<spdlog::logger> m_logger;
};

#endif
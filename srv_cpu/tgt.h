#ifndef TGT_H
#define TGT_H

#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <systemc>
#include <tlm>
#include "common_type.h"
#include "peq.h"

class tgt : public sc_core::sc_module {
   public:
    SC_HAS_PROCESS(tgt);
    tgt(sc_core::sc_module_name name);

    void mth_dat_proc();
    void mth_get_peq();
    void mth_snd_msg();
    void mth_entry();
    virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans,
                                               tlm::tlm_phase& phase,
                                               sc_core::sc_time& time);

   public:
    int m_cycle_cnt;
    int m_crdt_sch_cycle;
    little_end::peq<MY_DAT_T> m_peq;
    little_end::peq<MY_API_T> m_peq_ctl;
    sc_core::sc_in_clk m_clk;
    tlm_utils::simple_initiator_socket<tgt> m_tgt_tx;
    tlm_utils::simple_target_socket<tgt> m_tgt_rx;
    deque<shared_ptr<MY_DAT_T>> m_rcv_dat;
    deque<shared_ptr<MY_API_T>> m_snd_ctl;
    deque<shared_ptr<MY_DAT_T>> m_peq_que;
    deque<shared_ptr<MY_DAT_T>> m_snd_msg_que;
    deque<int> m_peq_ctl_stat;
    std::shared_ptr<spdlog::logger> m_logger;
};

class B : public sc_core::sc_module {
   public:
    SC_HAS_PROCESS(B);
    B(sc_core::sc_module_name name);
    ~B();
    void mth_entry();
    virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans,
                                               tlm::tlm_phase& phase,
                                               sc_core::sc_time& time);

   public:
    sc_core::sc_in_clk m_clk;
    tlm_utils::simple_target_socket<B> m_tlm_if;
};

#endif
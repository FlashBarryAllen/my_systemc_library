#include "tgt.h"

tgt::tgt(sc_core::sc_module_name name) : m_crdt_sch_cycle(1), m_cycle_cnt(0) {
    m_tgt_rx.register_nb_transport_fw(this, &tgt::nb_transport_fw);
    SC_METHOD(mth_entry);
    sensitive << m_clk.pos();
    dont_initialize();

    m_logger = spdlog::basic_logger_mt(
        this->name(), std::string("logs/") + this->name() + ".log", true);
    m_logger->set_level(spdlog::level::info);
    m_logger->set_pattern("[%Y-%m-%d %H:%M:%S] %v");
}

void tgt::mth_dat_proc() {
    m_logger->info("cycle: {:d}, [TGT] credit fifo depth: {:d}.", m_cycle_cnt, m_peq_ctl_stat.size());
    if (m_rcv_dat.size() != 0) {
        auto p_dat = m_rcv_dat.front();
        m_peq.delay(*p_dat, 6);
        auto p_ctl = make_shared<MY_API_T>();
        p_ctl->msg.type = CTRL_MSG;
        p_ctl->msg.credit = 1;
        m_peq_ctl.delay(*p_ctl, 6);
        m_peq_ctl_stat.push_back(1);
        m_logger->info("cycle: {:d}, [TGT] credit delay 6 cycles.", m_cycle_cnt);
        m_rcv_dat.pop_front();
    }
}

void tgt::mth_get_peq() {
    shared_ptr<MY_DAT_T> p_dat = nullptr;
    while ((p_dat = m_peq.get_next_transaction()) != nullptr) {
        // std::cout << "sc_time_stamp: " << sc_time_stamp() << std::endl;
        m_peq_que.push_back(p_dat);
    }

    shared_ptr<MY_API_T> p_ctl = nullptr;
    while ((p_ctl = m_peq_ctl.get_next_transaction()) != nullptr) {
        // std::cout << "sc_time_stamp: " << sc_time_stamp() << std::endl;
        m_snd_ctl.push_back(p_ctl);
        m_peq_ctl_stat.pop_front();
    }
}

void tgt::mth_snd_msg() {
    if (m_snd_ctl.size() != 0) {
        auto p_ctl = m_snd_ctl.front();

        tlm::tlm_generic_payload trans;
        auto p_api = new MY_API_T();
        p_api->msg.type = p_ctl->msg.type;
        p_api->msg.credit = p_ctl->msg.credit;

        trans.set_data_ptr((unsigned char*)p_api);
        tlm::tlm_phase phase = tlm::BEGIN_REQ;
        sc_time time = SC_ZERO_TIME;

        m_tgt_tx->nb_transport_fw(trans, phase, time);

        m_snd_ctl.pop_front();
        m_logger->info("cycle: {:d}, [TGT] snd credit.", m_cycle_cnt);
    }

    if (m_peq_que.size() != 0) {
        auto p_dat = m_peq_que.front();
        m_snd_msg_que.push_back(p_dat);
        m_peq_que.pop_front();
    }

    if (m_snd_msg_que.size() != 0) {
        tlm::tlm_generic_payload trans;
        auto p_api = new MY_API_T();
        p_api->msg.type = DATA_MSG;
        p_api->dat = m_snd_msg_que.front();
        trans.set_data_ptr((unsigned char*)p_api);
        tlm::tlm_phase phase = tlm::BEGIN_REQ;
        sc_time time = SC_ZERO_TIME;

        auto status = m_tgt_tx->nb_transport_fw(trans, phase, time);
        if (status == tlm::TLM_COMPLETED) {
            m_snd_msg_que.pop_front();
        } else {
        }

        m_logger->info("cycle: {:d}, [TGT][SND]", m_cycle_cnt);
    }
}

void tgt::mth_entry() {
    m_cycle_cnt++;

    mth_dat_proc();
    mth_get_peq();
    mth_snd_msg();
}

tlm::tlm_sync_enum tgt::nb_transport_fw(tlm::tlm_generic_payload& trans,
                                        tlm::tlm_phase& phase,
                                        sc_core::sc_time& time) {
    auto p_api = (MY_API_T*)trans.get_data_ptr();
    auto p_dat = std::static_pointer_cast<MY_DAT_T>(p_api->dat);
    m_rcv_dat.push_back(p_dat);
    delete p_api;

    m_logger->info("cycle: {:d}, [TGT][RCV]", m_cycle_cnt);
    return tlm::TLM_COMPLETED;
}

B::B(sc_core::sc_module_name name) {
    m_tlm_if.register_nb_transport_fw(this, &B::nb_transport_fw);
    SC_METHOD(mth_entry);
    sensitive << m_clk.pos();
}

B::~B() {}

void B::mth_entry() { return; }

tlm::tlm_sync_enum B::nb_transport_fw(tlm::tlm_generic_payload& trans,
                                      tlm::tlm_phase& phase,
                                      sc_core::sc_time& time) {
    return tlm::TLM_COMPLETED;
}
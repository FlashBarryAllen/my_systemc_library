#include "src.h"

src::src(sc_core::sc_module_name name) : m_src_sk("src_sk"), m_val(0), m_credit(0) {
    m_src_sk.register_nb_transport_bw(this, &src::nb_transport_bw);
    SC_METHOD(mth_entry);
    sensitive << m_clk.pos();
    dont_initialize();

    m_cycle_cnt = 0;

    m_logger = spdlog::basic_logger_mt(this->name(), std::string("logs/") + this->name() + ".log", true);
    m_logger->set_level(spdlog::level::info);
    m_logger->set_pattern("[%Y-%m-%d %H:%M:%S] %v");
}

void src::mth_entry() {  
    m_cycle_cnt++;  
    cal_speed();

    auto p_api = new MY_API_T();
    p_api->msg.type = DATA_MSG;

    auto p_dat = std::make_shared<MY_DAT_T>();
    p_dat->a = 10 + m_val;
    p_dat->b = 'a';
    p_dat->c = 101;
    
    p_api->dat = p_dat;

    m_logger->info("SRC print a: 0x{:x}", p_dat->a);

    tlm::tlm_generic_payload trans;
    trans.set_command(tlm::TLM_WRITE_COMMAND);
    trans.set_data_ptr((unsigned char*)p_api);
    tlm::tlm_phase phase = tlm::BEGIN_REQ;
    sc_core::sc_time time = sc_core::SC_ZERO_TIME;

    if (m_credit > 0) {
        m_src_sk->nb_transport_fw(trans, phase, time);
        m_credit--;
        m_val++;
    }
}

void src::cal_speed() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    std::cout << "当前时间：" << ts.tv_sec << "." << ts.tv_nsec << std::endl;

    double cur_us = sc_core::sc_time_stamp().to_seconds() * 10e6;
    
    std::cout << cur_us << std::endl;
}

tlm::tlm_sync_enum src::nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase,
    sc_core::sc_time& time) {
        m_logger->info("[SRC][RCV] cycle: {:d}", m_cycle_cnt);

        auto p_api = (MY_API_T*)trans.get_data_ptr();
        auto p_dat = static_pointer_cast<MY_DAT_T>(p_api->dat);
        auto type = p_api->msg.type;

        if (type == CTRL_MSG) {
            m_logger->info("[SRC][RCV] credit: {:d}", p_api->msg.credit);
            m_credit += p_api->msg.credit;
        } else if (type == DATA_MSG) {
            m_logger->info("[SRC][RCV] a: {:d}", p_dat->a);
        }

        delete p_api;
        return tlm::TLM_COMPLETED; 
}

void src::set_credit(int credit) {
    m_credit = credit;
}
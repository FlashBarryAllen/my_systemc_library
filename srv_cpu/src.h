#ifndef SRC_H
#define SRC_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include "common.h"

class src : public sc_core::sc_module
{
    public:
        SC_HAS_PROCESS(src);
        src(sc_core::sc_module_name name) : m_src_sk("src_sk"), m_val(0), m_credit(0) {
            m_src_sk.register_nb_transport_bw(this, &src::nb_transport_bw);
            SC_METHOD(mth_entry);
            sensitive << m_clk.pos();
            dont_initialize();
        }

        virtual void mth_entry() {            
            auto p_api = new MY_API_T();
            p_api->msg.type = DATA_MSG;

            auto p_dat = std::make_shared<MY_DAT_T>();
            p_dat->a = 10 + m_val;
            p_dat->b = 'a';
            p_dat->c = 101;
            
            p_api->dat = p_dat;
            std::cout << "[SRC][SND] cycle:" << sc_time_stamp() << " a:" << p_dat->a << std::endl;

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

        virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase,
            sc_core::sc_time& time) {
                std::cout << "[SRC][RCV] cycle:" << sc_time_stamp() << std::endl;

                auto p_api = (MY_API_T*)trans.get_data_ptr();
                auto p_dat = static_pointer_cast<MY_DAT_T>(p_api->dat);
                auto type = p_api->msg.type;

                if (type == CTRL_MSG) {
                    std::cout << "[SRC][RCV] credit:" << p_api->msg.credit << std::endl;
                    m_credit += p_api->msg.credit;
                } else if (type == DATA_MSG) {
                    std::cout << "[SRC][RCV] a:" << p_dat->a << std::endl;
                }

                delete p_api;
                return tlm::TLM_COMPLETED; 
        }

        void set_credit(int credit) {
            m_credit = credit;
        }

    public:
        int m_val;
        int m_credit;
        sc_core::sc_in_clk m_clk;
        tlm_utils::simple_initiator_socket<src> m_src_sk;
};

#endif
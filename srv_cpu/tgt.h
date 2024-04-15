#ifndef TGT_H
#define TGT_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include "peq.h"

class tgt : public sc_core::sc_module
{
    public:
        SC_HAS_PROCESS(tgt);
        tgt(sc_core::sc_module_name name) : m_tgt_sk("tgt_sk"), m_peq("peq"), m_crdt_sch_cycle(2)
            , m_print_cycle(0) {
            m_tgt_sk.register_nb_transport_fw(this, &tgt::nb_transport_fw);
            SC_METHOD(mth_entry);
            sensitive << m_clk.pos();
            dont_initialize();
        }

        void mth_dat_proc()
        {
            if (m_rcv_dat.size() != 0) {
                auto p_dat = m_rcv_dat.front();
                m_peq.delay(*p_dat, 3);
                auto p_ctl = make_shared<MY_API_T>();
                p_ctl->msg.type = CTRL_MSG;
                p_ctl->msg.credit = 2;
                m_snd_ctl.push_back(p_ctl);
                m_rcv_dat.pop_front();
            }
        }

        void mth_get_peq() {
            shared_ptr<MY_DAT_T> p_dat = nullptr;
            while ((p_dat = m_peq.get_next_transaction()) != nullptr) {
                //std::cout << "sc_time_stamp: " << sc_time_stamp() << std::endl;
                m_peq_que.push_back(p_dat);
            }
        }

        void mth_snd_msg() {
            if (m_snd_ctl.size() != 0 && m_print_cycle % m_crdt_sch_cycle == 0) {
                auto p_ctl = m_snd_ctl.front();

                tlm::tlm_generic_payload trans;
                auto p_api = new MY_API_T();
                p_api->msg.type = p_ctl->msg.type;
                p_api->msg.credit = p_ctl->msg.credit;

                trans.set_data_ptr((unsigned char*)p_api);
                tlm::tlm_phase phase = tlm::BEGIN_REQ;
                sc_time time = SC_ZERO_TIME;
                
                m_tgt_sk->nb_transport_bw(trans, phase, time);

                m_snd_ctl.pop_front();
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
                
                auto status = m_tgt_sk->nb_transport_bw(trans, phase, time);
                if (status == tlm::TLM_COMPLETED) {
                    m_snd_msg_que.pop_front();
                } else {

                }


                std::cout << "[TGT][SND] cycle:" << sc_time_stamp() << std::endl;
            }
        }

        void mth_entry() {
            m_print_cycle++;

            mth_dat_proc();
            mth_get_peq();
            mth_snd_msg();
        }

        virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase,
            sc_core::sc_time& time) {
                auto p_api = (MY_API_T*)trans.get_data_ptr();
                auto p_dat = std::static_pointer_cast<MY_DAT_T>(p_api->dat);
                m_rcv_dat.push_back(p_dat);
                delete p_api;

                std::cout << "[TGT][RCV] cycle:" << sc_time_stamp() << std::endl;
                return tlm::TLM_COMPLETED;
        }

    public:
        int m_print_cycle;
        int m_crdt_sch_cycle;
        little_end::peq<MY_DAT_T> m_peq;
        sc_core::sc_in_clk m_clk;
        tlm_utils::simple_target_socket<tgt> m_tgt_sk;
        deque<shared_ptr<MY_DAT_T>> m_rcv_dat;
        deque<shared_ptr<MY_API_T>> m_snd_ctl;
        deque<shared_ptr<MY_DAT_T>> m_peq_que;
        deque<shared_ptr<MY_DAT_T>> m_snd_msg_que;
};

#endif
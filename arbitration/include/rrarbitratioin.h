#include <systemc>
#include <queue>

const int kInputPortNum = 4;

class RRArbitration : public sc_core::sc_module {
public:
    SC_HAS_PROCESS(RRArbitration);
    RRArbitration(sc_core::sc_module_name name, int arb_num) : m_arb_num(arb_num), socket("socket") {
        socket.register_nb_transport_bw(this, &RRArbitration::nb_transport_bw);
        m_gnt_id = 0xFFFF;
        m_last_grant_id = m_arb_num - 1;

        for (int i = 0; i < m_arb_num; ++i) {
            m_input[i] = &m_input[i];
        }

        SC_METHOD(run);
        sesitive << m_clk.pos();
    }

    run() {
        // check whether have input
        bool t_have_input = false;
        for (int i = 0; i < m_arb_num; ++i) {
            if (m_input[i]->size()) {
                t_have_input = true;
                break;
            }
        }
        if (!t_have_input) {
            continue;
        }
    
        // check whether there is backpressure downstream TBD
    
        // arb
        m_gnt_id = 0xFFFF; // check gnt use
        for (int i = 0; i < m_arb_num; ++i) {
            m_last_grant_id = (m_last_grant_id + 1) % m_arb_num;
            if (m_input[m_last_grant_id]->size()) {
                m_gnt_id = m_last_grant_id;
                break;
            } 
        }

        assert(m_gnt_id != 0xFFFF);
        cout << "[" << sc_time_stamp()
            << "] Normal RR arb finish, gnt input id = " << m_gnt_id << endl;
        m_input[m_gnt_id]->pop_front();
    
        // if grant input need consume multi beats to send data,
        // here can control the period according to the grant id
        //wait(1 * m_period);
    }

    virtual tlm::tlm_sync_enum nb_transport_bw() {
        return tlm::TLM_COMPLETED;
    }

public:
    int m_arb_num;
    int m_gnt_id;
    int m_last_grant_id;
    sc_core::sc_in_clk m_clk;
    // unified input signal name
    std::deque<int> *m_input[kInputPortNum];
    tlm_utils::simple_target_socket<RRArbitration> socket;
};


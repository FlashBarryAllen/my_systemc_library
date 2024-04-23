#include "sch.h"

basic_sch::basic_sch(int arb_num)
{
    m_arb_num = arb_num;
    m_last_grant_id = m_arb_num - 1;
    m_input.resize(m_arb_num);
}

basic_sch::~basic_sch()
{

}

void basic_sch::set_val(int i, int val)
{
    //assert(i < m_arb_num);
    m_input[i].push_back(val);
}

int basic_sch::rr_sch()
{
    // check whether have input
    bool have_input = false;
    for (int i = 0; i < m_arb_num; ++i) {
        if (m_input[i].size()) {
            have_input = true;
            break;
        }
    }

    if (!have_input) {
        return -1;
    }

    // arb
    for (int i = 0; i < m_arb_num; i++) {
        m_last_grant_id = (m_last_grant_id + 1) % m_arb_num;
        if (m_input[m_last_grant_id].size()) {
            return m_last_grant_id;
        } 
    }

    return -1;
}

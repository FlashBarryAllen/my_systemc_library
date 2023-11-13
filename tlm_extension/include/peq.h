#ifndef PEQ_H
#define PEQ_H

#include <systemc>
#include <map>

namespace little_end {

template <class transaction_type>
class peq : public sc_core::sc_object
{
public:
    peq(const char* name, const int freq) : sc_core::sc_object(name)
    {
        m_period = sc_time(1.0 * 1000 / freq, SC_NS);
    }

    void delay(transaction_type& trans, const int t)
    {
        m_scheduled_events.insert(pair_type(t * m_period + sc_core::sc_time_stamp(), &trans));
    }

    void delay(transaction_type& trans)
    {
        m_scheduled_events.insert(pair_type(sc_core::sc_time_stamp(), &trans));
    }

    transaction_type* get_next_transaction()
    {
        if (m_scheduled_events.empty()) {
            return nullptr;
        }

        sc_core::sc_time now = sc_core::sc_time_stamp();

        if (m_scheduled_events.begin()->first <= now) {
            transaction_type* trans = m_scheduled_events.begin()->second;
            m_scheduled_events.erase(m_scheduled_events.begin());
            return trans;
        } else {
            return nullptr;
        }
    }

    // Cancel all events from the event queue
    void cancel_all() {
        m_scheduled_events.clear();
    }

private:
    sc_time    m_period;
    std::multimap<const sc_core::sc_time, transaction_type*> m_scheduled_events;

private:
    typedef std::pair<const sc_core::sc_time, transaction_type*> pair_type;

};

}

#endif

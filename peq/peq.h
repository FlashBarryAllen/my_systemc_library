#ifndef PEQ_H
#define PEQ_H

#include <systemc>
#include <map>

namespace little_end {

template <class transaction_type>
class peq : public sc_core::sc_object
{
public:
    peq(const char* name, const int freq = 1000) : sc_core::sc_object(name), m_freq(freq)
    {
        
    }

    void set_freq(const int freq) {
        m_freq = freq;
    }

    void delay(transaction_type trans, const int t)
    {
        sc_time period = sc_time(1.0 * 1000 / m_freq, SC_NS);
        m_scheduled_events.insert(pair_type(t * period + sc_core::sc_time_stamp(), trans));
    }

    void delay(transaction_type trans)
    {
        m_scheduled_events.insert(pair_type(sc_core::sc_time_stamp(), trans));
    }

    shared_ptr<transaction_type> get_next_transaction()
    {
        if (m_scheduled_events.empty()) {
            return nullptr;
        }

        sc_core::sc_time now = sc_core::sc_time_stamp();

        if (m_scheduled_events.begin()->first <= now) {
            shared_ptr<transaction_type> trans = m_scheduled_events.begin()->second;
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
    int        m_freq;
    std::multimap<const sc_core::sc_time, shared_ptr<transaction_type>> m_scheduled_events;

private:
    typedef std::pair<const sc_core::sc_time, shared_ptr<transaction_type>> pair_type;
};

}

#endif

#ifndef DELAY_H
#define DELAY_H

#include <systemc>
#include <map>

namespace theo_end {

template <class transaction_type>
class delay : public sc_core::sc_object
{
public:
    delay(const char* name) : sc_core::sc_object(name)
    {
    }

    void notify(transaction_type& trans, const sc_core::sc_time& t)
    {
        m_scheduled_events.insert(pair_type(t + sc_core::sc_time_stamp(), &trans));
    }

    void notify(transaction_type& trans)
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
    std::multimap<const sc_core::sc_time, transaction_type*> m_scheduled_events;

private:
    typedef std::pair<const sc_core::sc_time, transaction_type*> pair_type;

};

}

#endif

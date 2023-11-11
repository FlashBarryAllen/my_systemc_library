#ifndef DELAY_H
#define DELAY_H

#include <systemc>
#include <map>

namespace tlm_utils {

template <class PAYLOAD>
class delay : public sc_core::sc_object
{
public:
  typedef PAYLOAD transaction_type;
  typedef std::pair<const sc_core::sc_time, transaction_type*> pair_type;

public:
  delay(const char* name) : sc_core::sc_object(name)
  {
  }

  void notify(transaction_type& trans, const sc_core::sc_time& t)
  {
    m_scheduled_events.insert(pair_type(t + sc_core::sc_time_stamp(), &trans));
    m_event.notify(t);
  }

  void notify(transaction_type& trans)
  {
    m_scheduled_events.insert(pair_type(sc_core::sc_time_stamp(), &trans));
    m_event.notify(); // immediate notification
  }

  // needs to be called until it returns 0
  transaction_type* get_next_transaction()
  {
    if (m_scheduled_events.empty()) {
      return 0;
    }

    sc_core::sc_time now = sc_core::sc_time_stamp();
    if (m_scheduled_events.begin()->first <= now) {
      transaction_type* trans = m_scheduled_events.begin()->second;
      m_scheduled_events.erase(m_scheduled_events.begin());
      return trans;
    }

    m_event.notify(m_scheduled_events.begin()->first - now);

    return 0;
  }

  sc_core::sc_event& get_event()
  {
    return m_event;
  }

  // Cancel all events from the event queue
  void cancel_all() {
    m_scheduled_events.clear();
    m_event.cancel();
  }

private:
  std::map<const sc_core::sc_time, transaction_type*> m_scheduled_events;
  sc_core::sc_event m_event;
};

}

#endif

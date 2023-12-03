#include <cmath>
#include <deque>
#include <iomanip>
#include <iostream>
 
#include <systemc>
 
using namespace std;
using namespace sc_core;
 
const int kInputPortNum = 4;
 
class TestPlatform : public sc_module {
public:
  SC_HAS_PROCESS(TestPlatform);
 
  TestPlatform(const sc_module_name &name)
      : sc_module(name), m_period(sc_time(1000, SC_PS)) {
    SC_THREAD(PushPeq);
    SC_THREAD(RRArbitration);
    //SC_THREAD(QoSRRArbitration);
    //SC_THREAD(QoSRRPrioLevelArbitration);
  };
  ~TestPlatform() = default;
 
private:
  void PushPeq();
  void RRArbitration(); // normal round-robin
  void QoSRRArbitration(); // round-robin based one level
  void QoSRRPrioLevelArbitration(); // round-robin per priority level
 
public:
  sc_time m_period;
  std::deque<int> m_input[kInputPortNum * 3];
  sc_event m_push_evt;
};
 
void TestPlatform::PushPeq() {
 
  m_input[0].push_back(2);
  m_input[1].push_back(1);
  m_input[2].push_back(3);
  m_input[3].push_back(0);

  //m_input[0].push_back(2);
  //m_input[1].push_back(1);
  m_input[2].push_back(3);
  //m_input[3].push_back(0);
 
  m_input[4].push_back(2);
  m_input[5].push_back(1);
  m_input[6].push_back(3);
  m_input[7].push_back(0);
 
  m_input[8].push_back(2);
  m_input[9].push_back(1);
  m_input[10].push_back(3);
  m_input[11].push_back(0);
 
}
 
void TestPlatform::RRArbitration() {
  const int t_arb_num = kInputPortNum;
  int t_last_grant_id = t_arb_num - 1;
  int t_gnt_id = 0xFFFF;
  // unified input signal name
  std::deque<int> *t_input[kInputPortNum];
  for (int i = 0; i < t_arb_num; ++i) {
    t_input[i] = &m_input[i];
  }
  while (1) {
    // check whether have input
    bool t_have_input = false;
    for (int i = 0; i < t_arb_num; ++i) {
      if (t_input[i]->size()) {
        t_have_input = true;
        break;
      }
    }
    if (!t_have_input) {
      //wait(m_push_evt);
      continue;
    }
 
    // check whether there is backpressure downstream
 
    // arb
    t_gnt_id = 0xFFFF; // check gnt use
    for (int i = 0; i < t_arb_num; ++i) {
      t_last_grant_id = (t_last_grant_id + 1) % t_arb_num;
      if (t_input[t_last_grant_id]->size()) {
        t_gnt_id = t_last_grant_id;
        break;
      } 
    }
    assert(t_gnt_id != 0xFFFF);
    cout << "[" << sc_time_stamp()
         << "] Normal RR arb finish, gnt input id = " << t_gnt_id << endl;
    t_input[t_gnt_id]->pop_front();
 
    // if grant input need consume multi beats to send data,
    // here can control the period according to the grant id
    wait(1 * m_period);
  }
}
 
void TestPlatform::QoSRRArbitration() {
  wait(10,SC_NS);
  const int t_arb_num = kInputPortNum;
  const int t_priority_level = 4;  
  const int t_qos_low_bit = std::log2(t_arb_num) + 1;
  int t_last_grant_id = t_arb_num - 1;
  int t_gnt_id = 0xFFFF;  
  int t_max_priority = -1;
  // unified input signal name
  std::deque<int> *t_input[kInputPortNum];
  for (int i = 0; i < t_arb_num; ++i) {
    t_input[i] = &m_input[i + 4];
  }
  while (1) {
    // check whether have input
    bool t_have_input = false;
    for (int i = 0; i < t_arb_num; ++i) {
      if (t_input[i]->size()) {
        t_have_input = true;
        break;
      }
    }
    if (!t_have_input) {
      wait(m_push_evt);
      continue;
    }
 
    // check whether there is backpressure downstream
 
    // arb
    cout << "[" << sc_time_stamp()
         << "] input port id       Qos Value       input port prio      input "
            "total prio: (Qos value << "
         << t_qos_low_bit << ") + port prio" << endl;
    t_gnt_id = 0xFFFF;   // check gnt use
    t_max_priority = -1; // clear
    for (int i = 0; i < t_arb_num; ++i) {
      if (t_input[i]->size()) {
        int t_input_port_priority =
            (t_arb_num - (i - t_last_grant_id)) % t_arb_num;
        int t_qos_id = t_input[i]->front();
        
        // for + symbol priority greater than << / >>, so here must add ()
        int t_total_prioity =
            (t_qos_id << t_qos_low_bit) + t_input_port_priority;
        if (t_total_prioity > t_max_priority) {
          t_max_priority = t_total_prioity;
          t_gnt_id = i;
        }
        cout << "[" << sc_time_stamp() << "] " << setw(8) << i << setw(20)
             << t_qos_id << setw(20) << t_input_port_priority << setw(20)
             << t_total_prioity << endl;
      }
    }
    assert(t_gnt_id != 0xFFFF);
    cout << "[" << sc_time_stamp()
         << "] QoS based RR arb finish, gnt input id = " << t_gnt_id << endl;
    t_input[t_gnt_id]->pop_front();
    t_last_grant_id = t_gnt_id;
 
    // if grant input need consume multi beats to send data,
    // here can control the period according to the grant id
    wait(1 * m_period);
  }
}
 
void TestPlatform::QoSRRPrioLevelArbitration() {
  wait(20,SC_NS);
  const int t_arb_num = kInputPortNum;
  const int t_priority_level = 4;
  std::vector<int> t_last_grant_id (t_priority_level, t_arb_num - 1 );
  int t_gnt_id = 0xFFFF;
  
  // unified input signal name
  std::deque<int> *t_input[kInputPortNum];
  for (int i = 0; i < t_arb_num; ++i) {
    t_input[i] = &m_input[i + 8];
  }
  while (1) {
    // check whether have input
    bool t_have_input = false;
    for (int i = 0; i < t_arb_num; ++i) {
      if (t_input[i]->size()) {
        t_have_input = true;
        break;
      }
    }
    if (!t_have_input) {
      wait(m_push_evt);
      continue;
    }
 
    // check whether there is backpressure downstream
 
    // arb
    t_gnt_id = 0xFFFF; // check gnt use
    for (int level = t_priority_level - 1 ; level >= 0 ; --level) {
      for (int i = 0; i < t_arb_num; ++i) {
        t_last_grant_id[level] = (t_last_grant_id[level] + 1) % t_arb_num;
        if (t_input[t_last_grant_id[level]]->size()) {
          int t_qos_id = t_input[t_last_grant_id[level]]->front();
          if(t_qos_id == level){            
            t_gnt_id = t_last_grant_id[level];
            break;
          }
        } 
      }
      if(t_gnt_id != 0xFFFF){
        break;
      }
    }
    assert(t_gnt_id != 0xFFFF);
    cout << "[" << sc_time_stamp()
         << "] RR per priority level arb finish, gnt input id = " << t_gnt_id 
         <<" , req Qos value= " << t_input[t_gnt_id]->front() << endl;
    t_input[t_gnt_id]->pop_front();
 
    // if grant input need consume multi beats to send data,
    // here can control the period according to the grant id
    wait(1 * m_period);
  }
}
 
int sc_main(int argc, char **argv) {
  TestPlatform *m_platform;
  m_platform = new TestPlatform("TestPlatform");
  sc_start(30, SC_NS);
  return 0;
}
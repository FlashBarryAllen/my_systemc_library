#ifndef SCH_H
#define SCH_H

#include <vector>
#include <deque>

using namespace std;

class basic_sch {
    public:
        basic_sch(int arb_num);
        ~basic_sch();

        void set_val(int i, int val);
        int rr_sch();
    
    public:
        int m_arb_num;
        int m_last_grant_id;
        vector<deque<int>> m_input;
};

#endif
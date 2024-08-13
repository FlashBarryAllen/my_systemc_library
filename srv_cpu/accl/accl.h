#ifndef _ACCL_H_
#define _ACCL_H_

#include <systemc>
#include <map>
#include <deque>
#include "peq.h"
#include "common_type.h"

using namespace std;

class accl : public sc_core::sc_module
{
public:
    SC_HAS_PROCESS(accl);
    accl(sc_core::sc_module_name name);

    void mth_entry();

    void split_sg();
    void split_stripe();
    void split_su();
    void split_rdtxn();

    void mth_get_peq();

    void reclaim_rdtxn();
    void reclaim_su(int su_id);
    void reclaim_stripe(int stripe_id);
    void reclaim_sg(int sg_id);
    

public:
    std::shared_ptr<spdlog::logger> m_logger;
    sc_core::sc_in_clk m_clk;
    little_end::peq<int> m_peq;
    int m_cycle_cnt;
    int m_k_size;
    int m_sg_num;

    int m_stripe_cnt;
    int m_su_cnt;
    int m_rdtxn_cnt;
    int m_dat_cnt;

    map<int, int> m_stripe_sg_id;
    map<int, int> m_su_stripe_id;
    map<int, int> m_rdtxn_su_id;
    map<int, int> m_dat_rdtxn_id;

    deque<int> m_sg_pool;
    deque<int> m_stripe_pool;
    deque<int> m_su_pool;
    deque<int> m_rdtxn_pool;
    deque<int> m_dat_pool;

    deque<int> m_stripe_split;
    deque<int> m_su_split;
    deque<int> m_rdtxn_split;
    deque<int> m_dat_split;

    deque<shared_ptr<int>> m_peq_que;

    int m_sg_size;
    int m_stripe_size;
    int m_su_size;
    int m_rdtxn_size;
    int m_dat_size;

    map<int, int> m_sg_ctx;
    map<int, int> m_stripe_ctx;
    map<int, int> m_su_ctx;
    map<int, int> m_rdtxn_ctx;
    map<int, int> m_dat_ctx;
};

#endif
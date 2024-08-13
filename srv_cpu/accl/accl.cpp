#include "accl.h"

accl::accl(sc_core::sc_module_name name)
{
    SC_METHOD(mth_entry);
    sensitive << m_clk.pos();
    dont_initialize();

    m_logger = spdlog::basic_logger_mt(this->name(), std::string("logs/") + this->name() + ".log", true);
    m_logger->set_level(spdlog::level::info);
    m_logger->set_pattern("[%H:%M:%S %z] %v");

    m_cycle_cnt = 0;
    m_k_size    = 4;
    m_sg_num    = 3;

    m_stripe_cnt = 0;
    m_su_cnt     = 0;
    m_rdtxn_cnt  = 0;
    m_dat_cnt    = 0;

    m_sg_size     = 10;
    m_stripe_size = 60;
    m_su_size     = 240;
    m_rdtxn_size  = 1000;
    m_dat_size    = 5000;

    for (int i = 0; i < m_sg_size; i++)
    {
        m_sg_pool.push_back(i);
    }
    
    for (int i = 0; i < m_stripe_size; i++)
    {
        m_stripe_pool.push_back(i);
    }
    
    for (int i = 0; i < m_su_size; i++)
    {
        m_su_pool.push_back(i);
    }
    
    for (int i = 0; i < m_rdtxn_size; i++)
    {
        m_rdtxn_pool.push_back(i);
    }
    
    for (int i = 0; i < m_dat_size; i++)
    {
        m_dat_pool.push_back(i);
    }
}


void accl::mth_entry()
{
    m_cycle_cnt++;

    split_sg();
    split_stripe();
    split_su();
    split_rdtxn();

    mth_get_peq();

    reclaim_rdtxn();
}


void accl::split_sg()
{
    if (m_sg_num > 0)
    {
        int sg_id = m_sg_pool.front();

       while (m_stripe_cnt < m_k_size)
        {
            if (m_stripe_pool.size() <= 0)
            {
                break;
            }

            m_stripe_cnt++;
            int stripe_id = m_stripe_pool.front();
            m_logger->info("cycle=>{:8d} split_sg sg_id={:d}", m_cycle_cnt, sg_id);
            m_logger->info("cycle=>{:8d} split_sg stripe_id={:d}", m_cycle_cnt, stripe_id);

            m_stripe_sg_id[stripe_id] = sg_id;
            m_stripe_pool.pop_front();

            m_stripe_split.push_back(stripe_id);
        }
        
        if (m_stripe_cnt == m_k_size)
        {
            m_stripe_cnt = 0;
            m_sg_pool.pop_front();
            m_sg_num--;
        }
    }

}

void accl::split_stripe()
{
    if (!m_stripe_split.empty())
    {
        int stripe_id = m_stripe_split.front();

        while (m_su_cnt < m_k_size)
        {
            if (m_su_pool.size() <= 0)
            {
                break;
            }

            m_su_cnt++;
            int su_id = m_su_pool.front();
            m_su_stripe_id[su_id] = stripe_id;

            m_logger->info("cycle=>{:8d} split_stripe stripe_id={:d}", m_cycle_cnt, stripe_id);
            m_logger->info("cycle=>{:8d} split_stripe su_id={:d}", m_cycle_cnt, su_id);

            m_su_pool.pop_front();
            m_su_split.push_back(su_id);
        }

        if (m_su_cnt == m_k_size)
        {
            m_su_cnt = 0;
            m_stripe_split.pop_front();
        }
    }
}

void accl::split_su()
{
    if (!m_su_split.empty())
    {
        int su_id = m_su_split.front();

        while (m_rdtxn_cnt < m_k_size)
        {
            if (m_rdtxn_pool.size() <= 0)
            {
                break;
            }

            m_rdtxn_cnt++;
            int rdtxn_id = m_rdtxn_pool.front();
            m_rdtxn_su_id[rdtxn_id] = su_id;

            m_logger->info("cycle=>{:8d} split_su su_id={:d}", m_cycle_cnt, su_id);
            m_logger->info("cycle=>{:8d} split_su rdtxn_id={:d}", m_cycle_cnt, rdtxn_id);

            m_rdtxn_pool.pop_front();
            m_rdtxn_split.push_back(rdtxn_id);
        }

        if (m_rdtxn_cnt == m_k_size)
        {
            m_rdtxn_cnt = 0;
            m_su_split.pop_front();
        }
    }
}

void accl::split_rdtxn()
{
    if (!m_rdtxn_split.empty())
    {
        int rdtxn_id = m_rdtxn_split.front();

        while (m_dat_cnt < m_k_size)
        {
            if (m_dat_pool.size() <= 0)
            {
                break;
            }

            m_dat_cnt++;
            int dat_id = m_dat_pool.front();
            m_dat_rdtxn_id[dat_id] = rdtxn_id;

            m_logger->info("cycle=>{:8d} split_rdtxn rdtxn_id={:d}", m_cycle_cnt, rdtxn_id);
            m_logger->info("cycle=>{:8d} split_rdtxn dat_id={:d}", m_cycle_cnt, dat_id);

            m_dat_pool.pop_front();
            int delay_time = rand() % 100;
            m_peq.delay(dat_id, delay_time);
        }

        if (m_dat_cnt == m_k_size)
        {
            m_dat_cnt = 0;
            m_rdtxn_split.pop_front();
        }
    }
}

void accl::mth_get_peq() {
    shared_ptr<int> p_dat = nullptr;
    while ((p_dat = m_peq.get_next_transaction()) != nullptr) {
        m_logger->info("cycle=>{:8d} reclaim_dat dat_id={:d}", m_cycle_cnt, *p_dat);
        m_peq_que.push_back(p_dat);
    }
}

void accl::reclaim_rdtxn()
{
    if (!m_peq_que.empty())
    {
        shared_ptr<int> p_dat = m_peq_que.front();
        int dat_id = *p_dat;

        int rd_txn_id = m_dat_rdtxn_id[dat_id];
        m_logger->info("cycle=>{:8d} reclaim_rdtxn rd_txn_id={:d}", m_cycle_cnt, rd_txn_id);

        if (m_rdtxn_ctx.find(rd_txn_id) == m_rdtxn_ctx.end())
        {
            m_rdtxn_ctx[rd_txn_id] = 1;
        }
        else
        {
            m_rdtxn_ctx[rd_txn_id]++;
        }

        m_dat_rdtxn_id.erase(m_dat_rdtxn_id.find(dat_id));

        if (m_rdtxn_ctx[rd_txn_id] == m_k_size)
        {
            int su_id = m_rdtxn_su_id[rd_txn_id];
            m_logger->info("cycle=>{:8d} reclaim_su su_id={:d}", m_cycle_cnt, su_id);
            if (m_su_ctx.find(su_id) == m_su_ctx.end())
            {
                m_su_ctx[su_id] = 1;
            }
            else
            {
                m_su_ctx[su_id]++;
            }

            m_rdtxn_su_id.erase(m_rdtxn_su_id.find(rd_txn_id));

            reclaim_su(su_id);

            m_rdtxn_pool.push_back(rd_txn_id);
            m_rdtxn_ctx.erase(m_rdtxn_ctx.find(rd_txn_id));
        }

        m_dat_pool.push_back(dat_id);
        m_peq_que.pop_front();
    }
}

void accl::reclaim_su(int su_id)
{
    if (m_su_ctx[su_id] == m_k_size)
    {
        int stripe_id = m_su_stripe_id[su_id];
        m_logger->info("cycle=>{:8d} reclaim_stripe stripe_id={:d}", m_cycle_cnt, stripe_id);
        if (m_stripe_ctx.find(stripe_id) == m_su_ctx.end())
        {
            m_stripe_ctx[stripe_id] = 1;
        }
        else
        {
            m_stripe_ctx[stripe_id]++;
        }

        m_su_stripe_id.erase(m_su_stripe_id.find(su_id));

        reclaim_stripe(stripe_id);

        m_su_pool.push_back(su_id);
        m_su_ctx.erase(m_su_ctx.find(su_id));
    }
}
void accl::reclaim_stripe(int stripe_id)
{
    if (m_stripe_ctx[stripe_id] == m_k_size)
    {
        int sg_id = m_stripe_sg_id[stripe_id];
        m_logger->info("cycle=>{:8d} reclaim_sg sg_id={:d}", m_cycle_cnt, sg_id);
        if (m_sg_ctx.find(sg_id) == m_su_ctx.end())
        {
            m_sg_ctx[sg_id] = 1;
        }
        else
        {
            m_sg_ctx[sg_id]++;
        }

        m_stripe_sg_id.erase(m_stripe_sg_id.find(stripe_id));

        reclaim_sg(sg_id);

        m_stripe_pool.push_back(stripe_id);
        m_stripe_ctx.erase(m_stripe_ctx.find(stripe_id));
    }
}

void accl::reclaim_sg(int sg_id)
{
    if (m_sg_ctx[sg_id] == m_k_size)
    {
        m_sg_pool.push_back(sg_id);
        m_sg_ctx.erase(m_sg_ctx.find(sg_id));
    }
}
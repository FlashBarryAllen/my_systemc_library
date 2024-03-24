#ifndef SC_FIFO_WITH_DELAY_H
#define SC_FIFO_WITH_DELAY_H

#include <deque>
#include <vector>

namespace sc_component {

template <typename type, int len = 16>
class sc_fifo_with_delay {
    public:
        sc_fifo_with_delay() {
            m_len = len;
            m_vec = new type[m_len];
            //m_vec.resize(m_len);
            m_free = m_len;
            m_wi = 0;
            m_ri = 0;
            m_num_readable = 0;
        }

        ~sc_fifo_with_delay() {

        }

        bool nb_write(int val) {
            if (m_que.size() == m_len) {
                return false;
            }

            m_que.push_back(val);
        }

        bool nb_read(type& val) {
            if (m_que.empty()) {
                return false;
            }

            val = m_que.front();
            m_que.pop_front();
            return true;
        }

        bool write(const type& val) {
            if (m_free == 0) {
                return false;
            }

            m_vec[m_wi] = val;
            m_wi = (m_wi + 1) % m_len;
            m_free--;

            return true;
        }

        bool read(type& val) {
            if (m_free == m_len) {
                return false;
            }

            val = m_vec[m_ri];
            m_vec[m_ri] = 0;
            m_ri = (m_ri + 1) % m_len;
            m_free++;

            return true;
        }

    private:
        int m_len;
        int m_wi;
        int m_ri;
        int m_free;
        int m_num_readable;
        type* m_vec;
        //std::vector<type> m_vec;
        std::deque<type> m_que;
};
};


#endif
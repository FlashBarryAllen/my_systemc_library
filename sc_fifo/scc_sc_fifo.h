#ifndef SCC_SC_FIFO_H
#define SCC_sSC_FIFO_H

#include <deque>

namespace sc_component {

template <class type, int len = 16>
class sc_fifo {
    public:
        sc_fifo() {
            m_len = len;
        }

        ~sc_fifo() {

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

    private:
        int m_len;
        std::deque<type> m_que;
};
};


#endif
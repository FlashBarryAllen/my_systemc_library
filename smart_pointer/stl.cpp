#include <iostream>
#include <vector>
#include <deque>
#include <array>

int main()
{
    //std::vector<std::deque<int>> m_que;
    //m_que.resize(4);

    std::array<std::deque<int>, 2> m_que;


    m_que[0].push_back(1);
    m_que[0].push_back(2);
    m_que[0].push_back(3);
    m_que[0].push_back(4);

    m_que[1].push_back(5);
    m_que[1].push_back(6);
    m_que[1].push_back(7);
    m_que[1].push_back(8);

    return 0;
}
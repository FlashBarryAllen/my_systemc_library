/* implement diagonal propagation arbiter */
#ifndef DPA_H
#define DPA_H

#include <vector>

class ArbiterCell; // Forward declaration

class DPA {
public:
    DPA(int input_port, int output_port);
    void init();
    void Request(int input_port, int output_port);
    void Arbitrate();
    int HasGranted(int input_port, int output_port);
    int IsValidPort(int port) const;

public:
    int m_ptr;
    std::vector<int> m_row_mask;
    std::vector<int> m_col_mask;
    int m_num_ports;
    std::vector<std::vector<ArbiterCell*>> m_arbiter_cells;
    std::vector<std::vector<int>> m_requests;
    std::vector<std::vector<int>> m_grants;
    std::vector<std::vector<int>> m_dpa_map;
};

#endif // DPA_H
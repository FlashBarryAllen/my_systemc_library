#include "dpa.h"
#include "algorithm"
#include <memory>
class ArbiterCell {
public:
    ArbiterCell()
    {
    }

    // Function to arbitrate m_requests based on request signal, north and west
    int Grant(int request, int rm, int lm) {
        int grant = 0;

        if (rm && lm) {
           grant = request;
        }

        return grant;
    }
};

DPA::DPA(int input_port, int output_port)
{
    int nums = std::max(input_port, output_port);
    m_num_ports = nums;
    m_ptr = 0;
    m_row_mask.resize(m_num_ports, 1);
    m_col_mask.resize(m_num_ports, 1);
    m_requests.resize(m_num_ports, std::vector<int>(m_num_ports, 0));
    m_grants.resize(m_num_ports, std::vector<int>(m_num_ports, 0));

    // Create arbiter_cells for each cell
    m_arbiter_cells.resize(m_num_ports, std::vector<ArbiterCell*>(m_num_ports));
    for (int row = 0; row < m_num_ports; row++) {
        for (int col = 0; col < m_num_ports; col++) {
            m_arbiter_cells[row][col] = new ArbiterCell();
        }
    }

    m_dpa_map.resize(2 * m_num_ports - 1, std::vector<int>(m_num_ports, 0));
    int num = 0;
    for (int row = m_num_ports - 1; row < 2* m_num_ports - 1; row++) {
        for (int col = 0; col < m_num_ports; col++) {
            m_dpa_map[row][col] = num;
            num++;
        }
    }

    for (int row = 0; row < m_num_ports - 1; row++) {
        m_dpa_map[row] = m_dpa_map[row + m_num_ports];
    }
}

void DPA::Request(int input_port, int output_port)
{
    if (IsValidPort(input_port) && IsValidPort(output_port)) {
        m_requests[input_port][output_port] = 1;
    }
}

int DPA::HasGranted(int input_port, int output_port)
{
    if (IsValidPort(input_port) && IsValidPort(output_port)) {
        return m_grants[input_port][output_port];
    }

    return 0;
}

void DPA::init()
{
    for (int row = 0; row < m_num_ports; row++) {
        m_row_mask[row] = 1;
        m_col_mask[row] = 1;
    }
}

void DPA::Arbitrate()
{
    init();

    // Perform arbitration for each cell
    int times = 0;
    for (int row = m_ptr; row < m_num_ports; row++) {
        times++;
        for (int col = 0; col < m_num_ports; col++) {
            int cur_index = m_dpa_map[row + m_num_ports - 1 - col][col];
            int i = cur_index / m_num_ports;
            int j = cur_index % m_num_ports;
            //std::cout << index << std::endl;

            int request = m_requests[i][j];

            int grant = m_arbiter_cells[row][col]->Grant(request, m_row_mask[i], m_col_mask[j]);
            m_grants[i][j] = grant;

            if (grant) {
                m_row_mask[i] = 0;
                m_col_mask[j] = 0;
            }
        }
    }

    int left_times = m_num_ports - times;
    for (int row = 0; row < left_times; row++) {
        for (int col = 0; col < m_num_ports; col++) {
            int cur_index = m_dpa_map[row + m_num_ports - 1 - col][col];
            int i = cur_index / m_num_ports;
            int j = cur_index % m_num_ports;

            int request = m_requests[i][j];

            int grant = m_arbiter_cells[row][col]->Grant(request, m_row_mask[i], m_col_mask[j]);
            m_grants[i][j] = grant;

            if (grant) {
                m_row_mask[row] = 0;
                m_col_mask[col] = 0;
            }
        }
    }

    m_ptr = (m_ptr + 1) % m_num_ports;
}

int DPA::IsValidPort(int port) const 
{
    return port >= 0 && port < m_num_ports;
}
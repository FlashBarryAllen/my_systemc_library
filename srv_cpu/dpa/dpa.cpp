#include "dpa.h"
#include "algorithm"
#include <memory>
class ArbiterCell {
public:
    ArbiterCell()
    {
        m_north = 0;
        m_west  = 0;
        m_south = 0;
        m_east  = 0;
    }

    // Function to arbitrate m_requests based on request signal, north and west
    int Grant(int request, int north, int west, int rm, int lm) {
        m_north = north;
        m_west  = west;
        m_south = m_north | (request & !m_west);
        m_east  = m_west  | (request & !m_north);

        int grant = false;

        if (rm && lm) {
           grant = request & !north & !west;
        }

        return grant;
    }

public:
    int m_north;
    int m_west;
    int m_south;
    int m_east;
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
        for (int col = 0; col < m_num_ports; col++) {
            m_arbiter_cells[row][col]->m_north = 0;
            m_arbiter_cells[row][col]->m_west  = 0;
            m_arbiter_cells[row][col]->m_south = 0;
            m_arbiter_cells[row][col]->m_east  = 0;
        }
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
            int request = m_requests[row][col];
            int north = (row > 0) ? m_arbiter_cells[row - 1][col]->m_south : 0;
            int west  = (col > 0) ? m_arbiter_cells[row][col - 1]->m_east : 0;
            int grant = m_arbiter_cells[row][col]->Grant(request, north, west, m_row_mask[row], m_col_mask[col]);
            m_grants[row][col] = grant;

            if (grant) {
                m_row_mask[row] = 0;
                m_col_mask[col] = 0;
            }
        }
    }

    int left_times = m_num_ports - times;
    for (int row = 0; row < left_times; row++) {
        for (int col = 0; col < m_num_ports; col++) {
            int request = m_requests[row][col];
            int north = (row > 0) ? m_arbiter_cells[row - 1][col]->m_south : 0;
            int west  = (col > 0) ? m_arbiter_cells[row][col - 1]->m_east : 0;
            int grant = m_arbiter_cells[row][col]->Grant(request, north, west, m_row_mask[row], m_col_mask[col]);
            m_grants[row][col] = grant;

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
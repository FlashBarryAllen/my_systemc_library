
#include "test.h"

// Function to perform diagonal propagation on a matrix
void diagonalPropagation(vector<vector<double>> &matrix) {
    int n = matrix.size(); // Size of the matrix (n x n)

    // Iterate over the diagonal elements
    for (int i = 0; i < n; i++) {
        double diagonalElement = matrix[i][i]; // Current diagonal element

        // Propagate the diagonal element to the remaining elements in the row and column
        for (int j = 0; j < n; j++) {
            if (i != j) {
                matrix[i][j] -= (matrix[i][0] * matrix[j][0]) / diagonalElement;
                matrix[j][i] -= (matrix[0][i] * matrix[0][j]) / diagonalElement;
            }
        }
    }
}

// Function to print the matrix
void printMatrix(const vector<vector<double>> &matrix) {
    int n = matrix.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void TEST_dpa()
{
    // Example matrix
    vector<vector<double>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    // Perform diagonal propagation
    diagonalPropagation(matrix);

    // Print the modified matrix
    cout << "Modified matrix:" << endl;
    printMatrix(matrix);

    return;
}

void TEST_islip()
{
    islip* myislip = new islip(4, 4);
    myislip->init_priority_ptr();

    while(1) {
        myislip->init();

        for (auto i = 0; i < 4; i++) {
            myislip->set_ql(i, 0);
            myislip->set_ql(i, 1);
            myislip->set_ql(i, 2);
            myislip->set_ql(i, 3);
        }

        myislip->islip_sch();

        auto ret = myislip->sch_result;

        for (auto i = 0; i < ret.size(); i++) {
            auto in = ret[i].first;
            auto out = ret[i].second;

            std::cout << "(" << in << ", " << out << ")" << std::endl;
        }

        std::cout << std::endl;
    }

    return;
}

void TEST_encap_pkt()
{
    const string data = "12345678";
    uint8_t* dstMac = new uint8_t[6];
    dstMac[0] = 12;
    uint8_t* srcMac = new uint8_t[6];
    srcMac[0] = 45;
    uint32_t sourceIP = 1020;
    uint32_t destinationIP = 3040;
    uint16_t sourcePort = 5555;
    uint16_t destinationPort = 80;

    EncapsulatePacket(data, dstMac, srcMac, sourceIP, destinationIP, sourcePort, destinationPort);

    return;
}

void TEST_sch()
{
    basic_sch sch = basic_sch(3);

    sch.set_val(0, 100);
    sch.set_val(1, 200);
    sch.set_val(2, 300);

    for (int i = 0; i < 6; i++) {
        int sch_ret = sch.rr_sch();

        if (sch_ret == -1) {
            continue;
        }

        int val = sch.m_input[sch_ret].front();
        sch.m_input[sch_ret].pop_front();

        std::cout << sch_ret << ", " << val << std::endl;

        sch.set_val(0, 400);
        sch.set_val(1, 500);
        sch.set_val(2, 600);
    }
}
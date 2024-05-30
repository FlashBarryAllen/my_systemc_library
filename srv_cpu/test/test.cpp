
#include "test.h"
#include <gtest/gtest.h>

int Add(int a, int b)
{
    return a + b;
}

TEST(AddTest, Basic) {
    std::cout << "test gtest" << std::endl;
  EXPECT_EQ(Add(1, 2), 3);
}

void TEST_dpa()
{
    // Test case 1: Basic functionality
    std::cout << "Test case 1: Basic functionality" << std::endl;
    dpa_scheduler arbiter1(4, 4);
    arbiter1.request(0, 2);
    arbiter1.request(0, 3);
    arbiter1.request(1, 0);
    arbiter1.request(1, 2);
    arbiter1.request(2, 0);
    arbiter1.request(2, 1);
    arbiter1.request(2, 3);
    arbiter1.request(3, 1);
    arbiter1.request(3, 3);

    for (int i = 0; i < arbiter1.m_num_ports * 2; i++) {
        std::cout << arbiter1.m_ptr << std::endl;
        arbiter1.arbitration();
        for (int row = 0; row < arbiter1.m_num_ports; row++) {
            for (int col = 0; col < arbiter1.m_num_ports; col++) {
                std::cout << arbiter1.m_grants[row][col] << ", ";
            }

            std::cout << " " << std::endl;
        }

        auto result = arbiter1.sch_result;
        
        bool grant_0_2 = arbiter1.has_granted(0, 2);
        bool grant_0_3 = arbiter1.has_granted(0, 3);
        bool grant_1_0 = arbiter1.has_granted(1, 0);
        bool grant_1_2 = arbiter1.has_granted(1, 2);
        bool grant_2_0 = arbiter1.has_granted(2, 0);
        bool grant_2_1 = arbiter1.has_granted(2, 1);
        bool grant_2_3 = arbiter1.has_granted(2, 3);
        bool grant_3_1 = arbiter1.has_granted(3, 1);
        bool grant_3_3 = arbiter1.has_granted(3, 3);
    }
    
    if (arbiter1.has_granted(1, 2)) {
        std::cout << "Input 1 is granted access to output 2." << std::endl;
    } else {
        std::cout << "Input 1 is not granted access." << std::endl;
    }
    
    if (arbiter1.has_granted(2, 0)) {
        std::cout << "Input 2 is granted access to output 0." << std::endl;
    } else {
        std::cout << "Input 2 is not granted access." << std::endl;
    }

    // Test case 2: Invalid port numbers
    std::cout << "\nTest case 2: Invalid port numbers" << std::endl;
    try {
        arbiter1.request(-1, 2); // Invalid input port
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    try {
        arbiter1.has_granted(4, 0); // Invalid output port
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    // Test case 3: No requests
    std::cout << "\nTest case 3: No requests" << std::endl;
    dpa_scheduler arbiter2(2, 2);
    arbiter2.arbitration();
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
        if (arbiter2.has_granted(i, j)) {
            std::cout << "Unexpected grant: Input " << i << " to output " << j << std::endl;
        }
        }
    }

    // Test case 4: Multiple requests for the same output port
    std::cout << "\nTest case 4: Multiple requests for the same output port" << std::endl;
    dpa_scheduler arbiter3(3, 3);
    arbiter3.request(0, 2);
    arbiter3.request(1, 2);
    arbiter3.request(2, 2);
    arbiter3.arbitration();
    if (arbiter3.has_granted(0, 2)) {
        std::cout << "Input 0 is granted access to output 2." << std::endl;
    } else {
        std::cout << "Input 0 is not granted access." << std::endl;
    }
    if (arbiter3.has_granted(1, 2)) {
        std::cout << "Input 1 is granted access to output 2." << std::endl;
    } else {
        std::cout << "Input 1 is not granted access." << std::endl;
    }
    if (arbiter3.has_granted(2, 2)) {
        std::cout << "Input 2 is granted access to output 2." << std::endl;
    } else {
        std::cout << "Input 2 is not granted access." << std::endl;
    }
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
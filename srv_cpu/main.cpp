#include "gtest/gtest.h"
#include "top.h"
using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

int sc_main(int argc, char* argv[]) {
    InitGoogleTest(&argc, argv);
    int ret_val = RUN_ALL_TESTS();

    std::cout << "hello server cpu" << std::endl;
    TEST_dpa();

    // TEST_islip();
    TEST_encap_pkt();

    TEST_sch();

    top srv_top("srv_top");

    A a("aa");
    BB b("bb");
    sc_core::sc_clock* clk = new sc_core::sc_clock("clk", 1, sc_core::SC_NS);
    a.clock(*clk);
    b.clock(*clk);
    sc_fifo<int> tx_rx;
    a.tx(tx_rx);
    b.rx(tx_rx);

    CC c("cc");
    DD d("dd");
    c.clk(*clk);
    d.clk(*clk);
    c.tx.bind(d.rx);

    sc_start(1000, sc_core::SC_NS);

    std::cout << "done" << std::endl;

    return 0;
}
#include "top.h"
#include "gtest/gtest.h"
using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;


int sc_main(int argc, char* argv[])
{
    InitGoogleTest(&argc, argv);
    int ret_val = RUN_ALL_TESTS();

    std::cout << "hello server cpu" << std::endl;
    TEST_dpa();

    //TEST_islip();
    TEST_encap_pkt();

    TEST_sch();

    top srv_top("srv_top");

    sc_start(10, sc_core::SC_NS);

    std::cout << "done" << std::endl;
    
    return 0;
}
#include "top.h"

int sc_main(int args, char* argv[])
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
    }

    return 0;

    std::cout << "hello server cpu" << std::endl;
    top srv_top("srv_top");

    sc_start(10, sc_core::SC_NS);
    
    return 0;
}
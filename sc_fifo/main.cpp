#include <iostream>
#include <memory>
#include "scc_sc_fifo.h"

template<int num = 5>
struct ec_cu{
    int val;
    bool arr[num]; 
    
    bool is_done() {
        for (auto i = 0; i < num; i++) {
            if (arr[i] == false) {
                return false;
            }
        }

        return true;
    }
};

int main() {
    sc_component::sc_fifo<int> test;

    std::shared_ptr<ec_cu<6>> cu = std::make_shared<ec_cu<6>>();

    for (int i = 0 ; i < 6; i++) {
        cu->arr[i] = true;
    }

    if (cu->is_done()) {
        std::cout << "is_done" << std::endl;
    }

    /*
    test.nb_write(1);
    test.nb_write(12);
    
    test.nb_write(9);

    int val = 0;
    test.nb_read(val);
    std::cout << val << std::endl;
    test.nb_read(val);
    std::cout << val << std::endl;
    test.nb_read(val);
    std::cout << val << std::endl;
    */

    for (auto i = 0; i < 20; i++) {
        test.write(i);
    }

    for (auto i = 0 ; i < 20; i++) {
        int val = 0;
        test.read(val);

        test.write(val);
        
        std::cout << val << std::endl;
    }


    return 0;
}
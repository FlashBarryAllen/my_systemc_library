#ifndef SCC_STAT_H
#define SCC_STAT_H

#include <string>
#include <iostream>

namespace my_scc {

class stat_pps {
    public:
        stat_pps(): total_sum(0), cur_sum(0) {

        }
        void stat(int num) {
            cur_sum = cur_sum + num;
        }

        std::string c_str() {
            str.clear();
            total_sum += cur_sum;

            str += std::to_string(total_sum);
            str += "|";
            str += std::to_string(cur_sum);

            cur_sum = 0;

            return str;
        }

    private:
        int total_sum;
        int cur_sum;
        std::string str;
};

class stat_bw {

};

class stat_depth {

};

}

#endif
#include <iostream>
#include <memory>

int main() {
    while (true)
    {
        static int ch_id = 0;
        int source = ch_id;

        int target = source % 12;

        if (ch_id == 12) {
            ch_id = 0;
        }
        ch_id++;

        std::cout << target << std::endl;
    }

    return 0;
}

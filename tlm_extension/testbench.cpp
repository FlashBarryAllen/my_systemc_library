#include "top.h"

int sc_main(int argc, char* argv[])
{
    Top top("top");
    sc_start(1, SC_US);

    cout << "\n***** Messages have been written to file output.txt                    *****\n";

    return 0;
}

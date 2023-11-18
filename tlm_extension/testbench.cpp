#include "top.h"

int sc_main(int argc, char* argv[])
{
    Top top("top");
    sc_start(3, SC_SEC);

    cout << "\n***** Messages have been written to file output.txt                    *****\n";

    return 0;
}

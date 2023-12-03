#include <systemc>

class A : public sc_core::sc_module {
    public:
        SC_HAS_PROCESS(A);
        A(sc_core::sc_module_name name) : m_tx_fifo("m_tx_fifo", 40) {
            SC_METHOD(mth_entry);
            dont_initialize();
            sensitive << m_clk_wr.pos();

            SC_METHOD(read_mth_entry);
            dont_initialize();
            sensitive << m_clk_rd.pos();
        }

        void mth_entry() {
            static int val = 100;
            rx->write(val++);
            //for (auto i = 0; i < 10; i++) {
                //m_tx_fifo.write(val++);
                m_tx_fifo.nb_write(val);
            //}
            return;
        }

        void read_mth_entry() {
            //int data = tx->read();

            //std::cout << "read_mth_entry:" << std::endl << data << std::endl;

            int data = 0;
            //m_tx_fifo.read(data);
            m_tx_fifo.nb_read(data);

            std::cout << data << std::endl;
        }

    public:
        sc_core::sc_in_clk  m_clk_wr;
        sc_core::sc_in_clk  m_clk_rd;
        sc_core::sc_port<sc_core::sc_signal_out_if<int>> rx;
        sc_core::sc_port<sc_core::sc_signal_in_if<int>>  tx;
        sc_core::sc_fifo<int> m_tx_fifo;
};

int sc_main(int args, char* argv[]) {
    A* aa = new A("aa");
    sc_core::sc_clock clk_write("clk_write", 1, sc_core::SC_NS);
    sc_core::sc_clock clk_read("clk_read", 1, sc_core::SC_NS);
    aa->m_clk_wr(clk_write);
    aa->m_clk_rd(clk_read);

    sc_core::sc_signal<int> s;
    aa->rx(s);
    aa->tx(s);

    sc_start(30, sc_core::SC_NS);
    return 0;
}
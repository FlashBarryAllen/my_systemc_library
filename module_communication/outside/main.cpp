#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <memory>

class A : public sc_core::sc_module {
    public:
        SC_HAS_PROCESS(A);
        A(sc_core::sc_module_name name, int val) : src_socket("src_socket"),
            tgt_socket("tgt_socket")
            {
            tgt_socket.register_nb_transport_fw(this, &A::nb_transport_fw);
            SC_METHOD(mth_entry);
            dont_initialize();
            sensitive << m_clk.pos();
        }

        void mth_entry() {
            std::cout << "mth_entry" << std::endl;
            tlm::tlm_generic_payload trans;
            int* data = new int(2);
            trans.set_data_ptr((unsigned char*) data);
            tlm::tlm_phase  phase = tlm::BEGIN_REQ;
            sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

            src_socket->nb_transport_fw(trans, phase, delay);

            return;
        }

        tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& payload,
            tlm::tlm_phase& phase, sc_core::sc_time& delay) {
                unsigned char* data = payload.get_data_ptr();
                std::cout << "A recieve data from B is " << *(int*)data << std::endl;
                return tlm::TLM_COMPLETED;
        }
    
    public:
        sc_core::sc_in_clk  m_clk;
        tlm_utils::simple_initiator_socket<A> src_socket;
        tlm_utils::simple_target_socket<A>    tgt_socket;
};

class B : public sc_core::sc_module {
    public:
        SC_HAS_PROCESS(B);
        B(sc_core::sc_module_name name) : src_socket("src_socket"),
            tgt_socket("tgt_socket") {
            tgt_socket.register_nb_transport_fw(this, &B::nb_transport_fw);
            SC_METHOD(mth_entry);
            dont_initialize();
            sensitive << m_clk.pos();
        }

        void mth_entry() {
            int* data = new int(1);
            tlm::tlm_generic_payload trans;
            trans.set_data_ptr((unsigned char*)data);
            tlm::tlm_phase phase = tlm::BEGIN_REQ;
            sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

            src_socket->nb_transport_fw(trans, phase, delay);

            return;
        }

        virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& payload,
            tlm::tlm_phase& phase, sc_core::sc_time& delay) {
                unsigned char* data = payload.get_data_ptr();
                std::cout << "B recieve data from A is " << *(int*)data << std::endl;
                delete data;
                return tlm::TLM_COMPLETED;
        }

    public:
        sc_core::sc_in_clk m_clk;
        tlm_utils::simple_initiator_socket<B> src_socket;
        tlm_utils::simple_target_socket<B>    tgt_socket;
};

class C : public sc_core::sc_module {
    public:
        SC_HAS_PROCESS(C);
        C(sc_core::sc_module_name name) {
            SC_METHOD(mth_entry);
            sensitive << m_clk.pos();
        }

        void mth_entry() {
            int val = m_rx->read();
            std::cout << val << std::endl;
        }

    public:
        sc_core::sc_in_clk m_clk;
        sc_core::sc_port<sc_core::sc_signal_in_if<int>> m_rx;
};

class D : public sc_core::sc_module {
    public:
        SC_HAS_PROCESS(D);
        D(sc_core::sc_module_name name) {
            SC_METHOD(mth_entry);
            sensitive << m_clk.pos();
        }

        void mth_entry() {
            static int val = 100;
            m_tx->write(val++);
        }
    
    public:
        sc_core::sc_in_clk m_clk;
        sc_core::sc_port<sc_core::sc_signal_out_if<int>> m_tx;
};

class TOP : public sc_core::sc_module {
    public:
        SC_HAS_PROCESS(TOP);
        TOP(sc_core::sc_module_name name) {
            aa = new A("aa", 10);
            bb = new B("bb");

            aa->m_clk(clk);
            bb->m_clk(clk);
            aa->src_socket.bind(bb->tgt_socket);
            bb->src_socket.bind(aa->tgt_socket);

            cc = new C("cc");
            dd = new D("dd");

            cc->m_clk(clk);
            dd->m_clk(clk);
            cc->m_rx(m_s);
            dd->m_tx(m_s);
        }
    
    public:
        A* aa;
        B* bb;
        C* cc;
        D* dd;
        sc_core::sc_signal<int> m_s;
        sc_core::sc_clock clk = sc_core::sc_clock("clock", 1, sc_core::SC_NS);
};

int sc_main(int args, char* argc[]) {
    TOP* top = new TOP("top");

    sc_start(10, sc_core::SC_NS);

    return 0;
}
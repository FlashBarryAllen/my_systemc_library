#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <memory>

class A : public sc_core::sc_module
{
public:
    SC_HAS_PROCESS(A);
    A(sc_core::sc_module_name name):m_src_socket("src_socket"), m_tgt_socket("tgt_socket") {
        m_tgt_socket.register_nb_transport_fw(this, &A::nb_transport_fw);
        SC_METHOD(mth_entry);
        sensitive << m_clk.pos();
    }

    void mth_entry() {
        std::cout << "A" << std::endl;
        int* dat = new int(10);
        tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload();
        trans->set_command(tlm::TLM_WRITE_COMMAND);
        trans->set_data_length(4);
        trans->set_data_ptr((unsigned char*)dat);
        tlm::tlm_phase phase = tlm::BEGIN_REQ;
        sc_core::sc_time time = sc_core::SC_ZERO_TIME;

        m_src_socket->nb_transport_fw(*trans, phase, time);

        m_tx->write(123);
        int val = m_rx->read();
        std::cout << val << std::endl;
    }

    tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& time) {
        unsigned char* dat = trans.get_data_ptr();
        std::cout << *(int*)dat << std::endl;
        return tlm::TLM_COMPLETED;
    }

    ~A() {

    }

public:
    sc_core::sc_in_clk m_clk;
    tlm_utils::simple_initiator_socket<A> m_src_socket;
    tlm_utils::simple_target_socket<A>    m_tgt_socket;
    sc_core::sc_port<sc_core::sc_signal_out_if<int>> m_tx;
    sc_core::sc_port<sc_core::sc_signal_in_if<int>>  m_rx;
};

class B : public sc_core::sc_module {
public:
    SC_HAS_PROCESS(B);
    B(sc_core::sc_module_name name) : m_src_socket("src_socket"), m_tgt_socket("tgt_socket") {
        m_tgt_socket.register_nb_transport_fw(this, &B::nb_transport_fw);
        SC_METHOD(mth_entry);
        sensitive << m_clk.pos();
    }

    void mth_entry() {
        std::cout << "B" << std::endl;
        int* dat = new int(0);
        *dat = val + 100;
        tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload();
        trans->set_command(tlm::TLM_WRITE_COMMAND);
        trans->set_data_ptr((unsigned char*)dat);
        trans->set_data_length(4);
        tlm::tlm_phase phase = tlm::BEGIN_REQ;
        sc_core::sc_time time = sc_core::SC_ZERO_TIME;
        m_src_socket->nb_transport_fw(*trans, phase, time);
    }

    tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase,sc_core::sc_time& time) {
        unsigned char* dat = trans.get_data_ptr();
        std::cout << *(int*)dat << std::endl;
        val = *(int*)dat;
        return tlm::TLM_COMPLETED;
    }

public:
    int val;
    sc_core::sc_in_clk m_clk;
    tlm_utils::simple_initiator_socket<B> m_src_socket;
    tlm_utils::simple_target_socket<B> m_tgt_socket;
};

class C : public sc_core::sc_module {
public:
    SC_HAS_PROCESS(C);
    C(sc_core::sc_module_name name) {
        SC_METHOD(mth_entry);
        sensitive << m_clk.pos();
    }

    void mth_entry() {
        m_tx->write(456);
    }

public:
    sc_core::sc_in_clk m_clk;
    sc_core::sc_port<sc_core::sc_signal_out_if<int>> m_tx;
};

class D : public sc_core::sc_module {
public:
    SC_HAS_PROCESS(D);
    D(sc_core::sc_module_name name) {
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

class top : public sc_core::sc_module {
public:
    SC_HAS_PROCESS(top);
    top(sc_core::sc_module_name name) {
        m_clk = new sc_core::sc_clock("clock", 1, sc_core::SC_NS);
        //a = new A("a");
        //b = new B("b");
        //a = std::make_shared<A>("a");
        //b = std::make_shared<B>("b");
        a = std::make_unique<A>("a");
        b = std::make_unique<B>("b");
        a->m_clk(*m_clk);
        b->m_clk(*m_clk);
        a->m_src_socket.bind(b->m_tgt_socket);
        b->m_src_socket.bind(a->m_tgt_socket);

        a->m_tx(m_s_a);
        a->m_rx(m_s_a);

        c = new C("c");
        d = new D("d");
        c->m_clk(*m_clk);
        d->m_clk(*m_clk);
        c->m_tx(m_s_cd);
        d->m_rx(m_s_cd);
    }

public:
    //std::shared_ptr<A> a;
    //std::shared_ptr<B> b;
    //A* a;
    //B* b;
    std::unique_ptr<A> a;
    std::unique_ptr<B> b;
    C* c;
    D* d;
    sc_core::sc_clock* m_clk;
    sc_core::sc_signal<int> m_s_a;
    sc_core::sc_signal<int> m_s_cd;
};

int sc_main(int args, char* argc[])
{
    sc_dt::sc_int<16> var;
    var = 10;

    top my_top("my_top");

    sc_start(10, sc_core::SC_NS);

    return 0;
}
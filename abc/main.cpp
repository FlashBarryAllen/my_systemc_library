#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

class A : public sc_core::sc_module 
{
public:
    SC_HAS_PROCESS(A);
    A(sc_core::sc_module_name name) : m_init_socket("init")
    {
        SC_METHOD(mth_entry);
        sensitive << m_clk.pos();       
    }

    ~A()
    {

    }

    void mth_entry() {
        int* data = new int(10);
        tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload();
        trans->set_command(tlm::TLM_WRITE_COMMAND);
        trans->set_data_ptr((unsigned char*)data);
        trans->set_data_length(4);
        tlm::tlm_phase phase = tlm::BEGIN_REQ;
        sc_core::sc_time time = sc_core::SC_ZERO_TIME;
        m_init_socket->nb_transport_fw(*trans, phase, time);
    }

public:
    int val;
    sc_core::sc_in_clk m_clk;
    tlm_utils::simple_initiator_socket<A> m_init_socket;
};

class B : public sc_core::sc_module
{
public:
    SC_HAS_PROCESS(B);
    B(sc_core::sc_module_name name) : m_tgt_socket("tgt")
    {
        m_tgt_socket.register_nb_transport_fw(this, &B::nb_transport_fw);
        SC_METHOD(mth_entry);
        sensitive << m_clk.pos();
    }

    void mth_entry()
    {

    }

    virtual tlm::tlm_sync_enum nb_transport_fw( tlm::tlm_generic_payload& trans, 
        tlm::tlm_phase& phase, sc_core::sc_time& delay ) {
        unsigned char* data = trans.get_data_ptr();
        std::cout << "B rcv A data = " << *(int*)data << std::endl;
        return tlm::TLM_COMPLETED;                                    
    }

public:
    int val;
    sc_core::sc_in_clk m_clk;
    tlm_utils::simple_target_socket<B> m_tgt_socket;
};

class TOP : public sc_core::sc_module
{
public:
    SC_HAS_PROCESS(TOP);
    TOP(sc_core::sc_module_name name)
    {
        a = new A("a");
        b = new B("b");
        sc_core::sc_clock* m_clk = new sc_core::sc_clock("clock", 1, sc_core::SC_NS);
        a->m_clk(*m_clk);
        b->m_clk(*m_clk);
        a->m_init_socket.bind(b->m_tgt_socket);
    }

public:
    A* a;
    B* b;
};

int sc_main(int argc, char* argv[])
{
    TOP top("top");
    sc_start(10, sc_core::SC_NS);
    return 0;
}
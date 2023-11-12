#include <iostream>

#include "systemc.h"
#include "peq.h"

using namespace std;

class TestPlatform : public sc_module
{
    public:
        SC_HAS_PROCESS(TestPlatform);
        TestPlatform(const sc_module_name& name, const int freq) : sc_module(name), 
            m_peq("test_peq", freq), m_trigger_flag(true)
        {
            SC_METHOD(GetPeq);
            sensitive << m_clk.pos();

            SC_METHOD(PushPeq_1);
            SC_METHOD(PushPeq_2);
        };

    public:
        void PushPeq_1();
        void PushPeq_2();
        void GetPeq();

        ~TestPlatform() {}
        
    public:
        bool       m_trigger_flag;
        sc_in_clk  m_clk;
        little_end::peq<unsigned int> m_peq;
};

void TestPlatform::PushPeq_1()
{    
    // the transaction that peq will delay can't be a temporary memory space
    for (auto i = 0; i < 2; i++) {
        unsigned int * t_num_1 = new  unsigned int;
		*t_num_1 = 100 + i;
    	m_peq.delay(*t_num_1 , 10 + i);
        cout<<"["<<sc_time_stamp()
            <<"] delay number 1 to peq, delay cycle = " << 10 + i
            <<endl;    
    }
}

void TestPlatform::PushPeq_2()
{
    if (m_trigger_flag) {
        next_trigger(2, SC_NS);
        m_trigger_flag = false;
        return;
    }

	for (auto i = 0; i< 2; i++) {
        unsigned int * t_num_2 = new  unsigned int;
        *t_num_2 =  200 + i;
        m_peq.delay(*t_num_2 , 3);
        cout<<"["<<sc_time_stamp()
            <<"] delay number 2 to peq, delay cycle = 3"
            <<endl;
	}
}

void TestPlatform::GetPeq()
{
    unsigned int * t_get = NULL;
    //here must get next transaction until t_get is NULL
    while((t_get = m_peq.get_next_transaction()) != NULL)
    {
        cout<<"["<<sc_time_stamp()
            <<"] get number "
            << * t_get
            <<endl;

        delete t_get; //dynamic memory space, delete when no use
        t_get = nullptr;  
    }
}

int sc_main(int argc, char** argv)
{
    TestPlatform* platform = new TestPlatform("TestPlatform", 1000);
    sc_clock clk("clk", 1, SC_NS);
    platform->m_clk(clk);

    sc_start(10, SC_US);
    
    return 0;
}


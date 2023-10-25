/*
Original 2020-03-15
README:
	This is a example to teach you how to use a peq in systemC/TLM
	Assume that you want to implement such a scenario:
	 A delay 10T to a pipeline at 0T; B delay 3T to the same pipeline at 2T

execute:	
	g++ -g -Wall -lsystemc -m64 -pthread main.cpp  -L/$(your systemc path)/lib-linux64 -I/$(your systemc path)/include  -I/$(your systemc path)/src/tlm_utils -o sim

you need particular attention that  
	1. the only constructor of peq_with_get have a paramater, \
		it have no default constructor,\
		 so you must initialize it use initialization list

		you can check the constructor at ~systemc-2.3.1/src/tlm_utils/peq_with_get.h 
		   peq_with_get(const char* name) : sc_core::sc_object(name)

	2. for other points, can check my csdn blog
*/


#include <iostream>
#include <memory>
#include "systemc.h"
#include "tlm_utils/peq_with_get.h"

using namespace std;

class TestPlatform
: public sc_module
{
	public:
		SC_HAS_PROCESS(TestPlatform);

		TestPlatform(const sc_module_name&    name)
        : sc_module(name)
		, m_period (sc_time(1000,SC_PS))
		, m_test_peq("test_peq")
		{
			SC_THREAD(PushPeq_1);
			SC_THREAD(PushPeq_2);

			SC_THREAD(GetPeq);
    		sensitive<<m_test_peq.get_event();//sensitive event list
		};

	public:
		void PushPeq_1();
		void PushPeq_2();
		void GetPeq();

		~TestPlatform()
		{;}
		
	public:
		sc_time 			m_period;
		tlm_utils::peq_with_get<shared_ptr<int> >   
							m_test_peq;
};

void TestPlatform::PushPeq_1()
{
	shared_ptr<int> t_num_1 = make_shared<int>(100);
	//* t_num_1 =  100;
	
	// the transaction that peq will notify can't be a temporary memory space
	m_test_peq.notify(t_num_1 , 10 * m_period );
	cout<<"["<<sc_time_stamp()
		<<"] notify number 1 to peq, notify cycle = 10"
		<<endl;
}

void TestPlatform::PushPeq_2()
{
	wait(2* m_period);
	shared_ptr<int> t_num_2 = make_shared<int>(200);
	//* t_num_2 =  200;
	
	m_test_peq.notify(t_num_2 , 3 * m_period );
	cout<<"["<<sc_time_stamp()
		<<"] notify number 2 to peq, notify cycle = 3"
		<<endl;
}

void TestPlatform::GetPeq()
{
    shared_ptr<int>* t_get = NULL;
    while(1)
    {
        wait(); //wait sensitive event list

		//here must get next transaction entil t_get is NULL
        while((t_get = m_test_peq.get_next_transaction()) != NULL)
        {
          cout<<"["<<sc_time_stamp()
            <<"] get number "
            << *t_get
            <<endl;

			//in this block, must can't wait any event or cycle delay
			// if not, the time of transaction obtained will not accurate
			// wait(10 * m_period);	
		}
	}
}

/*
//this is an error using example
void TestPlatform::GetPeq()
{
    shared_ptr<int> * t_get = NULL;
    while(1)
    {
        wait(); 
		//this is an error using example
		// get_next_transaction not until NULL &　wait other cycle 
		// will lead to number 1 missing
        t_get = m_test_peq.get_next_transaction();
		if(t_get != NULL)
        {
			cout<<"["<<sc_time_stamp()
				<<"] get number "
				<< * t_get
				<<endl;

			delete t_get; 
			t_get = NULL;

			wait(10 * m_period);	
		}
	}
}
*/


int sc_main(int argc, char** argv)
{
    TestPlatform *    m_platform;
    m_platform = new  TestPlatform("TestPlatform");
    sc_start(1,SC_US);
	return 0;
}


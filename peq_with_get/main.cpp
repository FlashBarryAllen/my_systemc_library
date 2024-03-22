#include <iostream>
#include <random>
#include <vector>

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
        unsigned int t_num_1 = 0;
		t_num_1 = 100 + i;
    	m_peq.delay(t_num_1 , 10 + i);
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
        unsigned int t_num_2 = 0;
        t_num_2 =  200 + i;
        m_peq.delay(t_num_2 , 3);
        cout<<"["<<sc_time_stamp()
            <<"] delay number 2 to peq, delay cycle = 3"
            <<endl;
	}
}

void TestPlatform::GetPeq()
{
    std::shared_ptr<unsigned int> t_get = nullptr;
    //here must get next transaction until t_get is NULL
    while((t_get = m_peq.get_next_transaction()) != NULL)
    {
        cout<<"["<<sc_time_stamp()
            <<"] get number "
            << *t_get
            <<endl;

        t_get = nullptr;  
    }
}

struct test {
	uint32_t b;
    uint32_t a;
	uint32_t c;
} __attribute__((aligned(1)));

struct MyStruct {
    int a;
    char b;
    double c;
} __attribute__((aligned(8)));

#include <iostream>
#include <random>
#include <vector>

std::vector<double> generateGaussianValues(double mean, double stddev, int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(mean, stddev);

    std::vector<double> gaussianValues;
    gaussianValues.reserve(count);

    for (int i = 0; i < count; ++i) {
        double randomValue = distribution(gen);
        gaussianValues.push_back(randomValue);
    }

    return gaussianValues;
}

#include <bits/stdc++.h>
using namespace std;

vector<int> prime_list;

void generate_primes(int upper_limit){
    vector<bool> bool_index (upper_limit, true);
    for(int i = 2; i<upper_limit; i++){
        if (bool_index[i]){
            ::prime_list.push_back(i);
            for(int j = i*i; j<upper_limit; j+=i){
                bool_index[j] = false;
            }
        }
    }
}

SC_MODULE(stim)
{
    sc_in<bool> Clk;

    void StimGen()
    {
        cout << "Hello World!\n";
        wait();
        cout << "Hello again, world!\n";
        wait();
    }
    SC_CTOR(stim)
    {
        SC_THREAD(StimGen);
        sensitive << Clk.pos();
    }
};

int sc_main(int argc, char** argv)
{
    /*
    TestPlatform* platform = new TestPlatform("TestPlatform", 1000);
    sc_clock clk("clk", 1, SC_NS);
    platform->m_clk(clk);

    sc_start(10, SC_US);
    */
   	/*
    struct test A;
	std::cout << "sizeof(struct test) = " << sizeof(struct test) << std::endl;
    */

   /*
    generate_primes(300000); //for big numbers it gives a segmentation fault
    for(int i: prime_list){
        cout<<i<<" ";
    }
    return 0;
    */

    sc_clock TestClk("clk", 10,SC_NS);

    stim Stim1("Stimulus");
    Stim1.Clk(TestClk);

    sc_start();  // run forever

    return 0;
}
